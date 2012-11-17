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
#include <string.h>


#include <assert.h>
#include <math.h>
#include <sys/time.h>

#include <yael/matrix.h>
#include <yael/machinedeps.h>

#include "mex.h"


void mexFunction (int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray*prhs[])

{
  if(!((nrhs == 2 && (nlhs == 1 || nlhs == 2 || nlhs == 3)) || 
       (nrhs == 3 && nlhs == 2))) 
    mexErrMsgTxt("wrong number or inputs or outputs.");
  
  int d = mxGetM (prhs[0]);
  int n = mxGetN (prhs[0]);
  
  if(mxGetClassID(prhs[0]) != mxSINGLE_CLASS)
    mexErrMsgTxt("need single precision array.");

  int nev = (int) mxGetScalar (prhs[1]);


  if(nev > d || nev > n) 
    mexErrMsgTxt("too many singular values requested");
  
  float *u = NULL, *s = NULL, *v = NULL;  

  if(nlhs == 3) {    
    plhs[0] = mxCreateNumericMatrix (d, nev, mxSINGLE_CLASS, mxREAL);
    u = (float*)mxGetPr(plhs[0]);
    plhs[1] = mxCreateNumericMatrix (nev, 1, mxSINGLE_CLASS, mxREAL);
    s = (float*)mxGetPr(plhs[1]);
    plhs[2] = mxCreateNumericMatrix (n, nev, mxSINGLE_CLASS, mxREAL);
    v = (float*)mxGetPr(plhs[2]);
  } else if(nlhs == 2) {
    if(nrhs == 2) {
      plhs[0] = mxCreateNumericMatrix (d, nev, mxSINGLE_CLASS, mxREAL);
      u = (float*)mxGetPr(plhs[0]);
      plhs[1] = mxCreateNumericMatrix (nev, 1, mxSINGLE_CLASS, mxREAL);
      s = (float*)mxGetPr(plhs[1]);
    } else {
      plhs[0] = mxCreateNumericMatrix (nev, 1, mxSINGLE_CLASS, mxREAL);
      s = (float*)mxGetPr(plhs[0]);
      plhs[1] = mxCreateNumericMatrix (n, nev, mxSINGLE_CLASS, mxREAL);
      v = (float*)mxGetPr(plhs[1]);
    }
  } else if(nlhs == 1) {
    plhs[0] = mxCreateNumericMatrix (nev, 1, mxSINGLE_CLASS, mxREAL);
    s = (float*)mxGetPr(plhs[0]);    
  }

  int ret = fmat_svd_partial(d, n, nev, (float*)mxGetPr(prhs[0]), 
                             s, u, v); 
  
  if(ret <= 0) {
    mexErrMsgTxt("Did not find any singluar value.");    
  }

  if(ret != nev) {    
    fprintf(stderr, "WARN only %d / %d singular values converged\n", ret, nev); 
  }

}
