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


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <yael/spectral_clustering.h>
#include <yael/vector.h>

int main()
{
  int i, d = 2;

  int n1 = 20, n2 = 30, n = n1 + n2;
  //  int n1 = 5, n2 = 5, n = n1 + n2;
  double r1 = 1, r2 = 2;
  float * v = fvec_new (n * 2);

  for (i = 0 ; i < n1 ; i++) {
    v[i * d + 0] = r1 * cos (i * (double) 0.1);
    v[i * d + 1] = r1 * sin (i * (double) 0.1);
  }
  for (i = n1 ; i < n ; i++) {
    v[i * d + 0] = r2 * cos (i * (double) 0.1);
    v[i * d + 1] = r2 * sin (i * (double) 0.1);
  } 

  for (i = 0 ; i < n ; i++)
    fvec_print (v + i * d, d);

  int k = 2;
  int * assign = ivec_new (n);
  int * nassign = ivec_new (k);
  double sigma = 0.5;
  int niter = 40;
  int nt = 1;
  int seed = 0;
  int nredo = 4;
  spectral_clustering (d, n, k, sigma, niter, v, nt, seed, nredo, assign, nassign);

  printf ("nassign = "); ivec_print (nassign, k);
  printf ("assign = "); ivec_print (assign, n);


  free (v);
  return 0;
}
