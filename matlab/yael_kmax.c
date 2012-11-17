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


#include "mex.h"
#include <yael/sorting.h>

void mexFunction (int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray*prhs[])

{
  if (nrhs != 2) 
    mexErrMsgTxt ("Invalid number of input arguments");
  
  if (nlhs > 2)
    mexErrMsgTxt ("1 o 2 output arguments are required");

  if (mxGetClassID(prhs[0]) != mxSINGLE_CLASS)
    mexErrMsgTxt ("First parameter must be a single precision matrix"); 


  int d = mxGetM (prhs[0]);
  int n = mxGetN (prhs[0]);
  int k = mxGetScalar (prhs[1]);
  int tr = 0;   /* transpose or not in case of only one vector */

  if (mxGetClassID(prhs[0]) != mxSINGLE_CLASS)
    mexErrMsgTxt ("need single precision array"); 
  float * v = (float*) mxGetPr (prhs[0]);


  if (d == 1 && n > 1) {
    d = n;
    n = 1;
    tr = 1;
  }

  if (k > d)
    mexErrMsgTxt ("k should be smaller than the number of vectors"); 

  if (tr == 0) {
    plhs[0] = mxCreateNumericMatrix (k, n, mxSINGLE_CLASS, mxREAL);
    plhs[1] = mxCreateNumericMatrix (k, n, mxINT32_CLASS, mxREAL);
  }
  else {
    plhs[0] = mxCreateNumericMatrix (1, k, mxSINGLE_CLASS, mxREAL);
    plhs[1] = mxCreateNumericMatrix (1, k, mxINT32_CLASS, mxREAL);
  }

  float * val = (float*) mxGetPr (plhs[0]);
  int * idx = (int*) mxGetPr (plhs[1]);

  int i, j;
  for (j = 0 ; j < n ; j++) {
    fvec_k_max (v, d, idx, k);
    for (i = 0 ; i < k ; i++) {
      val[i] = v[idx[i]];
      idx[i]++; /* idx start from 1 in matlab */
    }
    idx += k;
    val += k;
    v += d;
  }
} 
