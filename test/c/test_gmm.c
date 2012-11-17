/*
Copyright © INRIA 2010-2011. 
Authors: Matthijs Douze & Herve Jegou 
Contact: matthijs.douze@inria.fr  herve.jegou@inria.fr

This software is a computer program whose purpose is to provide 
efficient tools for basic yet computationally demanding tasks, 
such as find k-nearest neighbors using exhaustive search 
and kmeans clustering. 

This software is governed by the CeCILL license under French law and
abiding by the rules of distribution of free software.  You can  use, 
modify and/ or redistribute the software under the terms of the CeCILL
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info". 

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability. 

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or 
data to be ensured and,  more generally, to use and operate it in the 
same conditions as regards security. 

The fact that you are presently reading this means that you have had
knowledge of the CeCILL license and that you accept its terms.
*/


/* frontend for the gmm function */

#include <string.h>
#include <stdlib.h> 
#include <stdio.h>
#include <assert.h> 

#include <yael/vector.h>
#include <yael/machinedeps.h>
#include <yael/gmm.h>

#define FMT_FVEC  0
#define FMT_TEXT  1

void usage(const char * cmd)
{
  printf ("Usage: %s  [options]\n", cmd);
  exit (0);
}

int main (int argc, char ** argv)
{
  int i;
  int k = 10;
  int d = 10;
  int n = 100;
  int niter = 40;
  int nredo = 1;
  int nt = count_cpu();
  int seed = 0;
  int ret;
  int fmt_in = FMT_FVEC;
  int fmt_out = FMT_FVEC;
  int compute_w = 0; /* Version with balanced.
  Use int compute_w = 1; if you want w to be computed */

  const char * fi_name = NULL;
  const char * fo_name = NULL;


  if (argc == 1)
    usage (argv[0]);

  for (i = 1 ; i < argc ; i++) {
    char *a = argv[i];

    if (!strcmp (a, "-h") || !strcmp (a, "--help"))
      usage (argv[0]);

    if (!strcmp (a, "-k") && i+1 < argc) {
      ret = sscanf (argv[++i], "%d", &k);
      assert (ret);
    }
    if (!strcmp (a, "-d") && i+1 < argc) {
      ret = sscanf (argv[++i], "%d", &d);
      assert (ret);
    }
    if (!strcmp (a, "-computew")) {
      compute_w = 1;
    }
    else if (!strcmp (a, "-n") && i+1 < argc) {
      ret = sscanf (argv[++i], "%d", &n);
      assert (ret);
    }
    else if (!strcmp (a, "-niter") && i+1 < argc) {
      ret = sscanf (argv[++i], "%d", &niter);
      assert (ret);
    }
    else if (!strcmp (a, "-nt") && i+1 < argc) {
      ret = sscanf (argv[++i], "%d", &nt);
      assert (ret);
    }
    else if (!strcmp (a, "-nredo") && i+1 < argc) {
      ret = sscanf (argv[++i], "%d", &nredo);
      assert (ret);
    }
    else if (!strcmp (a, "-seed") && i+1 < argc) {
      ret = sscanf (argv[++i], "%d", &seed);
      assert (ret);
    }
    else if (!strcmp (a, "-i") && i+1 < argc) {
      fi_name = argv[++i];
    }
    else if (!strcmp (a, "-itxt") && i+1 < argc) {
      fi_name = argv[++i];
      fmt_in = FMT_TEXT;
    }
    else if (!strcmp (a, "-o") && i+1 < argc) {
      fo_name = argv[++i];
    }
    else if (!strcmp (a, "-otxt") && i+1 < argc) {
      fo_name = argv[++i];
      fmt_out = FMT_TEXT;
    }
  }

  assert (fi_name && fo_name);

  fprintf (stderr, "k = %d\nd = %d\nn = %d\nniter = %d\nnredo = %d\n",
	   k, d, n, niter, nredo);
  fprintf (stderr, "nt = %d\nseed = %d\n", nt, seed);

  fprintf (stderr, "fi = %s  (fmt = %s)\n", fi_name, 
	   (fmt_in == FMT_FVEC ? "fvec" : "txt"));
  fprintf (stderr, "fo = %s  (fmt = %s)\n", fo_name, 
	   (fmt_out == FMT_TEXT ? "fvec" : "txt"));

  /* read the input vectors */
  
  /* read the vectors from the input file */
  float * v = fvec_new (n * d);
  if (fmt_in == FMT_FVEC)
    ret = fvecs_read (fi_name, d, n, v);
  else if (fmt_in == FMT_TEXT)
    ret = fvecs_read_txt (fi_name, d, n, v);
  else exit (1);
  assert (ret == n);

  /* Mixture evaluation */
  gmm_t * g = gmm_learn(d, n, k, niter, v, nt, seed, nredo, compute_w);

/*   /\* write the output file *\/ */
/*   if (fmt_out == FMT_FVEC) */
/*     ret = fvecs_write (fo_name, d, n, v); */
/*   else if (fmt_out == FMT_TEXT) */
/*     ret = fvecs_write_txt (fo_name, d, n, v); */
/*   else exit (2); */
/*   assert (ret == n); */

  gmm_delete(g);

  return 0;
}
