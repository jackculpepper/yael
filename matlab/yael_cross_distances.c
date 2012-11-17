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
  if (nrhs < 2 || nrhs > 4) 
    mexErrMsgTxt ("Invalid number of input arguments");
  
  if (nlhs != 1)
    mexErrMsgTxt ("1 output arguments required");

  int d = mxGetM (prhs[0]);
  int na = mxGetN (prhs[0]);
  int nb = mxGetN (prhs[1]);
  int distype = 2;
  int nt = 1;

  if (mxGetM (prhs[1]) != d)
      mexErrMsgTxt("Dimension of base and query vectors are not consistent");
  
  if (mxGetClassID(prhs[0]) != mxSINGLE_CLASS 
      || mxGetClassID(prhs[1]) != mxSINGLE_CLASS )
    mexErrMsgTxt ("need single precision array"); 

  if (nrhs >= 3)
    distype = (int) mxGetScalar(prhs[2]);

  if (nrhs >= 4 && distype != 2)
    nt = (int) mxGetScalar(prhs[3]);

  float *a = (float*) mxGetPr (prhs[0]);  
  float *b = (float*) mxGetPr (prhs[1]); 

  /* ouptut: centroids, assignment, distances */
  plhs[0] = mxCreateNumericMatrix (na, nb, mxSINGLE_CLASS, mxREAL);
  float *dis = (float*) mxGetPr (plhs[0]);

  if (distype == 2)
    compute_cross_distances (d, na, nb, a, b, dis);
  else 
    compute_cross_distances_alt_thread (distype, d, na, nb, a, b, dis, nt);
}
