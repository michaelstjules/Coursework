#%% 1.

import numpy as np
import matplotlib.pyplot as plt


# Periodic, otherwise no waves
# Python array indices are already modular

#up = u_{j+1}
#um = u_{j-1}
#alpha = a*dt/dx

#alpha = 1 => no dissipation
#dissipative => stable

#upwind: beta = |alpha|
def upwinda(u,up,um,dx,dt,a=1.): #for a > 0, stable for alpha <= 1, dissipative of order 2
    return u - (a*dt/dx)*(u-um)

def upwindb(u,up,um,dx,dt,a=-1.): #for a < 0, stable for alpha <= 1, dissipative of order 2
    return u - (a*dt/dx)*(up-u)

#beta = 0
def centered(u,up,um,dx,dt,a=1.): #not stable, not dissipative
    return u - (a*dt/(2.*dx))*(up-um)

#beta = 1
def LF(u,up,um,dx,dt,a=1.): #Lax-Friedricks, dissipative of order 2
    return ((um+up) - (a*dt/dx)*(up-um))/2.
    
#beta = alpha^2
def LW(u,up,um,dx,dt,a=1.): #Lax-Wendroff, dissipative of order 4
    return u - (a*dt/(2.*dx))*(up-um) + (((a*dt/dx)**2.)/2.)*(up-2*u+um)
    
def general(u,up,um,alpha,beta,a=1.):
    return u - (alpha/2.)*(up-um)+(beta/2.)*(up-2*u+um)

def scheme_array(u,n,scheme,dx,dt,a=1.):
    #u2 = [0. for i in u]
    J = len(u)
    #for j in range (0,J):
    #    u2[j] = scheme(a,u[j],u[(j+1)%J],u[j-1],dx,dt)
    return [scheme(u[j],u[(j+1)%J],u[j-1],dx,dt,a) for j in range (0,J)]

def initialize(dx,u0,a=1.,boundary_alpha=0.,boundary_beta=1.): #a=alpha, b=beta; take care of modular
    J = np.int(np.ceil((boundary_beta-boundary_alpha)/dx)) #not +1
    u = [u0(boundary_alpha+j*dx,a,boundary_alpha,boundary_beta) for j in range(0,J)]
    return u

def solve(scheme,u0,dx,dt,T,a=1.,boundary_alpha=0.,boundary_beta=1.):
    u = initialize(dx,u0,a,boundary_alpha,boundary_beta)
    J = len(u)
    N = np.int(np.ceil(T/dt))
    x = [boundary_alpha+j*dx for j in range(0,J)]
    for n in range(1,N+1):
        u = scheme_array(u,n,scheme,dx,dt,a)
    return x, u

def gen_array(u,n,alpha,beta):
    #u2 = [0. for i in u]
    J = len(u)
    #for j in range (0,J):
    #    u2[j] = general(u[j],u[(j+1)%J],u[j-1],alpha,beta)
    return [general(u[j],u[(j+1)%J],u[j-1],alpha,beta) for j in range(0,J)]

def gen_solve(alpha,beta,u0,dx,dt,T,a=1.,boundary_alpha=0.,boundary_beta=1.):
    u = initialize(dx,u0,a,boundary_alpha,boundary_beta)
    J = len(u)
    N = np.int(np.ceil(T/dt))
    x = [boundary_alpha+j*dx for j in range(0,J)]
    for n in range(1,N+1):
        u = gen_array(u,n,alpha,beta)
    return x, u

#initial conditions
def u0_cos(x,a=1.,boundary_alpha=0.,boundary_beta=1.): #initial conditions
    return np.cos(2*np.pi*x)

def u0_cos2(x,a=1.,boundary_alpha=0.,boundary_beta=1.): #initial conditions
    return np.cos(10*np.pi*x)

def u0_Gauss(x,a=1.,boundary_alpha=0.,boundary_beta=1.): #initial conditions
    return 2.**(-500.*(x-(boundary_alpha+boundary_beta)/2.)**2.)


def u0_Gauss2(x,a=1.,boundary_alpha=0.,boundary_beta=np.pi/2.):
    u = (x-0.8)/0.1
    y = (1./(np.sqrt(2.*np.pi)*0.1))*np.exp(-u*u/2)
    return y

def cycle(x,boundary_alpha,boundary_beta):
    y = x
    if x >= boundary_alpha:
        if x < boundary_beta:
            return y
        else:
            while y >= boundary_beta:
                y = y-(boundary_beta-boundary_alpha)
    while y < boundary_alpha:
         y = y+(boundary_beta-boundary_alpha)
    return y

def cycle2(x,boundary_alpha,boundary_beta):
    return (x-boundary_alpha)%(boundary_beta-boundary_alpha) + boundary_alpha

def exact_periodic(u0,dx,T,a=1.,boundary_alpha=0.,boundary_beta=1.):
    J = np.int(np.ceil((boundary_beta-boundary_alpha)/dx)) #not +1
    x = [boundary_alpha+j*dx for j in range(0,J)]
    return x, [u0(cycle2(xj-a*T,boundary_alpha,boundary_beta),a,boundary_alpha,boundary_beta) for xj in x]


#%% (a) dissipation error depends on dx: alpha fixed

a=1.
alpha = 0.5 #want alpha <= 1, <1 for dissipation

dx = 0.01
dt = alpha*dx/abs(a)
#x, u = solve(LF,u0_Gauss,dx,dt,T=-1.)
x, u = gen_solve(alpha,1.,u0_Gauss,dx,dt,T=-1.)
plt.plot(x,u,label="u(x,0)", marker=".", markersize=6.)
print np.sqrt(dx)*np.linalg.norm(u)

#x, u = solve(LF,u0_Gauss,dx,dt,T=10.)
x, u = gen_solve(alpha,1.,u0_Gauss,dx,dt,T=10.)
plt.plot(x,u,label="u(x,10), dx=0.01, alpha=0.5", marker="^", markersize=6., markevery=6)
print np.sqrt(dx)*np.linalg.norm(u)

dx = 0.005
alpha = 0.5
dt = alpha*dx/abs(a)
#x, u = solve(LF,u0_Gauss,dx,dt,T=10.)
x, u = gen_solve(alpha,1.,u0_Gauss,dx,dt,T=10.)
plt.plot(x,u,label="u(x,10), dx=0.005, alpha=0.5", marker=">", markersize=6., markevery=7)
print np.sqrt(dx)*np.linalg.norm(u)

dx = 0.001
alpha = 0.5
dt = alpha*dx/abs(a)
#x, u = solve(LF,u0_Gauss,dx,dt,T=10.)
x, u = gen_solve(alpha,1.,u0_Gauss,dx,dt,T=10.)
plt.plot(x,u,label="u(x,10), dx=0.001, alpha=0.5", marker="v", markersize=6., markevery=10)
print np.sqrt(dx)*np.linalg.norm(u)

plt.xlabel("x")
plt.ylabel("y")
plt.title("1.(a) u(x,10) with LF for different dx")
plt.legend(loc=9, bbox_to_anchor=(0.5, -0.2))
plt.show()


#%% (a)' dissipation error depends on dx: fixed dt, alpha varies



a=1.
dt = 1e-3

dx = 1e-2 
alpha = abs(a)*dt/dx

#x, u = solve(LF,u0_Gauss,dx,dt,T=-1.)
x, u = gen_solve(alpha,1.,u0_Gauss,dx,dt,T=-1.,a=1.)
plt.plot(x,u,label="u(x,0)", marker=".", markersize=10.)
print np.sqrt(dx)*np.linalg.norm(u)

#x, u = solve(LF,u0_Gauss,dx,dt,T=10.)
x, u = gen_solve(alpha,1.,u0_Gauss,dx,dt,T=1.,a=1.)
plt.plot(x,u,label="u(x,1), dx=0.01", marker="^", markersize=6., markevery=5)
print np.sqrt(dx)*np.linalg.norm(u)

dx = 5e-3
alpha = abs(a)*dt/dx
#x, u = solve(LF,u0_Gauss,dx,dt,T=10.)
x, u = gen_solve(alpha,1.,u0_Gauss,dx,dt,T=1.,a=1.)
plt.plot(x,u,label="u(x,1), dx=0.005", marker=">", markersize=6., markevery=7)
print np.sqrt(dx)*np.linalg.norm(u)

dx = 1e-3
alpha = abs(a)*dt/dx
#x, u = solve(LF,u0_Gauss,dx,dt,T=10.)
x, u = gen_solve(alpha,1.,u0_Gauss,dx,dt,T=1.,a=1.)
plt.plot(x,u,label="u(x,1), dx=0.001", marker="v", markersize=6., markevery=6)
print np.sqrt(dx)*np.linalg.norm(u)

#dx = 5e-4
#alpha = 0.5 
#alpha = abs(a)*dt/dx
#x, u = solve(LF,u0_Gauss,dx,dt,T=10.)
#x, u = gen_solve(alpha,1.,u0_Gauss,dx,dt,T=1.,a=1.,boundary_alpha=0.,boundary_beta=np.pi/2.)
#plt.plot(x,u,label="u(x,10), dx=0.001, alpha=0.5")
#print np.sqrt(dx)*np.linalg.norm(u)

#dx = 1e-4
#alpha = 0.5 
#alpha = abs(a)*dt/dx
#x, u = solve(LF,u0_Gauss,dx,dt,T=10.)
#x, u = gen_solve(alpha,1.,u0_Gauss,dx,dt,T=1.,a=1.,boundary_alpha=0.,boundary_beta=np.pi/2.)
#plt.plot(x,u,label="u(x,10), dx=0.001, alpha=0.5")
#print np.sqrt(dx)*np.linalg.norm(u)

plt.xlabel("x")
plt.ylabel("y")
plt.title("1.(a) u(x,1) with LF for different dx, dt=0.001")
plt.legend(loc=9, bbox_to_anchor=(0.5, -0.2))
plt.show()



#%% (b) dissipation vs scheme

a=1.
dx = 0.01
alpha = 0.5 #want alpha <= 1, <1 for dissipation
dt = alpha*dx/abs(a)

x, u = solve(LF,u0_Gauss,dx,dt,T=-1.)
#x, u = gen_solve(alpha,1.,u0_Gauss,dx,dt,T=-1.)
plt.plot(x,u,label="u(x,0)", marker=".", markersize=6.)
print np.sqrt(dx)*np.linalg.norm(u)

x, u = solve(upwinda,u0_Gauss,dx,dt,T=2.)
#x, u = gen_solve(alpha,abs(alpha),u0_Gauss,dx,dt,T=2.)
plt.plot(x,u,label="u(x,2)~upwind", marker="^", markersize=6., markevery=5)
print np.sqrt(dx)*np.linalg.norm(u)


#x, u = solve(centered,u0_Gauss,dx,dt,T=10.)
#plt.plot(x,u,label="u(x,10.), centered")
#print np.sqrt(dx)*np.linalg.norm(u)

x, u = solve(LF,u0_Gauss,dx,dt,T=2.)
#x, u = gen_solve(alpha,1.,u0_Gauss,dx,dt,T=2.)
plt.plot(x,u,label="u(x,2)~LF", marker=">", markersize=6., markevery=5)
print np.sqrt(dx)*np.linalg.norm(u)

x, u = solve(LW,u0_Gauss,dx,dt,T=2.)
#x, u = gen_solve(alpha,alpha**2.,u0_Gauss,dx,dt,T=2.)
plt.plot(x,u,label="u(x,2)~LW", marker="v", markersize=6., markevery=5)
print np.sqrt(dx)*np.linalg.norm(u)

plt.xlabel("x")
plt.ylabel("y")
plt.title("1.(b) u(x,2), with dx = 0.01, alpha = 0.5 for different schemes")
plt.legend(loc=9, bbox_to_anchor=(0.5, -0.2))
plt.show()

# Dissipation from least to most: LW, upwind, LF
# This is consistent with the dissipation orders: LW=4, upwind=2, LF=2, 
# since sin^2(theta) <= 1 and |Mk|^2 <= 1-Csin^(2r)(theta), higher order means sin^2r(theta) is closer to 0
# and so |Mk| is closer to 1 (as theta approaches 0, since there's also the constant C)
# Centered does not dissipate at all, and is very unstable, instead. It could not be visibly plotted with the other schemes.



#%% (c) Overall error may increase as dt decreases: Fix dx, let alpha vary

a=1.
alpha = 1. #for first one only
#alpha = abs(a)*dt/dx
dx = abs(a)*0.01/alpha #(=1)

x, u_ex = exact_periodic(u0_Gauss,dx,T=1.)
plt.plot(x,u_ex,label="u(x,1), exact (shifted)", marker=".", markersize=6.)
#print np.sqrt(dx)*np.linalg.norm(u)
#print abs(a)*dt/dx

dt = 0.01
x, u = solve(LF,u0_Gauss,dx,dt,T=1.)
plt.plot(x,u,label="u(x,1), dt=0.01, L2 error="+np.str(np.sqrt(dx)*np.linalg.norm(np.array(u)-np.array(u_ex))), marker="^", markersize=6., markevery=5)
#print np.sqrt(dx)*np.linalg.norm(np.array(u)-np.array(u_ex))
#print np.sqrt(dx)*np.linalg.norm(u)
#print abs(a)*dt/dx

dt = 0.0095
x, u = solve(LF,u0_Gauss,dx,dt,T=1.)
plt.plot(x,u,label="u(x,1), dt=0.0095, L2 error="+np.str(np.sqrt(dx)*np.linalg.norm(np.array(u)-np.array(u_ex))), marker=">", markersize=6., markevery=5)
#print np.sqrt(dx)*np.linalg.norm(np.array(u)-np.array(u_ex))
#print np.sqrt(dx)*np.linalg.norm(u)
#print abs(a)*dt/dx

dt = 0.008
x, u = solve(LF,u0_Gauss,dx,dt,T=1.)
plt.plot(x,u,label="u(x,1), dt=0.008, L2 error="+np.str(np.sqrt(dx)*np.linalg.norm(np.array(u)-np.array(u_ex))), marker="v", markersize=6., markevery=5)
#print np.sqrt(dx)*np.linalg.norm(np.array(u)-np.array(u_ex))
#print np.sqrt(dx)*np.linalg.norm(u)
#print abs(a)*dt/dx

dt = 0.005
x, u = solve(LF,u0_Gauss,dx,dt,T=1.)
plt.plot(x,u,label="u(x,1), dt=0.005, L2 error="+np.str(np.sqrt(dx)*np.linalg.norm(np.array(u)-np.array(u_ex))), marker="<", markersize=6., markevery=5)
#print np.sqrt(dx)*np.linalg.norm(np.array(u)-np.array(u_ex))
#print np.sqrt(dx)*np.linalg.norm(u)
#print abs(a)*dt/dx

dt = 0.001
x, u = solve(LF,u0_Gauss,dx,dt,T=1.)
plt.plot(x,u,label="u(x,1), dt=0.001, L2 error="+np.str(np.sqrt(dx)*np.linalg.norm(np.array(u)-np.array(u_ex))), marker="d", markersize=6., markevery=5)
#print np.sqrt(dx)*np.linalg.norm(np.array(u)-np.array(u_ex))
#print np.sqrt(dx)*np.linalg.norm(u)
#print abs(a)*dt/dx

plt.plot([0.5, 0.5], [0., 1.], color='k', linestyle='--', linewidth=1)


plt.xlabel("x")
plt.ylabel("y")
plt.title("1.(c) u(x,1) with LF for different dt, dx=0.01")
plt.legend(loc=9, bbox_to_anchor=(0.5, -0.2))
plt.show()





#%% (d) different frequency waves have different numerical speeds

a=1.

dt = 0.005
alpha = 0.7
dx = abs(a)*dt/alpha
T=10.1

x, u = solve(LW,u0_cos,dx,dt,0.)
plt.plot(x,u,label="u(x,0), for u0(x)=cos(2*pi*x)", marker="^", markersize=6., markevery=4)
print np.sqrt(dx)*np.linalg.norm(u)

x, u = solve(LW,u0_cos,dx,dt,T)
plt.plot(x,u,label="u(x,10.1), for u0(x)=cos(2*pi*x)", marker="v", markersize=6., markevery=5)
print np.sqrt(dx)*np.linalg.norm(u)
xj = x[np.argmax(u)]

x, u = solve(LW,u0_cos2,dx,dt,0.)
plt.plot(x,u,label="u(x,0), for u0(x)=cos(10*pi*x)", marker=">", markersize=6., markevery=3)
print np.sqrt(dx)*np.linalg.norm(u)

x, u = solve(LW,u0_cos2,dx,dt,T)
plt.plot(x,u,label="u(x,10.1), for u0(x)=cos(10*pi*x)", marker="<", markersize=6., markevery=3)
print np.sqrt(dx)*np.linalg.norm(u)
xj2 = x[np.argmax(u)]

plt.xlabel("x")
plt.ylabel("y")
plt.title("1.(d) u(x,5) with LW, alpha=0.7 for different sine curves")
plt.legend(loc=9, bbox_to_anchor=(0.5, -0.2))
plt.show()

# Consider the leftmost top peaks at (0,1) at t=0 for cos(2*pi*x) (blue, up-pointing triangles) and cos(10*pi*x) (red, right-pointing triangles): they start at the same place (0,1) for cos(2*pi*x) and cos(10*pi*x)
# but cos(2*pi*x) (green, down-pointing triangles) goes further right than does cos(10*pi*x) (cyan, left-pointing triangles) after T=10.1 (each peak in both did 10 full cycles to the rightt and another 1/10th of a cycle)
# I expect a_k < a for both, since I used LW. However, a_k is very close to a for cos(2*pi*x)
# I can calculate the numerical speed by dividing the distance traveled by a peak and then dividing by T
# I did this by computing the distance between a peak at time 0 and the corresponding peak at time T later, 
# adding the integer part of T (number of full cycles, since the length of the interval is 1), and then dividing by T

print("The numerical speeds are " +np.str((xj+10)/T) + " and " + np.str((xj2+10)/T) + ", respectively.")



#%% (e)

# Also from (d), cos(2*pi*x) almost exactly matches the exact solution for relatively large T (e.g. 10.1),
# so that the numerical speed a_k is very close to the actual speed a, and there's little dissipation
# On the other hand, the amplitude of cos(10*pi*x) decreases more than does cos(2*pi*x)'s, and a_k < a is more obvious
# As shown in class, higher frequency => faster dissipation and slower numerical wavespeed, and this is consistent with these observations

# Also in (c), I accidentally had alpha > 1, and the plot was the Lebanese flag :D









#%% Exact test

a=1.
alpha = 1.
dt = 0.005
dx = abs(a)*dt/alpha

x, u = exact_periodic(u0_Gauss,dx,T=0.)
plt.plot(x,u,label="u(x,0), exact (shifted)")
print np.sqrt(dx)*np.linalg.norm(u)

x, u = exact_periodic(u0_Gauss,dx,T=0.1)
plt.plot(x,u,label="u(x,0.5), exact (shifted)")
print np.sqrt(dx)*np.linalg.norm(u)

x, u = exact_periodic(u0_Gauss,dx,T=0.2)
plt.plot(x,u,label="u(x,0.2), exact (shifted)")
print np.sqrt(dx)*np.linalg.norm(u)

x, u = exact_periodic(u0_Gauss,dx,T=0.4)
plt.plot(x,u,label="u(x,0.4), exact (shifted)")
print np.sqrt(dx)*np.linalg.norm(u)

x, u = exact_periodic(u0_Gauss,dx,T=0.8)
plt.plot(x,u,label="u(x,0.8), exact (shifted)")
print np.sqrt(dx)*np.linalg.norm(u)

x, u = exact_periodic(u0_Gauss,dx,T=1.6)
plt.plot(x,u,label="u(x,1.6), exact (shifted)")
print np.sqrt(dx)*np.linalg.norm(u)

x, u = exact_periodic(u0_Gauss,dx,T=3.2)
plt.plot(x,u,label="u(x,3.2), exact (shifted)")
print np.sqrt(dx)*np.linalg.norm(u)

plt.xlabel("x")
plt.ylabel("y")
plt.title("exact u(x,t) at different t")
plt.legend(loc=9, bbox_to_anchor=(0.5, -0.2))
plt.show()


#%% (c) Overall error may increase as dt decreases

'''a=1.
alpha = 0.9 #no observable differences for alpha = 1?

dt = 0.01
dx = abs(a)*dt/alpha

#x, u = solve(LF,u0_Gauss,dx,dt,T=-1.)
#plt.plot(x,u,label="u(x,0)")
#print np.sqrt(dx)*np.linalg.norm(u)

x, u = exact_periodic(u0_Gauss,dx,T=15.)
plt.plot(x,u,label="u(x,15.), exact (shifted)", marker=".", markersize=6.)
print np.sqrt(dx)*np.linalg.norm(u)

x, u = solve(LW,u0_Gauss,dx,dt,T=15.)
plt.plot(x,u,label="u(x,15), dt=0.01", marker="^", markersize=6., markevery=2)
print np.sqrt(dx)*np.linalg.norm(u)

dt = 0.005
dx = abs(a)*dt/alpha
x, u = solve(LW,u0_Gauss,dx,dt,T=15.)
plt.plot(x,u,label="u(x,15), dt=0.005", marker=">", markersize=6., markevery=3)
print np.sqrt(dx)*np.linalg.norm(u)

dt = 0.001
dx = abs(a)*dt/alpha
x, u = solve(LW,u0_Gauss,dx,dt,T=15.)
plt.plot(x,u,label="u(x,15), dt=0.001", marker="v", markersize=6., markevery=15)
print np.sqrt(dx)*np.linalg.norm(u)

plt.plot([0.5, 0.5], [0., 1.], color='k', linestyle='--', linewidth=1)

#dt = 0.0005
#dx = abs(a)*dt/alpha
#x, u = solve(LF,u0_Gauss,dx,dt,T=5.)
#plt.plot(x,u,label="u(x,5), dt=0.0005")
#print np.sqrt(dx)*np.linalg.norm(u)

#dt = 0.0001
#dx = abs(a)*dt/alpha
#x, u = solve(LF,u0_Gauss,dx,dt,T=2.)
#plt.plot(x,u,label="u(x,5), dt=0.0001")
#print np.sqrt(dx)*np.linalg.norm(u)

plt.xlabel("x")
plt.ylabel("y")
plt.title("1.(c) u(x,15) with LF for different dt, and alpha=1")
plt.legend(loc=9, bbox_to_anchor=(0.5, -0.2))
plt.show()

# Opposite seems to be happening? 

# should I be plotting np.norm(u-u_exact)?
'''



#%%


'''dx = 0.005
u = initialize(dx,u0_Gauss)
J = len(u)
x = [j*dx for j in range(0,J)]
a=1.
alpha = 0.5 #want alpha <= 1
dt = alpha*dx/abs(a) #for stability

plt.plot(x,u,label="u(x,0)")
u1=u
print np.sqrt(dx)*np.linalg.norm(u1)
for n in range(1,40):
    u1 = scheme_array(u1,n,upwinda,dx,dt,a)
print np.sqrt(dx)*np.linalg.norm(u1)
plt.plot(x,u1,label="u(x,t')")
for n in range(1,40):
    u1 = scheme_array(u1,n,upwinda,dx,dt,a)
print np.sqrt(dx)*np.linalg.norm(u1)
plt.xlabel("x")
plt.ylabel("y")
plt.plot(x,u1,label="u(x,t'')")
plt.title("u(x,t) for specific t")
plt.legend()
plt.show()'''