import numpy as np
import myLinAlg
import matplotlib.pylab as plt



#%% 4.(a)

f = 10.*(2.*np.random.rand(30)-1.)
#v0 = np.zeros(30)
v0 = 10.*2.*np.random.rand(30)-1.
A = 10.*2.*np.random.rand(30,30)-1.
for i in range (0,30):
    #A[i,i] = sum(A[i,:])
    A[i,i] = np.sign(A[i,i])*np.linalg.norm(A[i,:],1)
# will be strictly diagonally dominant unless A[i,i] = 0 (or < machine epsilon times the rest of the sum) initially
#which is unlikely, but still diagonally dominant if A[i,i] != 0
  

#test that it's strictly diagonally dominant  
#for i in range (0,30):
#    print abs(A[i,i]) > np.linalg.norm(A[i,:],1)-abs(A[i,i])

#If using nonnegative entries only
#can add a positive multiple of the identity to fix this
#A = A + np.random.rand(1)*np.eye(30)
#np.random.rand(1) might return a value too close to 0

maxIter = 1000
tol = 10.**-9
print('maxIter = ' + np.str(maxIter))
print('tol = ' + np.str(tol))

#print f

#converges
plt.title('Jacobi on SDD matrices: logarithm of the residual norm vs iteration')
vJ = myLinAlg.IterativeSolve(myLinAlg.JacobiStep, A, f, v0, maxIter, tol)
fJ = np.dot(A,vJ)
#print fJ
print('The norm of the last residual is '+np.str(np.linalg.norm(fJ-f)))

#converges
plt.title('Gauss-Seidel on SDD matrices: logarithm of the residual norm vs iteration')
vGS = myLinAlg.IterativeSolve(myLinAlg.GSStep, A, f, v0, maxIter, tol)
fGS = np.dot(A,vGS)
#print fGS
print('The norm of the last residual is '+np.str(np.linalg.norm(fGS-f)))




#%%  4.(b) 

#Generate SPD but not necessarily strictly diagonally dominant matrices

f = 2.*np.random.rand(30)-1.
#v0 = np.zeros(30)
v0 = 2.*np.random.rand(30)-1.
A = np.random.rand(30,30)
A = np.dot(A.transpose(),A) #positive semidefinite, but not necessarily strictly so
#the probability that matrix is not SPD (strict) is the probability that its rows are 
#linearly dependent, which should be very low, since they are random (but not unformly random)

#A==A.transpose()
#np.linalg.eigvals(A)


#since A's eigenvalues are all positive A-cI will be invertible for all c<0
#so adding a random positive multiple of the identity will keep A positive semidefinite, 
#but further make it positive semidefinite (ignoring rounding the case when c=0 or c is too close to 0)
#A = A + np.random.rand(1)*np.eye(30)

maxIter = 10000
tol = 10.**-3
print('maxIter = ' + np.str(maxIter))
print('tol = ' + np.str(tol))

#print f

#diverges
plt.title('Jacobi on SPD matrices: logarithm of the residual norm vs iteration')
vJ = myLinAlg.IterativeSolve(myLinAlg.JacobiStep, A, f, v0, maxIter, tol)
fJ = np.dot(A,vJ)
#print fJ
print('The norm of the last residual is '+np.str(np.linalg.norm(fJ-f)))

#converges, but slowly
plt.title('Gauss-Seidel on SPD matrices: logarithm of the residual norm vs iteration')
vGS = myLinAlg.IterativeSolve(myLinAlg.GSStep, A, f, v0, maxIter, tol)
fGS = np.dot(A,vGS)
#print fGS
print('The norm of the last residual is '+np.str(np.linalg.norm(fGS-f)))


















#%%  Try 4.(b) with normal distribution

'''#Generate SPD but not necessarily strictly diagonally dominant matrices

f = np.random.normal(0.,100.,(30))
#v0 = np.zeros(30)
v0 = np.random.normal(0.,100.,(30))
A = np.random.normal(0.,10.,(30,30))
A = np.dot(A.transpose(),A) #positive semidefinite, but not necessarily strictly so
#the probability that matrix is not SPD (strict) is the probability that its rows are 
#linearly dependent, which should be very low, since they are random (but not unformly random)

#A==A.transpose()
#np.linalg.eigvals(A)


#since A's eigenvalues are all positive A-cI will be invertible for all c<0
#so adding a random positive multiple of the identity will keep A positive semidefinite, 
#but further make it positive semidefinite (ignoring rounding the case when c=0 or c is too close to 0)
#A = A + np.random.rand(1)*np.eye(30)

maxIter = 10000
tol = 10.**-3


print f

#should diverge?
vJ = myLinAlg.IterativeSolve(myLinAlg.JacobiStep, A, f, v0, maxIter, tol)
fJ = np.dot(A,vJ)
print fJ
print np.linalg.norm(fJ-f)

#should converge?
vGS = myLinAlg.IterativeSolve(myLinAlg.GSStep, A, f, v0, maxIter, tol)
fGS = np.dot(A,vGS)
print fGS
print np.linalg.norm(fGS-f)'''