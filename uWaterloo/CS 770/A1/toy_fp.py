#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""

Toy Floating-Point Number System

(c) Jeff Orchard, 2016
"""

import numpy as np

#import toy_fp_test

#import random


def Random(t, L=-100, U=100):
    '''
    b = toy_fp.Random(t, L=-100, U=100)
    
    Generate a random normalized binary floating-point number with
    t digits, and an exponent in the range [L,U]. For example,
    
      b = toy_fp.Random(5, -4, 4)
    
    might yield
    
      b = '-0.10111b-2'
    
    or
    
      b = '+0.11100b4'
    
    Note that the output is a string, and that the first character is
    always either a '+' or '-'. The number after the 'b' is
    the exponent for the base 2, although the exponent itself is
    represented in base-10. For example,
    
      b = '+0.11100b4'
    
    represents the number 0.11100 x 2^4.
    
    The default values for L and U are -100 and 100, respectively. They
    take their default values if they are not included in the call.
      eg.  b = toy_fp.Random(23)
    '''
    
    # === YOUR CODE BELOW ===
    p = np.random.randint(L,U+1) #or random.randint; doesn't seem to generate values with |p| >= 1000 often
    #d = random.getrandbits(t-1) #mantissa bits; generates a long, which may be at most 32 bits :(
    d = '0.1' #0 not included
    for i in range (2,t+1): #mantissa, range doesn't include the end t+1
        d = d+str(np.random.randint(0,2)) #d = d+str(random.getrandbits(1))
    s = np.random.randint(0,2) #s = random.getrandbits(1); sign bit
    if s==1:
        b = '+'
    else:
        b = '-'
    b = str(b) + str(d) + 'b' + str(p)
    return b
    
    
    

def fp2dec(b):
    '''
    x = toy_fp.fp2dec(b)
    
    Converts the string b to a decimal value (double-precision).
    Examples:
      toy_fp.fp2dec('+0.11000b1')  ->  1.5
      toy_fp.fp2dec('-0.10101b-2') -> -0.164062500000000
      toy_fp.fp2dec('-0.101b5')    -> -20
    '''

    # === YOUR CODE BELOW ===
    #print('fp is '+b)
    if len(b)<6: #debugging
        print('woops')
        return b
    if b[3]!='1': #must be normalized
        return 0    #could also be an invalid string
    #x = np.float64(0.)
    x = long(0)
    i = 3
    while b[i]!='b': 
    #    x = x + np.float(int(b[i]))*2**(2-i) #makes a lot of rounding errors?
        x = x + long(b[i])*2**(len(b)-i-3) #keep mantissa as an integer for now
        i=i+1
    i=i+1 #skip b
    p = int(b[i:])
    if b[0]=='-':
        x = (-1)*x
    #x = np.float64(np.float64(x)*2.**(p-len(b)+2+3))
    #print('mine is ')
    #print(x)
    #print('test is ')
    #print(toy_fp_test.fp2dec(b))
    #return x
    return np.float64(np.float64(x)*2.**(p-len(b)+2+3))


def fp2dec2(b): #alternate method, unused
    if len(b)<6: #debugging
        print('woops')
        return b
    if b[3]!='1':
        return 0    #could also be an invalid string
    #x = np.float64(0.)
    x = long(0)
    i = 3
    while b[i]!='b': 
        x = x + np.float64(int(b[i]))*2**(2-i) #makes a lot of rounding errors?
        #x = x + long(b[i])*2**(len(b)-i-3) #keep mantissa as an integer for now
        i=i+1
    i=i+1 #skip b
    p = int(b[i:])
    if b[0]=='-':
        x = (-1)*x
    return np.float64(np.float64(x)*2.**p)
    
    
    
def fp2dec3(b): #another alternate method, unused
    if len(b)<6: #debugging
        print('woops')
        return b
    if b[3]!='1':
        return 0    #could also be an invalid string
    #x = np.float64(0.)
    x = np.float64(0.0)
    i = 3
    while b[i]!='b': 
        i=i+1
    i=i+1 #skip b
    p = int(b[i:])
    i = 3
    while b[i]!='b': 
        x = x + np.float64(int(b[i]))*2.**(p+2-i)
        i=i+1
    if b[0]=='-':
        x = (-1)*x
    return np.float64(x)

    
    
def dec2fp(x_orig, t, L=-100, U=100):
    '''
    function b = toy_fp.dec2fp(x, t, L, U)
    
    Converts the number x to a binary floating-point representation,
    rounding to t digits, and L and U as the lower and upper bounds on
    the exponent.
    
    The default values for L and U are -100 and 100, respectively. They
    take their default values if they are not included in the call.
      eg.  b = toy_fp.dec2fp(3.14, 8)
    '''

    x = x_orig
    
    if x==0.:
        y = '+0.'
        for k in range(t):
            y = y + '0'
        y = y + 'b0'
        return y
    elif x==np.inf:
        y = 'Inf';
        return y
    elif x==-np.inf:
        y = '-Inf';
        return y
    elif np.isnan(x):
        y = 'NaN'
        return y
    
    if x<0:
        x = -x
        bx = '-0.'
    else:
        bx = '+0.'
    
    if x>=1:
        myexp = 0
        while x>=1:
            x = x / 2
            myexp = myexp + 1
    else:
        myexp = 1
        while x<1:
            x = x * 2
            myexp = myexp - 1
        x = x / 2
        
    remainder = x - np.floor(x)
    
    for d in range(t):
        remainder = remainder * 2.
        if remainder>=1.:
            bx = bx + '1'
            remainder = remainder - 1
        else:
            bx = bx + '0'
    
    # Round according to the last digit
    if remainder>=0.5:
        roundup = True
    else:
        roundup = False
    
    bx = bx + 'b' + str(myexp)
    
    if roundup:
        delta = bx[:3]
        delta = delta + '1b' + str(myexp-(t-1))
        #for k in range(t-1):
        #    delta = delta + '0'
        #delta = delta + '1' + bx[t+3:]
        bx_up = Math(bx, delta, (lambda z1,z2: z1+z2), t, L, U)
        bx = bx_up
    
    y = bx
    
    if myexp>U:
        y = 'overflow'
    elif myexp<L:
        y = '0.'
        for k in range(t):
            y = y + '0'
        y = y + 'b0'
    
    return y
    
    
    
        
    
def Math(b1, b2, fcn, t, L=-100, U=100):
    '''
    b = toy_fp.Math(b1, b2, fcn, t, L=-100, U=100)
     
    Performs a binary arithmetic operation.
    b1 and b2 are the input binary strings, from F(2, t, L, U)
    fcn is a function that takes 2 inputs, and returns a value.
    t, L, and U specify the FPNS.
     
    The output is a binary string for the nearest value in F(2, t, L, U).
     
    For example,
     
     toy_fp.Math('+0.11b2', '+0.1b1', (lambda z1,z2: z1-z2), 2, -5, 5)
     
    would perform subtraction, and return the string '+0.10b2'.
     
    The default values for L and U are -100 and 100, respectively. They
    take their default values if they are not included in the call.
      eg.  b = toy_fp.Math('+0.11b2', '+0.1b1', (lambda z1,z2: z1-z2), 2)
    '''
 
    x1 = fp2dec(b1)
    x2 = fp2dec(b2)
     
    y = fcn(x1, x2)
     
    b = dec2fp(y, t, L, U)
     
    return b



#def dd():
 #   s = '0'
  #  i=0
   # while True:
    #    i=i+1
     #   s = Random(23, -126, 127)
      #  fp2dec(s)
        #a = fp2dec(s)
        #b = toy_fp_test.fp2dec(s)
        #if i%1000 == 0:
         #   print(i)
          #  print(s)
           # print(a)
            #print(b)
#        if a!=b:
 #           break
  #  print(a)
   # print(b)
    #print(i)
    #return s

def add(x, y):
    return Math(x, y, (lambda z1,z2: z1+z2), 2, -126, 127)