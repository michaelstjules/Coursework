
# find_shift.py

import numpy as np
import scipy.io as sio
import matplotlib.pyplot as plt

#%% Choose a data file to read in the signals

if True:
    # Load shorter signals from a file
    sigs = sio.loadmat('signals1.mat')
    f = sigs['s0'].flatten()
    g = sigs['s1'].flatten()
else:
    # Load longer signals from a file
    sigs = sio.loadmat('signals2.mat')
    f = sigs['s2'].flatten()
    g = sigs['s3'].flatten()


#%% Plot the signals

plt.figure(1)
plt.clf()
plt.plot(f, label='f')
plt.plot(g, label='g')
plt.legend()


#%% Define the FindShift function

def FindShift(f, g):
    '''
    opt_shift = FindShift(f, g)

    Finds the shift to apply to f to align it with g.
    f and g must be 1-D signals of the same length.
    opt_shift is the amount to shift f to the right to align it with g.

    ie. np.roll(f, opt_shift) should align f with g
    '''

    opt_shift = 0

    # *** YOUR CODE HERE ***
    F = np.fft.fft(f)
    G = np.fft.fft(g)
    Fmax = max(abs(F))
    GdF = [0. for k in range (0, len(F))] #will be G_k/F_k, but 0 when G_k or F_k is small in magnitude
    for k in range (0, len(F)):
        if abs(F[k])/Fmax >= 0.25 and abs(G[k])/Fmax >= 0.25: 
            GdF[k] = G[k]/F[k]
    pc = np.fft.ifft(GdF) #phase correlation
    #or use the real part of it; the imaginary part is close to 0
    #plt.clf()
    #plt.plot(range (0, len(f)), pc)
    #plt.show()
    opt_shift = np.argmax(abs(pc))
    return opt_shift

#%% Find  optimal shift
opt_shift = FindShift(f, g)

#%% Apply the optimal shift to f to see if it's aligned with g
f_shifted = np.roll(f, opt_shift)

plt.figure(2)
plt.clf()
plt.plot(f_shifted, label='Shifted f')
plt.plot(g, label='g')
plt.title('Optimal shift = ' + str(opt_shift))









#%% test with f and known exact shifts of f
for i in range (0, len(f)):
    fi = np.roll(f,i)
    j = FindShift(f,fi)
    #print i, j
    if not(j==i):
        print i, j


#%% test with norm of difference instead

def FindShift2(f,g):
    m = np.float('inf')
    m_ind = 0
    for i in range (0, len(f)):
        n = np.linalg.norm(np.roll(f, i)-g)
        if n < m:
            m = n
            m_ind = i
    return m_ind
    
opt_shift = FindShift2(f, g)


f_shifted = np.roll(f, opt_shift)

plt.figure(3)
plt.clf()
plt.plot(f_shifted, label='Shifted f')
plt.plot(g, label='g')
plt.title('Optimal shift = ' + str(opt_shift))

for i in range (0, len(f)):
    fi = np.roll(f,i)
    j = FindShift2(f,fi)
    if not(j==i):
        print i, j


#%% Old code

'''
def FindShift(f, g):

    opt_shift = 0

    # *** YOUR CODE HERE ***
    F = np.fft.fft(f)
    G = np.fft.fft(g)
    Fmax = max(abs(F))
    GdF = [] #G_k/F_k, where G_k and F_k aren't too small
    GdF_indices = [] #the corresponding indices
    for k in range (0, len(F)):
        if abs(F[k])/Fmax >= 0.25 and abs(G[k])/Fmax >= 0.25: 
            GdF.append(G[k]/F[k])
            GdF_indices.append(k)
        else:
            GdF.append(0.)
            GdF_indices.append(k)
    #GdF = G/F
    i = np.argmax(abs(np.fft.ifft(GdF)))
    opt_shift = GdF_indices[i]
    opt_shift = np.argmax(abs(np.fft.ifft(GdF)))
    return opt_shift
'''