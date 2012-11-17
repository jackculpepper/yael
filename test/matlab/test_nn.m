% compute the nearest neighbors 
% If the mex-compiled version exists, it is automatically called. 
% Otherwise, the pure matlab implementation is used instead. 

addpath ('../../matlab')

n = 1000000;              % number of database vectors
nq = 1000;                % number of queries
d = 128;                  % dimension of the vectors
k = 1;                    % number of nearest neighbors to retrieve
distype = 2;              % number of processor cores to be used
v = single (rand(d,n));   % base vectors
q = single (rand(d,nq));  % query vectors

tic
[idx, dis] = yael_nn (v, q, k, distype);
toc
