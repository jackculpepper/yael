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
#include <assert.h>

#include <yael/eigs.h>
#include <yael/vector.h>
#include <yael/matrix.h>

int main()
{
  int i, j, k, d = 10,d2=5;
  float * a = fvec_new (d * d);
  float * b = fvec_new (d * d);
  float * b0 = fvec_new (d * d);
#define B0(i,j) b0[(i)+(j)*d]
#define A(i,j) a[(i)+(j)*d]
#define B(i,j) b[(i)+(j)*d] 
  float * lambda = fvec_new (d);
  float * v = fvec_new (d * d);
  float *v_part=fvec_new (d * d2);

  for (i = 0 ; i < d ; i++)
    for (j = 0 ; j  <= i ; j++) {
      A(i,j) = A(j,i) = drand48(); 
      B0(i,j)=drand48();
      B0(j,i)=drand48();
      
/*      B(i,j) = B(j,i) = drand48(); */
    }
  /* make a positive definite b (with b=b0*b0') */
  for (i = 0 ; i < d ; i++)
    for (j = 0 ; j  < d ; j++) {
      double accu=0;
      for(k=0;k<d;k++) 
        accu+=B0(i,k)*B0(j,k);
      B(i,j)=accu;
    }

  printf ("a = ");
  fmat_print(a,d,d);

  printf ("\nb = "); 
  fmat_print(b,d,d);

  printf ("Solution of the eigenproblem Av=lambda v\n");
  
  printf ("\n");
  int ret=eigs_sym (d, a, lambda, v);
  assert(ret==0);
  printf ("\n");


  printf("Eigenvectors:\n");
  fmat_print(v,d,d);

  fprintf(stdout, "lambda = ");
  fvec_print (lambda, d);
  printf ("\n");

  printf("Partial eigenvalues/vectors:\n");

  printf ("\n");
  ret=eigs_sym_part (d, a, d2, lambda, v_part);
  assert(ret>0);
  if(ret<d2) 
    printf("!!! only %d / %d eigenvalues converged\n",ret,d2);

  printf ("\n");

  printf("Eigenvectors:\n");
  fmat_print(v_part,d,d2);
  
  fprintf(stdout, "lambda = ");
  fvec_print (lambda, d2);
  printf ("\n");

   
  printf ("Solution of the generalized eigenproblem Av=lambda B v\n");

  printf ("\n");
  ret=geigs_sym (d, a, b, lambda, v);
  assert(ret==0);
  printf ("\n");

  fmat_print(v,d,d);

  fprintf(stdout, "lambda = ");
  fvec_print (lambda, d);
  printf ("\n");

  free (a);
  free (lambda);
  free (v);

  return 0;
}
