# myLinAlg

import numpy as np


def myQR(Aorig):
    '''
    Q, R = myQR(A)
    Performs QR factorization of input matrix A, where
    A is mxn
    Q is mxm and orthogonal
    R is mxn and upper-triangular
    '''

    R = Aorig.copy()
    M, N = np.shape(R)    
    Q = np.eye(M,M) #identity matrix

    for k in range (0,min(N,M)): #if k>M, the next line doesn't make sense
        x = R[k:M, k] #length M-1-k+1 = M-k; K:M = (k, k+1, ..., M-1)
        u = np.sign(x[0])*np.linalg.norm(x)*e0(M-k)+x
        u = (1./np.linalg.norm(u))*u
        F = np.eye(M-k,M-k)-2.*np.dot(np.matrix(u).transpose(), np.matrix(u))
        Qkl = np.concatenate((np.eye(k,k), np.zeros((M-k, k)))) #left side of Q_k
        Qkr = np.concatenate((np.zeros((k,M-k)),F)) #right side of Q_k
        Qk = np.concatenate((Qkl, Qkr), 1)
        Q = np.dot(Q, Qk)
        for j in range (k, N):
            #does this code work?
            R[k:M,j] = np.dot(F, R[k:M,j])
    return Q, R


def e0(n): #return the 1st (0th, with 0-indexing) standard vector in R^n
    e = [0. for i in range (0, n)]
    e[0] = 1.
    return np.array(e)










    '''#do the first step (k=0) separately, because Python doesn't handle empty matrices well
    x = R[0:M, 0] #length M-1-k+1 = M-k; K:M = (k, k+1, ..., M-1)
    u = np.sign(x[0])*np.linalg.norm(x)*e0(M)+x
    u = (1./np.linalg.norm(u))*u
    F = np.eye(M,M)-2.*np.dot(np.matrix(u).transpose(), np.matrix(u))
    Q = F
    for j in range (0, N):
        #does this code work?
        R[0:M,j] = np.dot(F, R[0:M,j])'''
    
    