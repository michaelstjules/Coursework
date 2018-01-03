import numpy as np
import matplotlib.pyplot as plt
import sympy


#%% 3. 
eps = np.finfo(float).eps

#(a)
def dfdx(x,dx):
    return (np.sin(x+dx)-np.sin(x))/dx
    
#xs = np.linspace(0., 2*np.pi, 1000)
#dfdxs = [dfdx(x,0.000001) for x in xs]
#plt.plot(xs, dfdxs)

logdxs = np.linspace(np.log10(np.finfo(float).eps), -1., 10000)
#logdxs = np.linspace(-10., -7., 10000000) #more refined search for minimum:
#gives 7.9523207758812153e-09
log_errors = [np.log10(abs(dfdx(1.,10.**i)-np.cos(1.))) for i in logdxs] 
#actual error, ignoring the errors inherent in approximating cos and sine
log_round = [np.log10(2*eps*np.sin(1.)/(10.**i)) for i in logdxs] #max error from rounding
log_trunc = [np.log10(abs((10.**i)*np.sin(1.)/2.)) for i in logdxs]
#approximate sin at the point in the interval from Taylor's theorem with just -sin(1)
plt.plot(logdxs,log_errors,'.', label="log_10 actual error",ms=1.5)
plt.plot(logdxs,log_round,label="log_10 round-off error")
plt.plot(logdxs,log_trunc,label="log_10 truncation error")
plt.title("3.(a) log Error vs. log dx")
plt.xlabel("log_10 dx")
plt.ylabel("log_10 |error|")
plt.legend()#loc=9, bbox_to_anchor=(0.5, -0.2))
plt.show()
min(log_errors)
logdxs[np.argmin(log_errors)]
10**logdxs[np.argmin(log_errors)]
2*(eps**0.5)


#%% (b)

def d4fdx4(x,dx):
    return (6.*np.sin(x)-4.*np.sin(x+dx)+np.sin(x+2.*dx)-4.*np.sin(x-dx)+np.sin(x-2.*dx))/(dx**4)

logdxs = np.linspace(np.log10(np.finfo(float).eps), -1., 100000)
#logdxs = np.linspace(np.log10(0.001), np.log10(0.01), 100000) #more refined search;
#gives 0.0034149907635269965
#logdxs = np.linspace(-2.295, -2.29, 10000) # to look for intersection
log_errors = [np.log10(abs(d4fdx4(1.,10.**i)-np.sin(1.))) for i in logdxs]
log_round = [np.log10(eps*16*np.sin(1.)/(10.**(4*i))) for i in logdxs]
log_trunc = [np.log10(abs((10.**(2*i))*np.sin(1.)/5.)) for i in logdxs]
plt.plot(logdxs,log_errors,'.', label="log_10 actual error",ms=1.5)
plt.plot(logdxs,log_round,label="log_10 round-off error")
plt.plot(logdxs,log_trunc,label="log_10 truncation error")
plt.title("3.(b) log Error vs. log dx")
plt.xlabel("log_10 dx")
plt.ylabel("log_10 |error|")
plt.legend()#loc=9, bbox_to_anchor=(0.5, -0.2))
plt.show()
min(log_errors)
logdxs[np.argmin(log_errors)]
10**logdxs[np.argmin(log_errors)]
(80*eps)**(1./6.)



#%% 4.

#switch u to U???

def FTCS(s,u,up,um,dt,dx):
    # s = sigma, u = u_j^n, up = u_{j+1}^n, um = u_{j-1}^n
    return (s*dt/(dx**2))*(up-2.*u+um) + u

#iterate to compute u^{n+1} from u^n for all j together
def FTCS_array(s,u,n,dt,dx,uBCl,uBCr):
    u2 = [0. for i in u]
    u2[0] = uBCl(n*dt)
    u2[-1] = uBCr(n*dt)
    for j in range (1,len(u2)-1):
        u2[j] = FTCS(s,u[j],u[j+1],u[j-1],dt,dx)
    return u2

def initialize(a,b,dx,u0): #a=alpha, b=beta
    J = np.int(np.ceil((b-a)/dx))+1
    u = [u0(a+j*dx) for j in range(0,J)]
    return u

def FTCS_solve(s,a,b,u0,uBCl,uBCr,dx,dt,T):
    u = initialize(a,b,dx,u0)
    J = len(u)
    N = np.int(np.ceil(T/dt))
    x = [a+j*dx for j in range(0,J)]
    for n in range(1,N+1):
        u = FTCS_array(s,u,n,dt,dx,uBCl,uBCr)
    return x, u

#IBVP conditions
def u0(x): #initial conditions
    return np.sin(np.pi*x)

def uBCl(t): #u(-1,t)
    return 0.

def uBCr(t): #u(1,t)
    return 0.

#%% (a)

s = 1.
dx = 0.05
dt = (1./4.)*(dx**2)/s #to satisfy the stability condition, r = 1/4
a=-1.
b=1.
u = initialize(a,b,dx,u0)
J = len(u)
x = [a+j*dx for j in range(0,J)]

plt.plot(x,u,label="u(x,0)")

u1=u
for n in range(1,251):
    u1 = FTCS_array(s,u1,0.,dt,dx,uBCl,uBCr) 
    #uBCl and uBCr don't depend on t here, so I just use 0, but it should be dt*n
plt.plot(x,u1,label="u(x,0.15625)")
for n in range(1,251):
    u1 = FTCS_array(s,u1,0.,dt,dx,uBCl,uBCr) 
    #uBCl and uBCr don't depend on t here, so I just use 0, but it should be dt*n
plt.xlabel("x")
plt.ylabel("y")
plt.plot(x,u1,label="u(x,0.3125)")
plt.title("4.(a) u(x,t) for specific t")
plt.legend()
plt.show()

#solution correctly decays to 0, just by having its scale decreased




#%% (b)

T = 0.1
a=-1.
b=1.


s = 0.5
dx = 0.03
dt = (1./4.)*(dx**2)/s
x, u = FTCS_solve(s,a,b,u0,uBCl,uBCr,dx,dt,T)
plt.plot(x,u,label="sigma=0.5")

s = 1.
dx = 0.03
dt = (1./4.)*(dx**2)/s
x, u = FTCS_solve(s,a,b,u0,uBCl,uBCr,dx,dt,T)
plt.plot(x,u,label="sigma=1")

s = 2.
dx = 0.03
dt = (1./4.)*(dx**2)/s
x, u = FTCS_solve(s,a,b,u0,uBCl,uBCr,dx,dt,T)
plt.plot(x,u,label="sigma=2")
plt.legend()
plt.title("4.(b) Solutions at t=0.1")
plt.xlabel("x")
plt.ylabel("y")
plt.show()

#Larger sigma (s) leads to faster decay. The solutions are identical,
#except for their magnitudes



#%% (c)

def exact(x,t):
    return (np.e**(-(np.pi**2.)*t))*np.sin(np.pi*x)

def exact_discrete(dx,a,J,n):
    return [exact(a+j*dx,n*dt) for j in range(0,J)]

T = 0.1
a=-1.
b=1.
s = 1.

dx = 0.1
dt = (1./4.)*(dx**2)/s #this fixes r=1/4
x, u = FTCS_solve(s,a,b,u0,uBCl,uBCr,dx,dt,T)
J = len(u)
N = np.int(np.ceil(T/dt)) #number of iterations
u_exact = np.array(exact_discrete(dx,a,J,N))
u = np.array(u)
print(np.linalg.norm(u-u_exact,float('inf')))
#0.00151963579736
print((dx**(0.5))*np.linalg.norm(u-u_exact))
#0.00151963579736

#plt.plot(range(0,len(u)),u-u_exact) #looks like sine


dx = 0.05
dt = (1./4.)*(dx**2)/s #this fixes r
x, u = FTCS_solve(s,a,b,u0,uBCl,uBCr,dx,dt,T)
J = len(u)
N = np.int(np.ceil(T/dt)) #number of iterations
u_exact = np.array(exact_discrete(dx,a,J,N))
u = np.array(u)
print(np.linalg.norm(u-u_exact,float('inf')))
#0.000378609269742
print((dx**(0.5))*np.linalg.norm(u-u_exact))
#0.000378609269742


dx = 0.025
dt = (1./4.)*(dx**2)/s #this fixes r
x, u = FTCS_solve(s,a,b,u0,uBCl,uBCr,dx,dt,T)
J = len(u)
N = np.int(np.ceil(T/dt)) #number of iterations
u_exact = np.array(exact_discrete(dx,a,J,N))
u = np.array(u)
print(np.linalg.norm(u-u_exact,float('inf')))
#9.45715118005e-05
print((dx**(0.5))*np.linalg.norm(u-u_exact))
#9.45715118005e-05


dx = 0.0125
dt = (1./4.)*(dx**2)/s #this fixes r
x, u = FTCS_solve(s,a,b,u0,uBCl,uBCr,dx,dt,T)
J = len(u)
N = np.int(np.ceil(T/dt)) #number of iterations
u_exact = np.array(exact_discrete(dx,a,J,N))
u = np.array(u)
print(np.linalg.norm(u-u_exact,float('inf')))
#2.36378341874e-05
print((dx**(0.5))*np.linalg.norm(u-u_exact))
#2.36378341874e-05


#both norms roughly decrease by a factor of 1/4 as dx is halved, 
#suggesting they are quadratic in dx, i.e. O(dx^2) convergence
#also, the scaled 2-norm and max norm are the same




#%% This didn't work for 2.

'''X = sympy.Matrix([[1, 0, 0, 0, 0],
                  [1, 1, 1/2, 1/6, 1/24],
                  [1, 2, 2, 4/3, 2/3],
                  [1, -1, 1/2, -1/6, 1/24],
                  [1, -2, 2, -4/3, 2/3]]).T

# a.
uxx = sympy.Matrix([[0, 0, 1, 0, 0]]).T
#axx = (X**-1)*uxx
axx = X.LUsolve(uxx)
#X*axx #test
uxxx = sympy.Matrix([[0, 0, 0, 1, 0]]).T
axxx = (X**-1)*uxxx
#X*axxx #test

# b.
uxxxx = sympy.Matrix([[0, 0, 0, 0, 1]]).T
axxxx = (X**-1)*uxxxx
#X*axxxx #test'''


#%% Old code for 3.

'''#logdxs = range(np.int(np.ceil(np.log10(np.finfo(float).eps))),0)
#logdxs = np.logspace(np.info(float).eps, 0.1, 50)


plt.figure(1)
plt.clf()
dxs = np.linspace(np.finfo(float).eps, 0.1, 10000)
log_errors = [np.log10(abs(dfdx(1.,dx)-np.cos(1.))) for dx in dxs]
log_round = [np.log10(eps*np.sin(1.)/dx) for dx in dxs]
log_trunc = [np.log10(abs(dx*np.sin(1.)/2.)) for dx in dxs]
plt.plot(dxs,log_errors,label="log_10 total error")
plt.plot(dxs,log_round,label="log_10 round-off error")
plt.plot(dxs,log_trunc,label="log_10 truncation error")
plt.title("log Error vs. dx")
plt.xlabel("dx")
plt.ylabel("log_10 |error|")
plt.legend(loc=9, bbox_to_anchor=(0.5, -0.2))
plt.show()
dxs[np.argmin(log_errors)]
min(log_errors)

plt.figure(2)
plt.clf()
logdxs = np.linspace(np.log10(np.finfo(float).eps), -1., 10000)
errors = [abs(dfdx(1.,10.**i)-np.cos(1.)) for i in logdxs]
plt.plot(logdxs,errors)
plt.title("Error vs. log dx")
plt.xlabel("log_10 dx")
plt.ylabel("|error|")
logdxs[np.argmin(errors)]
min(errors)


#xs = np.linspace(0., 2*np.pi, 1000)
#d4fdx4s = [d4fdx4(x,0.01) for x in xs]
#plt.plot(xs, d4fdx4s)





def d3fdx3(x,dx):
    return (-np.sin(x+dx)+0.5*np.sin(x+2.*dx)+np.sin(x-dx)-0.5*np.sin(x-2.*dx))/(dx**3)

xs = np.linspace(0., 2*np.pi, 1000)
d3fdx3s = [d3fdx3(x,0.0001) for x in xs]
plt.plot(xs, d3fdx3s)



def d2fdx2(x,dx):
    return (-(5./2.)*np.sin(x)+(4./3.)*np.sin(x+dx)-(1./12.)*np.sin(x+2.*dx)+(4./3.)*np.sin(x-dx)-(1./12.)*np.sin(x-2.*dx))/(dx**2)

xs = np.linspace(0., 2*np.pi, 1000)
d2fdx2s = [d2fdx2(x,0.000001) for x in xs]
plt.plot(xs, d2fdx2s)'''