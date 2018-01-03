"""
solve_ls.py
"""

import numpy as np
import matplotlib.pyplot as plt
import unknown as uk


#%% Generate some sample x-values in the domain [0,1]
x = np.linspace(0., 1, 100)

# Plot the 4 different regressors
plt.figure(1)
plt.clf()
plt.plot(x, uk.Regressor0(x), label='R0')
plt.plot(x, uk.Regressor1(x), label='R1')
plt.plot(x, uk.Regressor2(x), label='R2')
plt.plot(x, uk.Regressor3(x), label='R3')
plt.legend(loc='upper center')
plt.xlabel('x')


#%% Create the data itself

x = np.random.rand(1000)

y = uk.BlackBox(x)

plt.figure(2)
plt.clf()
plt.plot(x, y, '.')


#%% Formulate and solve the LS problem

#====== TO DO ======
# You need to compute the linear coefficients, stored in c

c = [0., 0, 0, 0]  # These are just place-holder coefficients

# YOUR CODE HERE

#transpose of A
#AT = np.matrix([uk.Regressor0(x), uk.Regressor1(x), uk.Regressor2(x), uk.Regressor3(x)])
AT = np.array([uk.Regressor0(x), uk.Regressor1(x), uk.Regressor2(x), uk.Regressor3(x)])
A = AT.transpose()
ATA = np.dot(AT, A)
Q, R = np.linalg.qr(ATA)
#(Q*R)*c = (AT*A)*c = AT*y => R*c = QT*AT*y
QTATy = np.dot(Q.transpose(), np.dot(AT,y).transpose())

#backsubstitution
for i in range (3, -1, -1): #3,2,1,0, in that order
    z = QTATy[i]
    for j in range (i+1,4):
        z = z-R[i,j]*c[j]
    c[i] = (1./R[i,i])*z







#======


#%% Output

#====== TO DO ======
# Compute the model (the linear combination of the regressors)
# And plot the model line over the data

# YOUR CODE HERE



plt.figure(3)
plt.clf()
plt.title('My fit curve and BlackBox samples')
plt.plot(x, y, '.', label='BlackBox')
plt.plot(x, np.dot(A, c), '.', label='My fit curve')
plt.legend(loc='upper center')
plt.xlabel('x')

#======

# Finally, this prints the linear coefficients
print c