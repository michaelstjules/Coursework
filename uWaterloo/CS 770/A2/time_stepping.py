
# time_stepping.py

import numpy as np
import matplotlib.pylab as plt
plt.ion()


#%% Define the function ode12

def ode12(de_fcn, tspan, y0, tol=0.1, h0=1., maxh=0.2):
    '''
    y, t = ode12(de_fcn, tspan, y0, tol=0.1, h0=1., maxh=0.2)
    
    Solves a system of DEs (specified by the dynamics function de_fcn) from
    tspan[0] to tspan[1]. It performs adaptive time-stepping using
    a combination of Euler's method and Heun's method,
    keeping the local trucation error below tol. The initial state, y0,
    should be the same type of object (either scalar or numpy.array)
    that de_fcn outpus. The initial time step is h0, and no step should
    be larger than maxh.
    
    The inputs of the dynamics function, de_fcn, should be only t and y,
    where t is the independent variable, and y is the current state of
    the system.
      ie. dydt = de_fcn(t, y)
    
    The output is two numpy arrays, the second one holding an array of
    time stamps. The first array holds the corresponding solution.
    
    For example, if scalar_fcn returns a scalar, then
      y, t = ode12(scalar_fcn, [0., 3.], 0.5)
      plot(t, y)
    plots the solution over time.
      
    Or, if vec_fcn returns a 2-element array, and the initial state is
      y0 = np.array([0.1, 0.2])
    then
      y, t = ode12(vec_fcn, [0., 3.], y0)
    and
      plot(y[:,0], y[:,1])
    generates a phase plot.
    
    '''
    t_history = [np.float(tspan[0])]
    y_history = [y0]
    
    t = tspan[0]  # current time
    y = np.array([np.float(i) for i in y0])  # currect state
    
    yE = y
    yH = y
    
    l = 0. #estimated local error
    
    n = 0 #number of evaluations of de_fcn
    
    h = min([h0,maxh,tspan[1]-tspan[0]])
    # Loop until you reach the end time
    while t<tspan[1]: #and n < 100000:
        while True: #n < 100000:
            #calculate yE
            yE = E(de_fcn, t, y, h)
            #n = n+1
            #calculate yH
            yH = H(de_fcn, t, y, h)
            #n = n+2
            n = n+3
            l = np.linalg.norm(yE-yH,2) #Euclidean distance
            #print 'yE = ' + str(yE)
            #print 'yH = ' + str(yH)
            #print 'l = ' + str(l)
            if l > np.float(tol):
                #print 'tol = ' + str(tol)
                #print 'h = ' + str(h)
                h = min([0.8*h*((tol/l)**(1./3.)), tspan[1]-t])
                #print 'h = ' + str(h)
                #continue
            else:
                #print 'it worked!'
                break
        #update
        t = t + h  # placeholder code to avoid an infinite loop
        y = yH
        h = min([maxh,tspan[1]-t])
        # Here is how to construct the output
        #print 'append'
        y_history.append(y)
        t_history.append(t) 
        h = min([maxh, tspan[1]-t]) #next step will be largest possible
    return np.array(y_history), np.array(t_history), n



#%% Define the function ode23

def ode23(de_fcn, tspan, y0, tol=0.1, h0=1., maxh=0.2):
    '''
    y, t = ode23(de_fcn, tspan, y0, tol=0.1, h0=1., maxh=0.2)
    
    Solves a system of DEs (specified by the dynamics function de_fcn) from
    tspan[0] to tspan[1]. It performs adaptive time-stepping using
    a combination of Heun's method and a 3rd-order Runge-Kutta method,
    keeping the local trucation error below tol. The initial state, y0,
    should be the same type of object (either scalar or numpy.array)
    that de_fcn outpus. The initial time step is h0, and no step should
    be larger than maxh.
    
    The inputs of the dynamics function, de_fcn, should be only t and y,
    where t is the independent variable, and y is the current state of
    the system.
      ie. dydt = de_fcn(t, y)
    
    The output is two numpy arrays, the second one holding an array of
    time stamps. The first array holds the corresponding solution.
    
    For example, if scalar_fcn returns a scalar, then
      y, t = ode23(scalar_fcn, [0., 3.], 0.5)
      plot(t, y)
    plots the solution over time.
      
    Or, if vec_fcn returns a 2-element array, and the initial state is
      y0 = np.array([0.1, 0.2])
    then
      y, t = ode23(vec_fcn, [0., 3.], y0)
    and
      plot(y[:,0], y[:,1])
    generates a phase plot.
    
    '''
    t_history = [np.float(tspan[0])]
    y_history = [y0]
    
    t = tspan[0]  # current time
    y = np.array([np.float(i) for i in y0])  # currect state
    
    yH = y
    yRK3 = y
    
    l = 0. #estimated local error
    
    n = 0 #number of evaluations of de_fcn
    
    h = min([h0,maxh,tspan[1]-tspan[0]])
    # Loop until you reach the end time
    while t<tspan[1]: #and n < 100000:
        while True: #n < 100000:
            yH = H(de_fcn, t, y, h)
            #n = n+2
            yRK3 = RK3(de_fcn, t, y, h)
            #n = n+3
            n = n+5
            l = np.linalg.norm(yH-yRK3,2) #Euclidean distance
            #print 'yE = ' + str(yE)
            #print 'yH = ' + str(yH)
            #print 'l = ' + str(l)
            if l > np.float(tol):
                #print 'tol = ' + str(tol)
                #print 'h = ' + str(h)
                h = min([0.8*h*((tol/l)**(1./3.)), tspan[1]-t])
                #print 'h = ' + str(h)
                #continue
            else:
                #print 'it worked!'
                break
        #update
        t = t + h  # placeholder code to avoid an infinite loop
        y = yRK3
        h = min([maxh,tspan[1]-t])
        # Here is how to construct the output
        #print 'append'
        y_history.append(y)
        t_history.append(t) 
        h = min([maxh, tspan[1]-t]) #next step will be largest possible
    return np.array(y_history), np.array(t_history), n


#step methods

#Euler
def E(de_fcn, t, y, h):
    return y+h*de_fcn(t,y)

#Heun
def H(de_fcn, t, y, h):
    k1 = de_fcn(t,y)
    k2 = de_fcn(t+h, y+h*k1)
    return y+h*(k1+k2)/2.

#3 stage Runge-Kutta
def RK3(de_fcn, t, y, h):
    k03 = de_fcn(t, y)
    k13 = de_fcn(t+(1./3.)*h, y+(1./3.)*h*k03)
    k23 = de_fcn(t+(2./3.)*h, y+(2./3.)*h*k13)
    return y+h*((1./4.)*k03+(3./4.)*k23)

#%% Part (b): Define the dynamics function

def lv(t, z):
    '''
    dzdt = lw(t, z)
    
    Evaluates the right-hand side of the Lotka-Volterra system
    of ODEs. The input z should be a 2-element array, and the output
    is a 2-element numpy array.
    '''
    #if t>3 or t<0:
        #print('t outside [0,1]')
    # You can access x and y as z[0] and z[1], respectively.  
    
    #make sure everything stays nonnegative; results seem to explode otherwise
    #z0 = np.float((max(0,z[0]))
    #z1 = np.float(max(0,z[1]))
    
    xy = np.float(z[0])*np.float(z[1])
    return np.array([3.*np.float(z[0])-xy/100., -2.*np.float(z[1])+xy/100.])
    

#tests
    
    
def ex(t, z): #exponential, for test
    return np.array(np.float(z[0]))
#ex = time_stepping.ode12(time_stepping.ex, [0,3], [1])
#plt.plot(ex[1], ex[0], 'r.')
    
def c3(t, z): #linear with slope 3, for test
    return np.array(3.)
#c3 = time_stepping.ode12(time_stepping.c3, [0,3], [1])
#plt.plot(c3[1], c3[0], 'r.')
    
def t1(t, z): #quadratic
    return np.array(np.float(t))
#t1 = time_stepping.ode12(time_stepping.t1, [0,3], [1])
#plt.plot(t1[1], t1[0], 'r.')
    
def cpl(t, z):
    return np.array(3.+np.log(t+1))
#cpl = time_stepping.ode12(time_stepping.cpl, [0,3], [1])
#plt.plot(cpl[1], cpl[0], 'r.')
    
def cos(t, z): #sine
    return np.array(np.cos(t))
#cos = time_stepping.ode12(time_stepping.cos, [0,3], [1])
#plt.plot(cos[1], cos[0], 'r.')



#%% Part (c):

#Set up some parameters for the IVP

tspan = [0,3]
y0 = np.array([300,150])


#test plots
def plotlv():
    lv12 = ode12(lv, tspan, y0)
    lv23 = ode23(lv, tspan, y0)
    #plt.plot(lv12[0][0,:], lv12[0][1,:], 'r.', lv23[0][0,:], lv23[0][1,:], 'b.')
    plt.plot([i[0] for i in lv12[0]], [i[1] for i in lv12[0]], 'r.', [i[0] for i in lv23[0]], [i[1] for i in lv23[0]], 'b.')
    plt.xlabel('x')
    plt.ylabel('y')
    plt.show()
    return
    
#time_stepping.ode12(time_stepping.lv, [0,3], [300,150])
#plt.plot(lv12[1], [i[0] for i in lv12[0]], 'r.')
    

# Call ode12 and ode23 for the IVP
# ode calls here

lv12 = ode12(lv, tspan, y0)
lv23 = ode23(lv, tspan, y0)












# Plot the solution
plt.figure(1)
plt.clf()

# Plot commands here

#x and y vs t
plt.plot(lv12[1], [i[0] for i in lv12[0]], 'r.', lv23[1], [i[0] for i in lv23[0]], 'b.', lv12[1], [i[1] for i in lv12[0]], 'r|', lv23[1], [i[1] for i in lv23[0]], 'b|')
plt.xlabel('t')
plt.ylabel('x (points) and y (vertical lines)')
plt.show()


#y vs x
plt.plot([i[0] for i in lv12[0]], [i[1] for i in lv12[0]], 'r.', [i[0] for i in lv23[0]], [i[1] for i in lv23[0]], 'b.')
plt.xlabel('x')
plt.ylabel('y')
plt.show()




#%% Part (d):

len(lv12[0]) #number of time steps for ode12
len(lv23[0]) #number of time steps for ode23
lv12[2] #number of lv evaluations for ode12
lv12[2] #number of lv evaluations for ode23

# You can put some code here to help you compute stuff for part (d).
# By the way, the function "len" gives you the length of an array.


