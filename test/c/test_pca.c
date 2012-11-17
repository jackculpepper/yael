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


#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <yael/vector.h>
#include <yael/matrix.h>


/* 
Copy/paste the points matrix into octave and compare:

[d,n]=size(centered_v)

cov=centered_v * centered_v';

[cov_eigenvectors,cov_eigenvalues]=eig(cov);

cov_eigenvalues=diag(cov_eigenvalues);

[sorted,perm]=sort(cov_eigenvalues)
cov_eigenvectors(:,perm(end:-1:1))

Lines should be the same up to the sign as eigs_f output


*/


void test_std (long n, long d, float *v) 
{
  fmat_center_columns(d,n,v);

  printf("\ncentered_v=");
  fmat_print(v,d,n);

  float *eig_f=fmat_new_pca(d,n,v,NULL);  
  
  printf("\neig_f=");
  fmat_print(eig_f,d,d);
  free(eig_f);
}



#define PCA_BLOCK_SIZE 256



pca_online_t * pca_online (long n, int d, const float *v)
{
  long i;

  pca_online_t * pca = pca_online_new (d);

  for (i = 0 ; i < n ; i += PCA_BLOCK_SIZE) {
    long iend = i + PCA_BLOCK_SIZE;
    if (iend > n) iend = n;
    long ntmp = iend - i;
    const float * vb = v + i * d;

    pca_online_accu (pca, vb, ntmp);
  }

  /* compute the PCA decomposition itself */
  pca_online_complete (pca);

  return pca;
}



/* Apply the matrix multiplication by block */
void apply_pca (const struct pca_online_s * pca, float * v, float * vo, int d, int n, int dout)
{
  long i;

  for (i = 0 ; i < n ; i += PCA_BLOCK_SIZE) {
    long iend = i + PCA_BLOCK_SIZE;
    if (iend > n) iend = n;
    long ntmp = iend - i;

    pca_online_project (pca, v + i * d, vo + i * dout, d, ntmp, dout);
  }  

  printf("\ncentered_v=");
  fmat_center_columns(d,n,v);
  fmat_print(v,d,n);

  double energy_in = fvec_sum_sqr (v, n * d);
  double energy_out = fvec_sum_sqr (vo, n * dout);
  printf ("Energy preserved = %.3f\n", (float) (energy_out / energy_in));
}



int main (int argc, char **argv)
{
  int d, n;

  if(argc!=3 || sscanf(argv[1],"%d",&n)!=1 || sscanf(argv[2],"%d",&d)!=1) {
    fprintf(stderr,"usage: test_pca npt ndim\n");
    return 1;
  }

  long i;
  float *v = fvec_new(n*d);
  for (i=0;i<n*d;i++) 
    v[i]=drand48()*2-1;
  float * v1 = fvec_new_cpy (v, n*d);

  /* reference version */
  test_std(n, d, v1);

  /* version with on-line reading of vectors */
  pca_online_t * pca = pca_online(n, d, v);
  
  printf("\ncov2=");
  fmat_print(pca->cov,d,d);

  printf("\neigvec=");
  fmat_print(pca->eigvec,d,d);


  /* Project the vector using the PCA matrix */
  int dout = d;
  float * vo = fvec_new (n*d);
  apply_pca (pca, v, vo, d, n, dout);

  free(v);

  return 0;
}

