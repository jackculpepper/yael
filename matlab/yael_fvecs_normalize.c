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
#include <yael/vector.h>

void mexFunction (int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray*prhs[])

{
  if (nrhs < 1 || nrhs > 4) 
    mexErrMsgTxt ("Invalid number of input arguments");
  
  if (nlhs < 1 || nlhs > 2)
    mexErrMsgTxt ("1 o 2 output arguments are required");

  if (mxGetClassID(prhs[0]) != mxSINGLE_CLASS)
    mexErrMsgTxt ("First parameter must be a single precision matrix"); 


  int d = mxGetM (prhs[0]);
  int n = mxGetN (prhs[0]);
  int norm = 2; 
  long i;

  if (nrhs >= 2)
    norm = mxGetScalar (prhs[1]);

  /* the set of vectors to be normalized */
  float * v = (float*) mxGetPr (prhs[0]);  

  plhs[0] = mxCreateNumericMatrix (d, n, mxSINGLE_CLASS, mxREAL);
  float * vo = (float*) mxGetPr (plhs[0]);
  
  /* norm should be double precision */
  plhs[1] = mxCreateNumericMatrix (n, 1, mxDOUBLE_CLASS, mxREAL);
  double * vnr = (double*) mxGetPr (plhs[1]);


  for (i = 0 ; i < n ; i++) {
    double nrtmp = fvec_norm (v + i * d, d, norm);
    vnr[i] = nrtmp;
    nrtmp = 1 / nrtmp;

    long j = 0;
    for (j = 0 ; j < d ; j++)
      vo[i*d+j] = v[i*d+j] * nrtmp;
  }
}
