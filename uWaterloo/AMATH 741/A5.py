#%% 3.

import numpy as np
import matplotlib.pyplot as plt





# U_j should be in the middle of the intervals; everything should be shifted by 1/2 dx ?
# can just shift IC vector by 1/2 dx (with modular wrapping)
# Deal with this later, or not at all, since it shouldn't make much difference



#up = u_{j+1}
#um = u_{j-1}


#for Riemann solvers

def U_star_exact(UL, UR): #UL = U_j, UR = U_{j+1}
    if UL > 0 and UR > 0:
        return UL
    elif UL < 0 and UR < 0:
        return UR
    elif UL < 0 and UR > 0:
        return 0
    elif UL + UR > 0:
        return UL
    else:
        return UR

def U_star_upwindish(UL, UR): # this is upwind when f_u > 0
# but the scheme we get from it is 1st order accurate regardless, by #1 (I didn't use f_u > 0)
# not stable unless f_u > 0, though (by numerical domain)
    return UL




def godunov_U_star(u,up,um,dx,dt,U_star,f=lambda z:(z**2.)/2.): 
    return u - (dt/dx)*(f(U_star(u,up))-f(U_star(um,u)))


def LF(u,up,um,dx,dt,l,f=lambda z:(z**2.)/2.): 
    return u - (dt/dx)*(1./2.)*((f(up)-f(um))-l*(up-2*u+um))


def upwindish(u,up,um,dx,dt,f=lambda z:(z**2.)/2.): #this is upwind when f_u > 0
# but this scheme is 1st order consistent regardless, by #1 (we didn't use f_u > 0)
# not stable unless f_u > 0, though (by numerical domain)
    return u - (dt/dx)*(f(u)-f(um))


def godunov_U_star_array(u,n,dx,dt,U_star,f=lambda z:(z**2.)/2.):
    J = len(u)
    return [godunov_U_star(u[j],u[(j+1)%J],u[j-1],dx,dt,U_star,f) for j in range (0,J)]

def LF_array(u,n,dx,dt,f=lambda z:(z**2.)/2.,f_u=lambda z:z):
    J = len(u)
    l = 0
    for j in (0,J-1): #shouldn't it be J instead of J-1? Why didn't J work?
        if abs(f_u(u[j])) > l:
            l = f_u(u[j])
    return [LF(u[j],u[(j+1)%J],u[j-1],dx,dt,l,f) for j in range (0,J)]

def initialize(dx,u0,boundary_alpha=-1.,boundary_beta=1.):
    J = np.int(np.ceil((boundary_beta-boundary_alpha)/dx)) #not +1
    u = [u0(boundary_alpha+j*dx,boundary_alpha,boundary_beta) for j in range(0,J)]
    return u

def solve(u0,dx,dt,T,U_star,boundary_alpha=-1.,boundary_beta=1.,f=lambda z:(z**2.)/2.):
    u = initialize(dx,u0,boundary_alpha,boundary_beta)
    #plot initial conditions?
    J = len(u)
    N = np.int(np.ceil(T/dt))
    x = [boundary_alpha+j*dx for j in range(0,J)]
    for n in range(1,N+1):
        u = godunov_U_star_array(u,n,dx,dt,U_star,f)
        #plot solution at this time?
    return x, u


def LF_solve(u0,dx,dt,T,boundary_alpha=-1.,boundary_beta=1.,f=lambda z:(z**2.)/2.,f_u=lambda z:z):
    u = initialize(dx,u0,boundary_alpha,boundary_beta)
    #plot initial conditions?
    J = len(u)
    N = np.int(np.ceil(T/dt))
    x = [boundary_alpha+j*dx for j in range(0,J)]
    for n in range(1,N+1):
        u = LF_array(u,n,dx,dt,f,f_u)
        #plot solution at this time?
    return x, u




#initial conditions
def u0_sin(x, boundary_alpha=-1., boundary_beta=1.):
    return np.sin(np.pi*x)

def u0_sinp1d2(x, boundary_alpha=-1., boundary_beta=1.):
    return (np.sin(np.pi*x)+1)/2




def characteristic(x0, t, u0, boundary_alpha=-1.,boundary_beta=1.):
    #dx/dt = u0(x) => x = x0 + u0(x)*t
    return (x0+u0(x0)*t-boundary_alpha)%(boundary_beta-boundary_alpha)+boundary_alpha #wrap around for periodic BC?

def plot_chars(u0,T=2.,dt=0.005,dx=0.1,boundary_alpha=-1.,boundary_beta=1.):
    J = np.int(np.ceil((boundary_beta-boundary_alpha)/dx)) #not +1
    x0s = [boundary_alpha+j*dx for j in range(0,J)]
    N = np.int(T/dt)
    ts = [dt*n for n in range(0,N)]
    for x0 in x0s:
        xs = [characteristic(x0, t, u0, boundary_alpha, boundary_beta) for t in ts]
        plt.plot(xs, ts, linestyle='None', marker='.', markersize=2.)
    plt.title("Characteristics")
    plt.xlabel("x")
    plt.ylabel("t")
    plt.show()
    return




#%% (a) u_0(x) = (sin(pi*x)+1)/2
# This could be more efficient, since it solves from the beginning each time

#CFL condition: dt <= |u(x,0)|*dx, for all x; since |u(x,0)| <=1, this is just
# dt <= dx
dx = 0.01
dt = 0.01

x, u = solve(u0_sinp1d2,dx,dt,T=0,U_star=U_star_exact)
plt.plot(x,u,label="U(x,0), initial conditions", marker=".", markersize=6.)

x, u = solve(u0_sinp1d2,dx,dt,T=0.1,U_star=U_star_exact)
plt.plot(x,u,label="U(x,0.1)", marker=".", markersize=6.)

x, u = solve(u0_sinp1d2,dx,dt,T=2./np.pi,U_star=U_star_exact)
plt.plot(x,u,label="U(x,2/pi) (shock/compression)", marker=".", markersize=6.)

x, u = solve(u0_sinp1d2,dx,dt,T=0.73,U_star=U_star_exact)
plt.plot(x,u,label="U(x,0.73)", marker=".", markersize=6.)

x, u = solve(u0_sinp1d2,dx,dt,T=1.,U_star=U_star_exact)
plt.plot(x,u,label="U(x,1)", marker=".", markersize=6.)

x, u = solve(u0_sinp1d2,dx,dt,T=2.,U_star=U_star_exact)
plt.plot(x,u,label="U(x,2)", marker=".", markersize=6.)

x, u = solve(u0_sinp1d2,dx,dt,T=4.,U_star=U_star_exact)
plt.plot(x,u,label="U(x,2)", marker=".", markersize=6.)

plt.xlabel("x")
plt.ylabel("y")
plt.title("3.(a) U(x,T) at different times T, where u_0(x) = (sin(pi*x)+1)/2")
plt.legend(loc=9, bbox_to_anchor=(0.5, -0.2))
plt.show()

#plot characteristics
plot_chars(u0_sinp1d2)
#plot more characteristics to close the gaps between them
plot_chars(u0_sinp1d2, dx=0.005)
# intersection and so compression region at time t = 2/pi ~ 0.64 and for some -1 < x <-0.5, 
# but the shock isn't obvious until about t=0.73
# no rarefaction, as the whole domain is covered (since u0 is continuous?)


#%% (a) u_0(x) = sin(pi*x)

#CFL conditions: dt <= |u(x,0)|*dx, for all x; since |u(x,0)| <=1, this is just
# dt <= dx
dx = 0.01
dt = 0.01

x, u = solve(u0_sin,dx,dt,T=0,U_star=U_star_exact)
plt.plot(x,u,label="U(x,0), initial conditions", marker=".", markersize=6.)

x, u = solve(u0_sin,dx,dt,T=0.1,U_star=U_star_exact)
plt.plot(x,u,label="U(x,0.1)", marker=".", markersize=6.)

x, u = solve(u0_sin,dx,dt,T=1./np.pi,U_star=U_star_exact)
plt.plot(x,u,label="U(x,1/pi) (shock/compression)", marker=".", markersize=6.)

x, u = solve(u0_sin,dx,dt,T=1.,U_star=U_star_exact)
plt.plot(x,u,label="U(x,1)", marker=".", markersize=6.)

x, u = solve(u0_sin,dx,dt,T=2.,U_star=U_star_exact)
plt.plot(x,u,label="U(x,2)", marker=".", markersize=6.)

x, u = solve(u0_sin,dx,dt,T=4.,U_star=U_star_exact)
plt.plot(x,u,label="U(x,2)", marker=".", markersize=6.)

plt.xlabel("x")
plt.ylabel("y")
plt.title("3.(a) U(x,T) at different times T, where u_0(x) = sin(pi*x)")
plt.legend(loc=9, bbox_to_anchor=(0.5, -0.2))
plt.show()



# plot characteristics
plot_chars(u0_sin)
plot_chars(u0_sin, dx=0.005)
# first intersection at time t = 1/pi (by shock conditions), at x=-1 and x=1,
# and there's a shock there (a large gap between u(-1,t) and u(1,t))
# again, no rarefaction, as the whole domain is covered (since u0 is continuous?)





#%% (b) u_0(x) = (sin(pi*x)+1)/2

dx = 0.005
dt = 0.005

x, u = solve(u0_sinp1d2,dx,dt,T=0,U_star=U_star_exact)
plt.plot(x,u,label="U(x,0), initial conditions", marker=".", markersize=6.)

x, u = solve(u0_sinp1d2,dx,dt,T=1.,U_star=U_star_exact)
plt.plot(x,u,label="U(x,1)", marker=".", markersize=6.)

x, u = solve(u0_sinp1d2,dx,dt,T=5.,U_star=U_star_exact)
plt.plot(x,u,label="U(x,5)", marker=".", markersize=6.)

x, u = solve(u0_sinp1d2,dx,dt,T=10.,U_star=U_star_exact)
plt.plot(x,u,label="U(x,10)", marker=".", markersize=6.)

x, u = solve(u0_sinp1d2,dx,dt,T=25.,U_star=U_star_exact)
plt.plot(x,u,label="U(x,25)", marker=".", markersize=6.)

x, u = solve(u0_sinp1d2,dx,dt,T=50.,U_star=U_star_exact)
plt.plot(x,u,label="U(x,50)", marker=".", markersize=6.)

plt.xlabel("x")
plt.ylabel("y")
plt.title("3.(b) U(x,T) at different times T, where u_0(x) = (sin(pi*x)+1)/2")
plt.legend(loc=9, bbox_to_anchor=(0.5, -0.2))
plt.show()

#The solution flattens out towards ~0.5 constant over time

#%% (b) halving dx and dt

dx = 0.0025
dt = 0.0025

x, u = solve(u0_sinp1d2,dx,dt,T=0,U_star=U_star_exact)
plt.plot(x,u,label="U(x,0), initial conditions", marker=".", markersize=6.)

x, u = solve(u0_sinp1d2,dx,dt,T=1.,U_star=U_star_exact)
plt.plot(x,u,label="U(x,1)", marker=".", markersize=6.)

x, u = solve(u0_sinp1d2,dx,dt,T=5.,U_star=U_star_exact)
plt.plot(x,u,label="U(x,5)", marker=".", markersize=6.)

x, u = solve(u0_sinp1d2,dx,dt,T=10.,U_star=U_star_exact)
plt.plot(x,u,label="U(x,10)", marker=".", markersize=6.)

x, u = solve(u0_sinp1d2,dx,dt,T=25.,U_star=U_star_exact)
plt.plot(x,u,label="U(x,25)", marker=".", markersize=6.)

x, u = solve(u0_sinp1d2,dx,dt,T=50.,U_star=U_star_exact)
plt.plot(x,u,label="U(x,50)", marker=".", markersize=6.)

plt.xlabel("x")
plt.ylabel("y")
plt.title("3.(b) U(x,T) at different times T, where u_0(x) = (sin(pi*x)+1)/2")
plt.legend(loc=9, bbox_to_anchor=(0.5, -0.2))
plt.show()

# The shock is sharper or more vertical (as expected, since we're getting closer 
# to the shock from each side). The plots are pretty indistinguishable otherwise. 
# Plotting this already takes too long, so I won't reduce dx further.  


#%% (b) larger T (reduced dx, dt)

dx = 0.01
dt = 0.01

x, u = solve(u0_sinp1d2,dx,dt,T=0,U_star=U_star_exact)
plt.plot(x,u,label="U(x,0), initial conditions", marker=".", markersize=6.)

x, u = solve(u0_sinp1d2,dx,dt,T=50,U_star=U_star_exact)
plt.plot(x,u,label="U(x,50)", marker=".", markersize=6.)

x, u = solve(u0_sinp1d2,dx,dt,T=100,U_star=U_star_exact)
plt.plot(x,u,label="U(x,100)", marker=".", markersize=6.)

x, u = solve(u0_sinp1d2,dx,dt,T=200,U_star=U_star_exact)
plt.plot(x,u,label="U(x,200)", marker=".", markersize=6.)

plt.xlabel("x")
plt.ylabel("y")
plt.title("3.(b) U(x,T) at different times T, where u_0(x) = (sin(pi*x)+1)/2")
plt.legend(loc=9, bbox_to_anchor=(0.5, -0.2))
plt.show()


#%% (c) 

dx = 0.01
dt = 0.01

# exact flux

x, u = solve(u0_sinp1d2,dx,dt,T=0,U_star=U_star_exact)
plt.plot(x,u,label="U(x,0), initial conditions", marker=".", markersize=6.)

x, u = solve(u0_sinp1d2,dx,dt,T=0.1,U_star=U_star_exact)
plt.plot(x,u,label="U(x,0.1)", marker=".", markersize=6.)

x, u = solve(u0_sinp1d2,dx,dt,T=2./np.pi,U_star=U_star_exact)
plt.plot(x,u,label="U(x,2/pi)", marker=".", markersize=6.)

x, u = solve(u0_sinp1d2,dx,dt,T=1.,U_star=U_star_exact)
plt.plot(x,u,label="U(x,1)", marker=".", markersize=6.)

x, u = solve(u0_sinp1d2,dx,dt,T=2.,U_star=U_star_exact)
plt.plot(x,u,label="U(x,2)", marker=".", markersize=6.)

x, u = solve(u0_sinp1d2,dx,dt,T=4.,U_star=U_star_exact)
plt.plot(x,u,label="U(x,2)", marker=".", markersize=6.)

plt.xlabel("x")
plt.ylabel("y")
plt.title("3.(c) U(x,T) at different times T with exact flux, where u_0(x) = (sin(pi*x)+1)/2")
plt.legend(loc=9, bbox_to_anchor=(0.5, -0.2))
plt.show()


# local LF

x, u = LF_solve(u0_sinp1d2,dx,dt,T=0)
plt.plot(x,u,label="U(x,0), initial conditions", marker=".", markersize=6.)

x, u = LF_solve(u0_sinp1d2,dx,dt,T=0.1)
plt.plot(x,u,label="U(x,0.1)", marker=".", markersize=6.)

x, u = LF_solve(u0_sinp1d2,dx,dt,T=2./np.pi)
plt.plot(x,u,label="U(x,2/pi)", marker=".", markersize=6.)

x, u = LF_solve(u0_sinp1d2,dx,dt,T=1.)
plt.plot(x,u,label="U(x,1)", marker=".", markersize=6.)

x, u = LF_solve(u0_sinp1d2,dx,dt,T=2.)
plt.plot(x,u,label="U(x,2)", marker=".", markersize=6.)

x, u = LF_solve(u0_sinp1d2,dx,dt,T=4.)
plt.plot(x,u,label="U(x,2)", marker=".", markersize=6.)

plt.xlabel("x")
plt.ylabel("y")
plt.title("3.(c) U(x,T) at different times T with local LF, where u_0(x) = (sin(pi*x)+1)/2")
plt.legend(loc=9, bbox_to_anchor=(0.5, -0.2))
plt.show()

#local LF suffers from oscillations near the shock (by time t=2)