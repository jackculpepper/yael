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


/* This file is a mex-matlab wrap for the nearest neighbor search function of yael */

#include <assert.h>
#include <math.h>
#include "mex.h"
#include <sys/time.h>


#include <yael/nn.h> 


void mexFunction (int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray*prhs[])

{
  if (nrhs < 2 || nrhs > 5) 
    mexErrMsgTxt ("Invalid number of input arguments");
  
  if (nlhs != 2)
    mexErrMsgTxt ("2 output arguments required");

  int d = mxGetM (prhs[0]);
  int n = mxGetN (prhs[0]);
  int nq = mxGetN (prhs[1]);
  int nt = 1;

  if (mxGetM (prhs[1]) != d)
      mexErrMsgTxt("Dimension of base and query vectors are not consistent");
  
  
  if (mxGetClassID(prhs[0]) != mxSINGLE_CLASS 
      || mxGetClassID(prhs[1]) != mxSINGLE_CLASS )
    mexErrMsgTxt ("need single precision array"); 


  float *b = (float*) mxGetPr (prhs[0]);  /* database vectors */
  float *v = (float*) mxGetPr (prhs[1]);  /* query vectors */
  int k = 1; 
  int distype = 2;

  if (nrhs >= 3)
    k = (int) mxGetScalar(prhs[2]);

  if (nrhs >= 4)
    distype = (int) mxGetScalar(prhs[3]);

  if (nrhs >= 5)
    nt = (int) mxGetScalar(prhs[4]); 

  if (n < k) 
    mexErrMsgTxt("fewer vectors than number to be returned");    


  /* ouptut: centroids, assignment, distances */

  plhs[0] = mxCreateNumericMatrix (k, nq, mxINT32_CLASS, mxREAL);
  int *assign = (int*) mxGetPr (plhs[0]);
  
  plhs[1] = mxCreateNumericMatrix (k, nq, mxSINGLE_CLASS, mxREAL);
  float *dis = (float*) mxGetPr (plhs[1]);

  /* With Matlab, we have to avoid using threads for the L2 distance, 
     because this one makes a call to MKL, which is no thread-safe */
  if (distype == 2 || distype == 16)
    knn_full (distype, nq, n, d, k, b, v, NULL, assign, dis);
  else
    knn_full_thread (distype, nq, n, d, k, b, v, NULL, assign, dis, nt);

  /* post-processing: convert to matlab indices, and enforce full sort */

  int i, j;

  for (i = 0 ; i < nq * k ; i++)
    assign[i]++;
}
