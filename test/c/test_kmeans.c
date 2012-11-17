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

#include <yael/vector.h>
#include <yael/kmeans.h>
#include <yael/machinedeps.h>

int main (int argc, char ** argv)
{
  int k = 50;                           /* number of cluster to create */
  int d = 20;                           /* dimensionality of the vectors */
  int n = 1000;                         /* number of vectors */
  int nt = 2;                           /* number of threads to use */
  int niter = 0;                        /* number of iterations (0 for convergence)*/
  int redo = 1;                         /* number of redo */

  float * v = fvec_new_rand (d * n);    /* random set of vectors */

  /* variables are allocated externaly */
  float * centroids = fvec_new (d * k); /* output: centroids */
  float * dis = fvec_new (n);           /* point-to-cluster distance */
  int * assign = ivec_new (n);          /* quantization index of each point */
  int * nassign = ivec_new (k);         /* output: number of vectors assigned to each centroid */

  double t1 = getmillisecs();
  kmeans (d, n, k, niter, v, 1, 1, redo, centroids, dis, assign, nassign);
  double t2 = getmillisecs();

  printf ("kmeans performed in %.3fs\n", (t2 - t1)  / 1000);
  
  ivec_print (nassign, k);

  return 0;
}
