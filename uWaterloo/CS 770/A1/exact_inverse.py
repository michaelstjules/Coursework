
# Exact Inverse


#import numpy as np


# Use just one of the following two lines
import numpy as np
import toy_fp                 # your code
import toy_fp_test
#import toy_fp_test as toy_fp  # for comparision

# Here is some code showing you how to use toy_fp.
#b = toy_fp.Random(10, L=-5, U=5)   # generate a random number
#c = toy_fp.dec2fp(3.14, 10, L=-5, U=5)  # convert decimal # to FP #

#c2 = toy_fp.fp2dec(c)  # convert FP to decimal

#d = toy_fp.Math(b, c, (lambda z1,z2: z1/z2), 10)  # divide two FP numbers


def frac(N): #3.d, seems to give about 84-85% for large N
    #N is the number of samples
    n = 0
    for i in range (0,N):
        #print(i)
        x = toy_fp.Random(23, -126, 127)
        #y = toy_fp.Math('+0.1b1', x, (lambda z1,z2: z1/z2), 23, -126, 127)
        #print('x is ' + x)        
        #print y
        #print toy_fp.Math(x, y, (lambda z1,z2: z1*z2), 23, -126, 127)
        #print(toy_fp.fp2dec(toy_fp.Math(x, y, (lambda z1,z2: z1*z2), 23, -126, 127)))
        #problems with float and exact equality
        #if toy_fp.Math(x, y, (lambda z1,z2: z1*z2), 23, -126, 127)==toy_fp.dec2fp(1.0, 23):
        #    n+=1
        z = inv(x)
        #print('x*y is ' + z)
        #print toy_fp.dec2fp(1.0, 23, -126, 127)
        if z == toy_fp.dec2fp(1.0, 23, -126, 127):
            #print('inverse!')
            n=n+1
    return np.float64(n)/np.float64(N)

def inv(x): #returns x*1/x
    y = toy_fp.Math('+0.1b1', x, (lambda z1,z2: z1/z2), 23, -126, 127)
    #print('1/x is ' + y)
    #print(y)
    #print('x*y is')
    return toy_fp.Math(x, y, (lambda z1,z2: z1*z2), 23, -126, 127)
    
def invt(x):
    y = toy_fp_test.Math('+0.1b1', x, (lambda z1,z2: z1/z2), 23, -126, 127)
    #print('1/x is ' + y)
    #print(y)
    #print('x*y is')
    return toy_fp_test.Math(x, y, (lambda z1,z2: z1*z2), 23, -126, 127)

#def frac(N):
#    n = 0
#    for i in range (0,N):
#        x = toy_fp.fp2dec(toy_fp.Random(23, -126, 127))
#        y = toy_fp.fp2dec(toy_fp.dec2fp(1/x))
#        if x*y==1:
#            n+=1
#    return n/N


def dd(): #debugging
    s = toy_fp.Random(23, -126, 127)
    print s
    return toy_fp.dec2fp(toy_fp.fp2dec(s), 23)