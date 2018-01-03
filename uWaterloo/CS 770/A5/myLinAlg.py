# # myLinAlg


import numpy as np
import matplotlib.pylab as plt



def JacobiStep(A, u0, f):
    '''
    Usage: u = JacobiStep(A, u0, f)
    Performs one Jacobil iteration on
    A u = f using the guess u0.
    '''
    
    # *** YOUR CODE HERE ***
    
    #This seems very inefficient to recompute each time! 
    '''n = len(A)
    LpU = np.copy(A)
    D = np.zeros((n,n))
    for i in range (0,n):
        D[i,i] = A[i,i]
        LpU[i,i] = 0.
    LpU = -LpU
    u0 = np.linalg.solve(D, f+np.dot(LpU, u0))'''
   
    
    
    
    n = len(u0)
    u = np.zeros(n)
    for k in range (0,n):
        #u0k = f[k] - sum(A[k,0:k]*u0[0:k]) - sum(A[k,k+1:n]*u0[k+1:n])
        #u0[k] = (np.float(A[k,k])**-1.)*u0k
        u[k] = (np.float(A[k,k])**-1.)*(f[k] - sum(A[k,0:k]*u0[0:k]) - sum(A[k,k+1:n]*u0[k+1:n]))
    return u



def GSStep(A, u0, f):
    '''
    Usage: u = GSStep(A, u0, f)
    Performs one Gauss-Seidel iteration on
    A u = f using the guess u0.
    '''
    
    # *** YOUR CODE HERE ***
    
    #Note that forwardsub ignores the upper right entries (only L[k,j], j<=k are used, 
    #so A and D-L are treated the same way)
    #So I can just use A instead of D-L
    n = len(u0)
    for k in range (0,n):
        u0[k] = f[k] - sum(A[k,k+1:n]*u0[k+1:n]) 
    u0 = forwardsub(A,u0)
    
    return u0



def IterativeSolve(myStep, A, f, u0, maxIter, tol):
    '''
    Usage: u = IterativeSolve(myStep, A, f, u0, maxIter, tol)
    Iteratively solves A u = f, taking steps specified by the
    function myStep. The initial guess is u0. The matrix A must be
    NxN, and f must be an N-vector. The iteration stops
    either after maxIter iterations, or when the 2-norm of
    the residual is less than tol.
    '''
    
    # *** YOUR CODE HERE ***
    # You can call your myStep function like this...
    # u = myStep(A, u0, f)
    r = f - np.dot(A, u0)
    rnorms = [np.linalg.norm(r)]
    u = u0.copy() #since myStep modifies its input
    i = 0
    while rnorms[i] >= tol and i <= maxIter:
    #while np.linalg.norm(r) >= tol and i <= maxIter:
        u = myStep(A, u, f)
        r = f - np.dot(A, u)
        i = i +1
        rnorms.append(np.linalg.norm(r))
    plt.plot(range (0, len(rnorms)), np.log(rnorms), 'r.') #, ms=0.2
    #plt.ylim(0.,30.)
    plt.xlabel('i, iteration')
    plt.ylabel('log(||r^i||_2)') #, when <=30')
    plt.show()    
    return u




def backsub(U, b):
    '''
    x = backsub(U,b)
    Solves the system U*x = b, where U is an nxn upper-triangular matrix
    and b is an n-vector.
    '''
    N = np.shape(U)[0]
    x = b.copy()
    for k in (np.arange(N,0,-1)-1):
        for j in np.arange(k+1,N):
            x[k] = x[k] - U[k,j]*x[j]   # [1] for this
        x[k] = x[k] / U[k,k]     # [1] for the rest
    return x



def forwardsub(L, b):
    '''
    x = forwardsub(L,b)
    Solves the system L*x = b, where L is an nxn lower-triangular matrix
    and b is an n-vector.
    Note: L does NOT need to be unit-diagonal.
    '''
    N = np.shape(L)[0]
    x = b.copy()
    for k in np.arange(0,N):
        for j in np.arange(0,k):
            x[k] = x[k] - L[k,j]*x[j]   # [1] for this
        x[k] = x[k] / L[k,k]     # [1] for the rest
    return x