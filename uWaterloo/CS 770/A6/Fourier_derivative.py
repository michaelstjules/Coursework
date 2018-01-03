
# Fourier_derivative.py

import numpy as np
import matplotlib.pyplot as plt
from numpy.fft import fft, ifft, fftshift, ifftshift
import scipy.io as sio


#%% Read in the Signal

blah = sio.loadmat('interesting_function.mat')
f = blah['f'].flatten()
x = blah['x'].flatten()

N = len(f)
L = x[-1] - x[0]  # The spatial extent of the signal



#Other test functions

#f = x #test the identity function

#f1 = x[0:N/2]
#f2 = f1[::-1]
#f = 1.5*np.append(f1, f2)



plt.figure(1)
plt.clf()
plt.plot(x, f)
plt.title('f(x)');


# *** YOUR CODE HERE ***



#0 should be at the centre




L = N*(x[-1]-x[0])/(N-1) #correction?


#doesn't the DFT cycle around (modular)?
#should I pad f with 0's outside of its domain?
#or treat f like it's periodic, and consider the Fourier transform from functions on the interval to functions on the integers
#Should I normalize by dividing by root(N)? Doesn't matter since applying inverse, anyway


#FIELD OF VIEW!




dx = L/N #x[1]-x[0] #assume evenly spaced samples
w = np.array([np.float(i)/L for i in range (-N/2, N/2)])

#w = np.roll(w, N/2)

dw = 1./L #w[1]-w[0]


#F = fft(dx*f) #multiply by dx to approximate the integral by a Riemann sum
F = fft(f)
F = fftshift(F)
G = 2.j*np.pi*w*F
G = ifftshift(G)
#df = ifft(dw*G) #multiply by dw to approximate the integral by a Riemann sum
df = ifft(G)

plt.figure(2)
plt.clf()
plt.plot(x, np.real(df)) #df should be real-valued
#plt.plot(x, abs(df)) #df should be real-valued
plt.title('df/dx (x)')

#This is definitely not the derivative. Takes nonreal complex values, too, but f is real. 