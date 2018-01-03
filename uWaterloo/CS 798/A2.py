import numpy as np
import matplotlib.lines as mlines
import matplotlib.pyplot as plt
import time
#time.process_time() # only available in Python version 3.3 or later
#time.clock() #measures process time in UNIX, so I'll use that

#%% 1.2

def Mh(eta, z): #Moreau envelope of the hinge loss
    if z >= 1.:
        return 0.
    elif z <= 1. - eta:
        return 1.-z-(1./2.)*eta
    return (1./(2.*eta))*((1.-z)**2.) 
    #should I worry about when eta gets rounded to 0? No problems so far

def h(t): #hinge loss
    return max(1.-t, 0.)
    #if t > 1:
     #   return 0
    #return 1.-t

def Mh_plot():
    plt.xlabel('z')
    plt.ylabel('y')
    z = np.arange(-0.5, 1.5, 0.001)
    plt.plot(z, [Mh(4, t) for t in z], 'y.', z, [Mh(2, t) for t in z], 'r.', z, [Mh(1, t) for t in z], 'm.', z, [Mh(1./2., t) for t in z], 'b.', z, [Mh(1./4., t) for t in z], 'c.', z, [Mh(1./8., t) for t in z], 'g.', z, [h(t) for t in z], 'k.', ms=2)
    #legend
    ol = mlines.Line2D([], [], color='orange', label='M_h^eta(z), eta = 4')
    rl = mlines.Line2D([], [], color='red', label='M_h^eta(z), eta = 2')
    ml = mlines.Line2D([], [], color='magenta', label='M_h^eta(z), eta = 1')
    bl = mlines.Line2D([], [], color='blue', label='M_h^eta(z), eta = 1/2')
    cl = mlines.Line2D([], [], color='cyan', label='M_h^eta(z), eta = 1/4')
    gl = mlines.Line2D([], [], color='green', label='M_h^eta(z), eta = 1/8')
    kl = mlines.Line2D([], [], color='black', label='h(z) = max{1-z, 0}')
    plt.legend(handles=[ol, rl, ml, bl, cl, gl, kl])
    plt.suptitle('Proximal maps of the hinge loss, h')
    plt.show()








#%% 2

#Global variables
d = 20
#d = np.random.randint(1, 100)
#n = np.random.randint(1, 1000)
n = 500
x = np.random.normal(0.0, 1.0, (n, d)) #matrix of the x_i
y = (-1)**np.random.binomial(1, 0.5, n) #convert to floats???
lmbd = 1. #lambda, tuning parameter for regularization


#possible sequences for eta
def ttop(t, p=-1): #(t+1)^p
    return np.float(t+1)**np.float(p)
    
def logttop(t, p=-1): #(log(t+2))^p
    return np.log2(np.float(t+2))**np.float(p)
    



#%% 2.1

#(proximal) subgradient algorithm
def subgrad(dl, Pg, w0 = np.random.normal(0.0, 1.0, d), eta = ttop, T=1000): #should I use a tolerance? gap between f(x_{t+1}) and f(x_t) or x_{t+1} and x_t, e.g. eps=10.**-5
    #dl is the subgradient of l, Pg is the proximal map of g, f is thex0 is the initial point
    w = w0
    for t in range (0,T):
        dt = dl(w)
        dt = (1./np.linalg.norm(dt))*dt #optional normalization
        et = eta(t) #eta(t, w, dt, f, dl, Pg)
        w = Pg(et, w-et*dt)
    #return w, f(w)
    return w
    
    
#(proximal) subgradient algorithm for testing
def subgrad_test(f, dl, Pg, w0 = np.random.normal(0.0, 1.0, d), eta = ttop, T=1000): 
    w = w0
    fvals = [0. for i in range (0,T+1)]
    times = [0. for i in range (0,T+1)]
    fvals[0] = f(w)
    times[0] = 0.
    for t in range (0,T):
        time0 = time.clock()
        dt = dl(w)
        dt = (1./np.linalg.norm(dt))*dt #optional normalization
        et = eta(t) #eta(t, w, dt, f, dl, Pg)
        w = Pg(et, w-et*dt)
        time1 = time.clock()
        fvals[t+1] = f(w)
        times[t+1] = time1-time0
    plt.plot(range (0,len(fvals)), fvals, 'r.', ms=2)
    plt.suptitle('Subgradient algorithm objective function value vs iteration')
    plt.xlabel('iteration, t')
    plt.ylabel('f(x_t)')
    plt.show()
    plt.plot(range (0,len(times)), times, 'r.', ms=2)
    plt.suptitle('Subgradient algorithm time vs iteration')
    plt.xlabel('iteration, t')
    plt.ylabel('processing time, in seconds')
    plt.show()
    return w, fvals[len(fvals)-1]
    

#unnormalized subgradient; eta = 1, no proximal map
def subgrad_test2(f, df, w0 = np.random.normal(0.0, 1.0, d), T=1000): 
    w = w0
    fvals = [0. for i in range (0,T+1)]
    times = [0. for i in range (0,T+1)]
    fvals[0] = f(w)
    times[0] = 0.
    for t in range (0,T):
        time0 = time.clock()
        dt = df(w)
        w = w-dt
        time1 = time.clock()
        fvals[t+1] = f(w)
        times[t+1] = time1-time0
    plt.plot(range (0,len(fvals)), fvals, 'r.', ms=2)
    plt.suptitle('Subgradient algorithm objective function value vs iteration')
    plt.xlabel('iteration, t')
    plt.ylabel('f(x_t)')
    plt.show()
    plt.plot(range (0,len(times)), times, 'r.', ms=2)
    plt.suptitle('Subgradient algorithm time vs iteration')
    plt.xlabel('iteration, t')
    plt.ylabel('processing time, in seconds')
    plt.show()
    return w, fvals[len(fvals)-1]

#subgrad_test(f_SVM, df_SVM, Pg_SVM, x[0], ttop)
#subgrad_test(f_SVM, df_SVM, Pg_SVM, x[0], lambda t: (np.float(t+1))**-.5, 100)
#subgrad_test(f_SVM, df_SVM, Pg_SVM, x[0], lambda t: (np.log2(np.float(t+2)))**-1., 100)
#subgrad_test(f_SVM, df_SVM, Pg_SVM, x[0], lambda t: (np.log2(np.float(t+2)))**-0.5, 100)
#subgrad_test(f_SVM, df_SVM, Pg_SVM, x[0], lambda t: 1., 100)
#subgrad_test2(f_SVM, df_SVM)



#Nesterov's dual averaging algorithm
def NDA(df, PC, w0, gamma, T=1000):
    w = w0
    s = np.array([0. for i in w0]) #s0
    beta = 1.
    for t in range (0, T):
        dt = df(w)
        s = s+dt
        w = PC(-1./(2.*gamma*beta)*s)
        beta = beta + 1./beta
    return w
    
    


#Nesterov's dual averaging algorithm for testing
def NDA_test(f, df, PC, w0, gamma, T=1000):
    w = w0
    fvals = [0. for i in range (0,T+1)]
    times = [0. for i in range (0,T+1)]
    fvals[0] = f(w)
    times[0] = 0.
    s = np.array([0. for i in w0]) #s0
    beta = 1.
    for t in range (0, T):
        time0 = time.clock()
        dt = df(w)
        s = s+dt
        w = PC(-1./(2.*gamma*beta)*s)
        beta = beta + 1./beta
        time1 = time.clock()
        fvals[t+1] = f(w)
        times[t+1] = time1-time0
    plt.plot(range (0,len(fvals)), fvals, 'r.', ms=2)
    plt.suptitle('Nesterov\'s dual avegaring algorithm objective function value vs iteration')
    plt.xlabel('iteration, t')
    plt.ylabel('f(x_t)')
    plt.show()
    plt.plot(range (0,len(times)), times, 'r.', ms=2)
    plt.suptitle('Nesterov\'s dual avegaring algorithm time vs iteration')
    plt.xlabel('iteration, t')
    plt.ylabel('processing time, in seconds')
    plt.show()
    return w, fvals[len(fvals)-1]
    
    
#NDA_test(f_SVM, df_SVM, PC_SVM, x[0], 0.2)


    
#letting g be the nondifferentiable sum (with the hinge loss terms) makes
#computing its proximal map no easier than just minimizing the objective function directly,
#so I will use g=0 and l=f, the objective function
   

def f_SVM(w): 
    z = 0.
    for i in range (0, n):
        z = z+h(np.float(y[i])*np.dot(w,x[i]))
    return (1./np.float(n))*z + lmbd*np.dot(w,w)
# \Theta(nd) running time
    
    
    
    
def dh(w):
    if w > 1.:
        return 0.
    elif w < 1.:
        return -1.
    return (-1.)*np.random.uniform() #random float in [-1, 0]


def df_SVM(w): #also uses x_i, y_i and lambda, 
#but I've taken these to be global variables
#so that the optimization algorithms don't need to take them as input
    dt = dh(y[0]*(np.dot(w, x[0])))*np.float(y[0])*x[0] #subgradient summand for x[0], y[0]
    for i in range (1, n):
        dt = dt + dh(y[i]*(np.dot(w, x[i])))*np.float(y[i])*x[i]
    dt = (1./np.float(n))*dt
    dt = dt + (2.*lmbd)*w #add the regularizer's gradient
    return dt
# \Theta(nd) running time


#the proximal map for g = 0 is just the identity
def Pg_SVM(et, z):
    return z
    
#similarly, no natural choice for C, 
#so I take it to be the whole space and PC is just the identity
def PC_SVM(z):
    return z
 


#%% 2.2 

#apply to dual problem

#the minimax inequality doesn't seem to hold (the optimal value (take negative of) is greater for dual than the optimal value for the primal)



#proximal gradient algorithm (with backtracking)
def proximalgrad(l, dl, Pg, w0, eta0=1., T=1000):
    w = w0
    wprev = w0
    eta = np.float(eta0)
    for t in range (0,T):
        dt = dl(w)
        z = w - eta*dt
        w = Pg(eta, z)
        dw = w-wprev
        lwprev = l(wprev)
        while l(w) > lwprev + np.dot(dw, dt) + (1./(2.*eta))*np.dot(dw,dw):
            eta = eta/2.
            w = Pg(eta, wprev - eta*dt)
            dw = w-wprev
        wprev = w
    return w
    

#proximal gradient algorithm (with backtracking) for testing
def proximalgrad_test(fp, f, l, dl, Pg, w0, eta0=1., T=1000):
    #f for dual objective, fp for primal objective
    w = w0
    fvals = [0. for i in range (0,T+1)]
    fpvals = [0. for i in range (0,T+1)]
    times = [0. for i in range (0,T+1)]
    fvals[0] = -f(w) #negative of the dual objective (max<->min)
    wp = np.array([0. for j in range (0,d)]) #primal value
    for i in range (0,n):
        wp = wp+w[i]*np.float(y[i])*x[i]
    wp = (1./(2.*lmbd))*wp
    fpvals[0] = fp(wp)
    times[0] = 0.
    wprev = w0
    eta = np.float(eta0)
    for t in range (0,T):
        time0 = time.clock()
        dt = dl(w)
        z = w - eta*dt
        w = Pg(eta, z)
        dw = w-wprev
        lwprev = l(wprev)
        while l(w) > lwprev + np.dot(dw, dt) + (1./(2.*eta))*np.dot(dw,dw):
            eta = eta/2.
            w = Pg(eta, wprev - eta*dt)
            dw = w-wprev
        wprev = w
        time1 = time.clock()
        fvals[t+1] = -f(w)
        wp = np.array([0. for j in range (0,d)]) #primal iterate
        for i in range (0,n):
            wp = wp+w[i]*np.float(y[i])*x[i]
        wp = (1./(2.*lmbd))*wp
        fpvals[t+1] = fp(wp) #primal value
        times[t+1] = time1-time0
    plt.plot(range (0,len(fvals)), fvals, 'r.', range (0,len(fpvals)), fpvals, 'b.', ms=2)
    #label = 'Dual objective value', ms=2)
    #label = 'Primal objective value', 
    plt.suptitle('Proximal gradient algorithm with backtracking dual and primal objective function values vs iteration')
    plt.xlabel('iteration, t')
    plt.ylabel('f(x_t)')
    dl = mlines.Line2D([], [], color='red', label='Dual objective')
    pl = mlines.Line2D([], [], color='blue', label='Primal objective')
    plt.legend(handles=[dl, pl])
    plt.show()
    plt.plot(range (0,len(times)), times, 'r.', ms=2)
    plt.suptitle('Proximal gradient algorithm with backtracking time vs iteration')
    plt.xlabel('iteration, t')
    plt.ylabel('processing time, in seconds')
    plt.show()
    return w, fvals[len(fvals)-1], fpvals[len(fpvals)-1]


#since f_dual = l_dual on the feasible set and we project onto the feasible set before applying f, 
# we may as well just use l_dual for f_dual, too

#proximalgrad_test(f_SVM, l_dual, l_dual, dl_dual, Pg_dual, np.array([0. for i in range (0,n)]), 1./(2.*np.sqrt(n)+1./(4.*lmbd)))
#proximalgrad_test(f_SVM, l_dual, l_dual, dl_dual, Pg_dual, np.random.uniform(0.,1./np.float(n),n), 1./(2.*np.sqrt(n)+1./(4.*lmbd)))
#proximalgrad_test(f_SVM, l_dual, l_dual, dl_dual, Pg_dual, np.array([0. for i in range (0,n)]), 1.)
#proximalgrad_test(f_SVM, l_dual, l_dual, dl_dual, Pg_dual, np.random.uniform(0.,1./np.float(n),n), 1.)

    
#accelerated proximal gradient algorithm
def accelproxgrad(f, l, dl, Pg, w0, eta0=1., T=1000):
    u = w0
    wprev = w0
    w = wprev
    eta = np.float(eta0)
    gamma = 1.
    gprev = gamma
    for t in range (1,T):
        dt = dl(w)
        z = u - eta*dt
        w = Pg(eta, z)
        wu = w-u
        lu = l(u)
        while l(w) > lu + np.dot(wu, dt) + (1./(2.*eta))*np.dot(wu,wu):
            eta = eta/2.
            w = Pg(eta, u - eta*dt)
            wu = w-u
        if f(w) > f(wprev):
            w = wprev
        gprev = gamma
        gamma = (1.+np.sqrt(1.+4.*(gamma**2.)))/2.
        u = w + ((gprev-1.)/gamma)*(w-wprev)
        wprev = w
    return w
    

#accelerated proximal gradient algorithm for testing
def accelproxgrad_test(fp, f, l, dl, Pg, w0, eta0=1., T=1000):
    u = w0
    fvals = [0. for i in range (0,T+1)]
    fpvals = [0. for i in range (0,T+1)]
    times = [0. for i in range (0,T+1)]
    fvals[0] = -f(w0)
    wp = np.array([0. for j in range (0,d)]) #primal value
    for i in range (0,n):
        wp = wp+w0[i]*np.float(y[i])*x[i]
    wp = (1./(2.*lmbd))*wp
    fpvals[0] = fp(wp)
    times[0] = 0.
    wprev = w0
    w = wprev
    eta = np.float(eta0)
    gamma = 1.
    gprev = gamma
    for t in range (1,T+1):
        time0 = time.clock()
        dt = dl(w)
        z = u - eta*dt
        w = Pg(eta, z)
        wu = w-u
        lu = l(u)
        while l(w) > lu + np.dot(wu, dt) + (1./(2.*eta))*np.dot(wu,wu):
            eta = eta/2.
            w = Pg(eta, u - eta*dt)
            wu = w-u
        if f(w) > f(wprev):
            w = wprev
        gprev = gamma
        gamma = (1.+np.sqrt(1.+4.*(gamma**2.)))/2.
        u = w + ((gprev-1.)/gamma)*(w-wprev)
        wprev = w
        time1 = time.clock()
        fvals[t] = -f(w)
        wp = np.array([0. for j in range (0,d)]) #primal value
        for i in range (0,n):
            wp = wp+w[i]*np.float(y[i])*x[i]
        wp = (1./(2.*lmbd))*wp
        fpvals[t] = fp(wp)
        times[t] = time1-time0
    plt.plot(range (0,len(fvals)), fvals, 'r.', range (0,len(fpvals)), fpvals, 'b.', ms=2)
    plt.suptitle('Accelerated proximal gradient algorithm dual and primal objective function values vs iteration')
    plt.xlabel('iteration, t')
    plt.ylabel('f(x_t)')
    plt.ylabel('f(x_t)')
    dl = mlines.Line2D([], [], color='red', label='Dual objective')
    pl = mlines.Line2D([], [], color='blue', label='Primal objective')
    plt.legend(handles=[dl, pl])
    plt.show()
    plt.plot(range (0,len(times)), times, 'r.', ms=2)
    plt.suptitle('Accelerated proximal gradient algorithm time vs iteration')
    plt.xlabel('iteration, t')
    plt.ylabel('processing time, in seconds')
    plt.show()
    return w, fvals[len(fvals)-1], fpvals[len(fpvals)-1]
    

#as above, use l_dual for f_dual, too
#accelproxgrad_test(f_SVM, l_dual, l_dual, dl_dual, Pg_dual, np.array([0. for i in range (0,n)]), 1./(2.*np.sqrt(n)+1./(4.*lmbd)))
#accelproxgrad_test(f_SVM, l_dual, l_dual, dl_dual, Pg_dual, np.random.uniform(0.,1./np.float(n),n), 1./(2.*np.sqrt(n)+1./(4.*lmbd)))
#accelproxgrad_test(f_SVM, l_dual, l_dual, dl_dual, Pg_dual, np.array([0. for i in range (0,n)]), 1.)
#accelproxgrad_test(f_SVM, l_dual, l_dual, dl_dual, Pg_dual, np.random.uniform(0.,1./np.float(n),n), 1.)

#these are actually the negatives; we're minimizing
#f = l+g, where l is the objective function defined everywhere, ignoring the constraint (0<=a<=1), and
# g is the indicator function for the constraint
  
def l_dual(a): 
    #for i in range (0, len(a)): #check that a is in the domain
    #    if a[i] < 0 or a[i] > 1:
    #        return np.float('inf') 
    z = np.float(a[0])*np.float(y[0])*x[0]
    for i in range (1, len(a)):
        z = z + np.float(a[i])*np.float(y[i])*x[i]
    return -1.*sum(a) + (1./(4.*lmbd))*np.dot(z,z)
#\Theta(nd) running time
#Note that this function is L-smooth (for some L, and nondifferentiability aside), because
#because l(b)-l(a) - <b-a, dl(a) > <= 2 ||b-a||_1, and all norms on R^n (a is in R^n, not R^d) are equivalent
#however the best upper bound for L I found is 2*sqrt(n) + 1/(4*lambda), which is pretty big

def dl_dual(a): 
    #ignore the cases a[i]=0 or 1
    dt = []
    #dti = 0.
    #collect common terms, using distributivity of the dot product and scalar multiplication over addition
    Sayx = a[0]*np.float(y[0])*x[0] #will be 1/(2*lambda)* sum_j a[j]*y[j]*x[j]
    for j in range (1, len(a)):
        Sayx = Sayx + a[j]*np.float(y[j])*x[j]
    Sayx = (1./(2.*lmbd))*Sayx
    for i in range (0,len(a)):
        #the code below that is commented out was much slower (\Theta(n^2 d))
        #dti = 0. #partial with respect to a_i
        #for j in range (0, len(a)):
            #dti = dti+a[j]*np.float(y[i])*np.float(y[j])*np.dot(x[i], x[j])
        #dti = (1./(2.*lmbd))*dti - 1.
        dt.append(-1.+np.dot(np.float(y[i])*x[i],Sayx)-1.)
    return np.array(dt)
#\Theta(nd)


def Pg_dual(eta, z):
    a = []
    ai = 0.
    for i in range (0, len(z)):
        if z[i] > 1./np.float(n):
            ai = 1./np.float(n)
        elif z[i] < 0:
            ai = 0.
        else:
            ai = z[i]
        a.append(ai)
    return np.array(a)
#\Theta(n)
#no dependence on eta






#def l_dual(a):
#    for i in range (0, len(a)): #check that a is in the domain
#        if a[i] < 0 or a[i] > 1:
#            return np.float('inf')
#    z = a[0]*np.float(y[0])*x[0]
#    for i in range (1, len(a)):
#        z = z + a[i]*np.float(y[i])*x[i]
#    return (1./(4.*lmbd))*np.dot(z,z)




#def dl_dual(a):
#    for i in range (0, len(a)): #for a outside the domain of f (so f(a) = infinity), the subgradient is empty
#        if a[i] < 0 or a[i] > 1:
#            print 'oops'
#    dt = []
#    dti = 0.
#    for i in range (0,len(a)):
#        dti = 0. #partial with respect to a_i
#        for j in range (0, len(a)):
#            dti = dti+a[j]*np.float(y[i])*np.float(y[j])*np.dot(x[i], x[j])
#        dti = (1./(2.*lmbd))*dti
#        dt.append(dti)
#    return np.array(dt)
    
    
#def g_dual(a):
#    for i in range (0, len(a)): #check that $a$ is in the domain
#        if a[i] < 0 or a[i] > 1:
#            np.float('inf')
#    return -1.*sum(a)
    



#def Pg_dual(et, z): #only valid if the output is between 0 and 1, otherwise need to check ends points, of which there are 2^n
#    return et*(z+1) #z+1: adds 1 to each component of z

        
    
    
#%% 2.3
    
def dMh(eta, z): #derivative of the Moreau envelope of the hinge loss
    if z >= 1.:
        return 0.
    elif z <= 1. - eta:
        return -1.
    return (z-1.)/eta

def f_smoothed(eta, w):
    z = 0.
    for i in range (0, n):
        z = z+Mh(eta, np.float(y[i])*np.dot(w,x[i]))
    return (1./np.float(n))*z + lmbd*np.dot(w,w)
#\Theta(nd)

def df_smoothed(eta, w): #derivative of the objective function with the smoothed hinge loss
    dt = dMh(eta, y[0]*(np.dot(w, x[0])))*np.float(y[0])*x[0]
    for i in range (1, n):
        dt = dt + dMh(eta, y[i]*(np.dot(w, x[i])))*np.float(y[i])*x[i]
    dt = (1./np.float(n))*dt
    dt = dt + (2.*lmbd)*w #add the regularizer's gradient
    return dt
#\Theta(nd)

#smoothed_pg_test(f_SVM, f_smoothed, f_smoothed, df_smoothed, Pg_SVM, x[0])
#smoothed_apg_test(f_SVM, f_smoothed, f_smoothed, df_smoothed, Pg_SVM, x[0])

#Below are the modified proximal gradient and accelerated proximal gradient algorithms,
#for smoothed objective, further taking a sequence of etas


#proximal gradient algorithm (with backtracking), with smoothed objective
def smoothed_pg(l, dl, Pg, w0, eta0=1., etas=ttop, T=1000):
    w = w0
    wprev = w0
    eta = np.float(eta0)
    for t in range (0,T):
        et = etas(t)
        dt = dl(et, w)
        z = w - eta*dt
        w = Pg(eta, z)
        dw = w-wprev
        while l(et, w) > l(et, wprev) + np.dot(dw, dt) + (1./(2.*eta))*np.dot(dw,dw):
            eta = eta/2.
            w = Pg(eta, wprev - eta*dt)
            dw = w-wprev
        wprev = w
    return w


#proximal gradient algorithm (with backtracking) for testing
def smoothed_pg_test(f0, f, l, dl, Pg, w0, eta0=1., etas=ttop, T=1000):
    w = w0
    fvals = [0. for i in range (0,T+1)] #smoothed objective values
    f0vals = [0. for i in range (0,T+1)] #objective values
    times = [0. for i in range (0,T+1)]
    fvals[0] = f(etas(0), w)
    f0vals[0] = f0(w)
    times[0] = 0.
    wprev = w0
    eta = np.float(eta0)
    for t in range (0,T):
        time0 = time.clock()
        et = etas(t+1)
        dt = dl(et, w)
        z = w - eta*dt
        w = Pg(eta, z)
        dw = w-wprev
        while l(et, w) > l(et, wprev) + np.dot(dw, dt) + (1./(2.*eta))*np.dot(dw,dw):
            eta = eta/2.
            w = Pg(eta, wprev - eta*dt)
            dw = w-wprev
        wprev = w
        time1 = time.clock()
        fvals[t+1] = f(et, w)
        f0vals[t+1] = f0(w)
        times[t+1] = time1-time0
    plt.plot(range (0,len(fvals)), fvals, 'r.', range (0,len(f0vals)), f0vals, 'b.', ms=2)
    plt.suptitle('Proximal gradient algorithm with backtracking objective function value vs iteration')
    plt.xlabel('iteration, t')
    plt.ylabel('f(x_t)')
    dl = mlines.Line2D([], [], color='red', label='Smoothed objective')
    pl = mlines.Line2D([], [], color='blue', label='Original objective')
    plt.legend(handles=[dl, pl])
    plt.show()
    plt.plot(range (0,len(times)), times, 'r.', ms=2)
    plt.suptitle('Proximal gradient algorithm with backtracking time vs iteration')
    plt.xlabel('iteration, t')
    plt.ylabel('processing time, in seconds')
    plt.show()
    return w, fvals[len(fvals)-1], f0vals[len(f0vals)-1] 



def smoothed_apg(f, l, dl, Pg, w0, eta0=1., etas=ttop, T=1000):
    u = w0
    wprev = w0
    w = wprev
    eta = np.float(eta0)
    gamma = 1.
    gprev = gamma
    for t in range (1,T):
        et = etas(t)
        dt = dl(et, w)
        z = u - eta*dt
        w = Pg(eta, z)
        wu = w-u
        while l(et, w) > l(et, u) + np.dot(wu, dt) + (1./(2.*eta))*np.dot(wu,wu):
            eta = eta/2.
            w = Pg(eta, u - eta*dt)
            wu = w-u
        if f(et, w) > f(et, wprev): #or could use f0
            w = wprev
        gprev = gamma
        gamma = (1.+np.sqrt(1.+4.*(gamma**2.)))/2.
        u = w + ((gprev-1.)/gamma)*(w-wprev)
        wprev = w
    return w
    
def smoothed_apg_test(f0, f, l, dl, Pg, w0, eta0=1., etas=ttop, T=1000):
    u = w0
    fvals = [0. for i in range (0,T+1)]
    f0vals = [0. for i in range (0,T+1)]
    times = [0. for i in range (0,T+1)]
    fvals[0] = f(etas(0), w0)
    f0vals[0] = f0(w0)
    times[0] = 0.
    wprev = w0
    w = wprev
    eta = np.float(eta0)
    gamma = 1.
    gprev = gamma
    for t in range (1,T+1):
        time0 = time.clock()
        et = etas(t)
        dt = dl(et, w)
        z = u - eta*dt
        w = Pg(eta, z)
        wu = w-u
        while l(et, w) > l(et, u) + np.dot(wu, dt) + (1./(2.*eta))*np.dot(wu,wu):
            eta = eta/2.
            w = Pg(eta, u - eta*dt)
            wu = w-u
        if f(et, w) > f(et, wprev):
            w = wprev
        gprev = gamma
        gamma = (1.+np.sqrt(1.+4.*(gamma**2.)))/2.
        u = w + ((gprev-1.)/gamma)*(w-wprev)
        wprev = w
        time1 = time.clock()
        fvals[t] = f(et, w)
        f0vals[t] = f0(w)
        times[t] = time1-time0
    plt.plot(range (0,len(fvals)), fvals, 'r.', range (0,len(f0vals)), f0vals, 'b.', ms=2)
    plt.suptitle('Accelerated proximal gradient algorithm objective function value vs iteration')
    plt.xlabel('iteration, t')
    plt.ylabel('f(x_t)')
    dl = mlines.Line2D([], [], color='red', label='Smoothed objective')
    pl = mlines.Line2D([], [], color='blue', label='Original objective')
    plt.legend(handles=[dl, pl])
    plt.show()
    plt.plot(range (0,len(times)), times, 'r.', ms=2)
    plt.suptitle('Accelerated proximal gradient algorithm time vs iteration')
    plt.xlabel('iteration, t')
    plt.ylabel('processing time, in seconds')
    plt.show()
    return w, fvals[len(fvals)-1], f0vals[len(f0vals)-1]









#%% 2.4 Comparisons


#vary n,d, lmbd

#reload funtions, since they use the global variables

'''d = 20
n = 500
x = np.random.normal(0.0, 1.0, (n, d)) #matrix of the x_i
y = (-1)**np.random.binomial(1, 0.5, n)
lmbd = 0.5 #lambda, tuning parameter for regularization'''


'''swap n and d
d = 500
n = 20
x = np.random.normal(0.0, 1.0, (n, d)) #matrix of the x_i
y = (-1)**np.random.binomial(1, 0.5, n)'''


'''d = 100
n = 100
x = np.random.normal(0.0, 1.0, (n, d)) #matrix of the x_i
y = (-1)**np.random.binomial(1, 0.5, n)'''


#On iteration time (d=20, n=500):
#subradient algorithms on the primal problem have the fastest iterates; 
#smoothed proximal gradient algorithms on the primal are about twice as slow;
#the proximal gradient algorithms on the dual are further 1.4-2x slower, when initialized on a uniformly random input in the domain
#when instead intitalized on the the 0 vector, they are fast, but the iterates stay near 0 for large problem sizes relative to lambda


#all algorithms on the primal problem are fast to converge in number of iterates, except
#sometimes the subgradient with eta_t = 1/t (unnormalized and no eta_t is fast) is noticeably slower than the others
#and sometimes subgrad_test2 (unnormalized, eta=1, constant) just increases without bound


#The proximal gradient values on the dual problem seem to violate the minimax inequality:
#the dual objective values are much greater than the primal objective values, suggesting a negative duality gap
#so I suspect I made a mistake in the dual objective function
#The smoothed proximal gradient algorithms on the primal problem seem to agree with the subgradient algorithms on the primal problem
#The algorithms on the primal problem (including smoothed (accelerated) proximal gradient) seem to agree pretty well most of the time.

#primal, (proximal) subgradient from 2.1
#subgrad_test(f_SVM, df_SVM, Pg_SVM, x[0], ttop)
#subgrad_test(f_SVM, df_SVM, Pg_SVM, x[0], logttop)
#subgrad_test2(f_SVM, df_SVM)
#NDA_test(f_SVM, df_SVM, PC_SVM, x[0], 0.2)

#dual, proximal gradient from 2.2
#proximalgrad_test(f_SVM, l_dual, l_dual, dl_dual, Pg_dual, np.array([0. for i in range (0,n)]), 1.)
#proximalgrad_test(f_SVM, l_dual, l_dual, dl_dual, Pg_dual, np.random.uniform(0.,1./np.float(n),n), 1.)
#accelproxgrad_test(f_SVM, l_dual, l_dual, dl_dual, Pg_dual, np.array([0. for i in range (0,n)]), 1.)
#accelproxgrad_test(f_SVM, l_dual, l_dual, dl_dual, Pg_dual, np.array([0. for i in range (0,n)]), 1./(2.*np.sqrt(n)+1./(4.*lmbd)))
#accelproxgrad_test(f_SVM, l_dual, l_dual, dl_dual, Pg_dual, np.random.uniform(0.,1./np.float(n),n), 1./(2.*np.sqrt(n)+1./(4.*lmbd)))
#accelproxgrad_test(f_SVM, l_dual, l_dual, dl_dual, Pg_dual, np.random.uniform(0.,1./np.float(n),n), 1.)

#primal, smoothed proximal gradient from 2.3
#smoothed_pg_test(f_SVM, f_smoothed, f_smoothed, df_smoothed, Pg_SVM, x[0])
#smoothed_apg_test(f_SVM, f_smoothed, f_smoothed, df_smoothed, Pg_SVM, x[0])












#%% 2.5 Sparse SVM


#subgrad_test(f1, df1, Pg_SVM, x[0], ttop) #many iterates to converge, but pretty consistently descending
#subgrad_test(f1, df1, Pg_SVM, x[0], logttop) #fewer iterates towards convergence, but oscillates a little
#subgrad_test2(f1, df1) #does not converge at all! oscillates all over the place in some sort of pattern
#smoothed_pg_test(f1, f1_smoothed, f1_smoothed, df1_smoothed, Pg_SVM, x[0]) #seems to converge fast (in number of iterates and time per iterate), but possibly to a larger value than is optimal
#so it might just be converging very slowly; or eta is dropping too quickly
#smoothed_apg_test(f1, f1_smoothed, f1_smoothed, df1_smoothed, Pg_SVM, x[0]) #sometimes has the same problem as smoothed_pg, but often better, too

#subgrad and smoothed (a)pg don't always converge to the same values! (a)pg often converge to greater values
#smoothed apg is often the best


def P1(eta, z): #proximal map of the 1-norm
    w = [0. for j in range (0, len(z))]
    for j in range (0, len(z)):
        if z[j] < -eta:
            w[j] = z[j] + eta
        elif z[j] > eta:
            w[j] = z[j] - eta
    return w

#P_g^eta(z) = sign(z)*(|z|-eta)_+
#\Theta(d)
    
def M1(eta, z): #Moreau envolope of the 1-norm
    w = P1(eta, z)
    zw = z-w
    return (1./eta)*np.dot(zw, zw)+np.linalg.norm(w, 1)
#\Theta(d)

def f1(w): #sparse SVM objective
    z = 0.
    for i in range (0, n):
        z = z+h(np.float(y[i])*np.dot(w,x[i]))
    return (1./np.float(n))*z + lmbd*np.linalg.norm(w, 1)
#\Theta(nd)
    
def df1(w): #subgradient of the objective function
    dt = dh(y[0]*(np.dot(w, x[0])))*np.float(y[0])*x[0]
    for i in range (1, n):
        dt = dt + dh(y[i]*(np.dot(w, x[i])))*np.float(y[i])*x[i]
    dt = (1./np.float(n))*dt
    dt = dt + np.sign(w) #add the l1 regularizer's gradient; 
    #I've chosen to treat the derivative of |.| at 0 as 0 here, rather than more generally from [-1, 1]
    return dt
#\Theta(nd)

def f1_smoothed(eta, w): #sparse SVM objective with smoothed hinge loss
    z = 0.
    for i in range (0, n):
        z = z+Mh(eta, np.float(y[i])*np.dot(w,x[i]))
    return (1./np.float(n))*z + lmbd*np.linalg.norm(w, 1)
#\Theta(nd)

def df1_smoothed(eta, w): #subgradient of the objective function with the smoothed hinge loss
    dt = dMh(eta, y[0]*(np.dot(w, x[0])))*np.float(y[0])*x[0]
    for i in range (1, len(y)):
        dt = dt + dMh(eta, y[i]*(np.dot(w, x[i])))*np.float(y[i])*x[i]
    dt = (1./np.float(n))*dt
    dt = dt + np.sign(w) #add the l1 regularizer's gradient; 
    #I've chosen to treat the derivative of |.| at 0 as 0 here, rather than more generally from [-1, 1]
    return dt
 #\Theta(nd)   

#smooth the the hinge loss (for gradient), since slow otherwise