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

#include <yael/gmm.h>
#include <yael/machinedeps.h>

#include "mex.h"


void mexFunction (int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray*prhs[])

{
  if (nrhs < 2 || nrhs % 2 != 0) 
    mexErrMsgTxt("even nb of input arguments required.");
  else if (nlhs != 3) 
    mexErrMsgTxt("3 output arguments are required for a GMM: gaussian weights, means and variance.");

  int d = mxGetM (prhs[0]);
  int n = mxGetN (prhs[0]);
  long seed = 0L;
  int flags = GMM_FLAGS_MU;
  
  if(mxGetClassID(prhs[0])!=mxSINGLE_CLASS)
    mexErrMsgTxt("need single precision array.");

  float *v = (float*) mxGetPr (prhs[0]);
  int k = (int) mxGetScalar (prhs[1]);

  int niter = 50, redo = 1, nt = 1, verbose = 1;

  {
    int i;
    for(i = 2 ; i < nrhs ; i += 2) {
      char varname[256];
      if (mxGetClassID(prhs[i]) != mxCHAR_CLASS) 
        mexErrMsgTxt ("variable name required");         

      if (mxGetString (prhs[i], varname, 256) != 0)
        mexErrMsgTxt ("Could not convert string data");

      if (!strcmp(varname, "niter")) 
        niter = (int) mxGetScalar (prhs[i+1]);

      else if (!strcmp(varname, "nt"))  
	/* !!! Normally, use nt=1 for multi-threading in Matlab: 
	   Blas is already multi-threaded. 
	   Explicit call with nt>1 may cause memory leaks for some Matlab version */
	nt = (int) mxGetScalar (prhs[i+1]); 
      
      else if (!strcmp(varname,"redo")) 
        redo = (int) mxGetScalar (prhs[i+1]);

      else if (!strcmp(varname,"seed")) 
        seed = (int) mxGetScalar (prhs[i+1]);

      else if (!strcmp(varname,"verbose")) 
        verbose = (int) mxGetScalar (prhs[i+1]);

      else 
        mexErrMsgTxt("unknown variable name");  
    }
  }
  
  /* default: use 1 processor core */
  if (nt == 0)
    nt = 1;

  if (verbose > 0)
    printf("Input: %d vectors of dimension %d\nk=%d niter=%d nt=%d "
	   "redo=%d verbose=%d seed=%d v1=[%g %g ...], v2=[%g %g... ]\n",
	   n, d, k, niter, nt, redo, verbose, seed, v[0], v[1], v[d], v[d+1]); 

  if(n < k) {
    mexErrMsgTxt("fewer points than gaussian in the mixture");
  }


  /* ouptut: GMM, i.e., weights, mu and variances */
  gmm_t * g = gmm_learn (d, n, k, niter, v, nt, seed, redo, flags); 

  plhs[0] = mxCreateNumericMatrix (1, k, mxSINGLE_CLASS, mxREAL);
  plhs[1] = mxCreateNumericMatrix (d, k, mxSINGLE_CLASS, mxREAL);
  plhs[2] = mxCreateNumericMatrix (d, k, mxSINGLE_CLASS, mxREAL);

  fvec_cpy ((float*) mxGetPr(plhs[0]), g->w, k);      /* w     */
  fvec_cpy ((float*) mxGetPr(plhs[1]), g->mu, d * k);  /* mu    */
  fvec_cpy ((float*) mxGetPr(plhs[2]), g->sigma, d * k);  /* sigma */
  
  gmm_delete (g); 
}
