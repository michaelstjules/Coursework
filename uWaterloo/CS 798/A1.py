import numpy as np
import matplotlib.pyplot as plt





#fix fp3 for #4

#5???




#A1.fp(np.array([[np.sqrt(2.), 0.],[0., 1.]]))
A = np.array([[np.sqrt(2.), 0.],[0., 1.]])

#%% 3.2

def fp(A, N=1000, eps=0.001, f = lambda t: 1./np.float(t+1)):
    #A is the matrix, N is the maximum number of iterates and eps isan upper bound for the error
    #Fictitious play algorithm
    m = len(A) #number of rows
    n = len(A[0]) #number of columns of A
    AT = np.copy(np.transpose(A))
    #n = len(AT) 
    
    #initialize vectors
    x = np.array([1./m for i in range(m)]) #unnecessary for eta = 1 (so 1-eta=0)
    y = np.array([1./n for i in range(n)]) #same
    #u = np.array([0. for i in range(m)])
    #v = np.array([0. for i in range(n)])
    #ATx = np.array([np.float('nan') for i in range(n)]) #for maxv
    #Ay = np.array([np.float('nan') for i in range(m)]) #for minv
    #p = np.array([])
    #q = np.array([])
    
    gap = np.float('inf')
    #minv = np.float('inf')
    #maxv = np.float('-inf')
    ATxs = [] #stores eta*max v for each iteration
    Ays = [] #stores eta*min u for each iteration
    
    
    #Do one iteration to avoid "null" initial values for plot data
    eta = f(0)
    #eta = 1.
    p = e(0, m) #u_0 = 0, so any vector will do
    x = (1-eta)*x + eta*p #x=p since 1-eta = 0
    #v=v_0 here
    q = e(0, n) #v_0 = 0, so any vector will do
    y = (1-eta)*y + eta*q #y=q
    ATx = np.dot(AT, x)
    Ay = np.dot(A, y)
    v = (1./eta)*ATx #v_1
    u = (1./eta)*Ay #u_1
    t = 1
    
    while not (gap <= eps): #use this for nan
        if t > N-1:
            break
        eta = f(t)
        #eta = 1./np.float(1+t)**exp
        i = np.argmin(u)
        p = e(i, m)
        x = (1-eta)*x + eta*p
        #x = np.array([(1-eta)*z for z in x]) + np.array([eta*z for z in p])
        j = np.argmax(v) #v=v_t here
        
        #for plotting
        #if ATx[j] > maxv:
        #    maxv = ATx[j]
        #if Ay[i] < minv:
        #    minv = Ay[i]
            
        #get added while ATx[j] = Ay[i] = nan without separate loop
        ATxs.append(ATx[j])
        Ays.append(Ay[i])
        
        gap = ATx[j]-Ay[i]
        q = e(j, n)
        y = (1-eta)*y + eta*q
        #y = np.array([(1-eta)*z for z in y]) + np.array([eta*z for z in q])
        ATx = np.dot(AT, x)
        Ay = np.dot(A, y)
        v = (1./eta)*ATx #update to v_{t+1} after using v_t
        u = (1./eta)*Ay
        t = t+1
    plt.xlabel('t')
    plt.ylabel('eta*max v_{t+1} in red, eta*min u_{t+1} in blue')
    plt.plot(range (1, t), ATxs, 'r.', range (1, t), Ays, 'b.', ms=2)
    #, markersize=np.sqrt(1000./(t+1)
    #plt.axis([1, t, np.float(minv-0.1*(maxv-minv)), np.float(maxv+0.1*(maxv-minv))])
    #plt.axis([0,t,1,10])
    plt.show()
    return [t, gap, ATxs[len(ATxs)-1], Ays[len(Ays)-1]]
    
#ATxs[len(ATxs)-1]
#Ays[len(Ays)-1]
    
    
def e(i, n):
    #i-th standard basis vector in R^n, i = 0, ..., n-1
    e = []
    for j in range (0,i): #stops before i
        e.append(0.)
    e.append(1.) #i-th entry
    for j in range (i+1, n):
        e.append(0.)
    return np.array(e)
    

#%% 3.3

def fp2(A, N=1000, eps=0.001):
    #Fictitious play algorithm for #3
    #seems to converge faster in general
    m = len(A) #number of rows
    n = len(A[0]) #number of columns of A
    AT = np.copy(np.transpose(A))
    #n = len(AT) 
    
    #initialize vectors
    x = np.array([1./m for i in range(m)]) #unnecessary for eta = 1 (so 1-eta=0)
    y = np.array([1./n for i in range(n)]) #same

    gap = np.float('inf')
    ATxs = []
    Ays = []
    
    #Do one iteration to avoid "null" initial values for plot data
    eta = 1.
    p = e(0, m) #u_0 = 0, so any vector will do
    x = (1-eta)*x + eta*p #x=p since 1-eta = 0
    ATx = np.dot(AT, x)
    v = (1./eta)*ATx #use v_1
    j = np.argmax(v)
    q = e(j, n)
    y = (1-eta)*y + eta*q #y=q
    ATx = np.dot(AT, x)
    Ay = np.dot(A, y)
    
    u = Ay #u_1
    t = 1
    
    while not (gap <= eps): #use this for nan
        if t > N-1:
            break
        eta = 1./np.float(1+t)
        i = np.argmin(u)
        p = e(i, m)
        x = (1-eta)*x + eta*p
        ATx = np.dot(AT, x)
        v = (t+1)*ATx #update v to v_{t+1} first
        j = np.argmax(v)
            
        ATxs.append(ATx[j])
        Ays.append(Ay[i])
        
        
        #not the correct gap! between max v_{t+1} and min u_t?
        gap = ATx[j]-Ay[i]
        q = e(j, n)
        y = (1-eta)*y + eta*q
        Ay = np.dot(A, y)
        u = (t+1)*Ay
        t = t+1
    plt.xlabel('t')
    plt.ylabel('eta*max v_{t+1} in red, eta*min u_{t+1} in blue')
    plt.plot(range (1, t), ATxs, 'r.', range (1, t), Ays, 'b.', ms=2)
    plt.show()
    return [t, gap, ATxs[len(ATxs)-1], Ays[len(Ays)-1]]
    
  

#generate random matrix
def randMatrix():
    m = np.random.randint(2,5)
    n = np.random.randint(2,5)
    A = np.random.normal(size=(m,n))
    return A  
     

#visual comparison of fp and fp2 for a random matrix     
     
     
def test3(): 
    A = randMatrix()
    print A
    print 'fp'
    print fp(A)
    print 'fp2'
    print fp2(A)
    return
    
    
#compare ratios of performance; fp2 is better

def test_ratios(M=100, N=1000):
    win1 = 0 #number of matrices for which fp had fewer iterations than fp2
    win2 = 0 #number of matrices for which fp2 had fewer iterations than fp
    tie1 = 0 #number of matrices for which max number of iterations was hit by both, but fp was closer to convergence
    tie2 = 0 #number of matrices for which max number of iterations was hit by both, but fp2 was closer to convergence
    for i in range(0,M):
        A = randMatrix()
        m1 = fp_no_plot(A, N)
        m2 = fp2_no_plot(A, N)
        if m1[0]<m2[0]: #fewer iterations for fp
            win1 = win1 + 1
        else:
            if m1[0]>m2[0]: #fewer for fp2
                win2 = win2 + 1
            else:
                if m1[0]==m2[0] and m1[0]==N:
                    if m1[1]<=m2[1]: #m1 had smaller gap
                        tie1 = tie1+1
                    else:
                        tie2 = tie2 + 1
    #return ratios with M
    return np.float(win1)/np.float(M), np.float(tie1)/np.float(M), np.float(win2)/np.float(M), np.float(tie2)/np.float(M)
   
   
   
#v = np.dot(AT,([1./m for i in range(m)])) #just the column sums of A x 1/m
#u = np.array([1./n for i in range(n)]) #just the row sumns of A x 1/n   
   
    
    
    
    
#%% 3.4
    
#doesn't always seem to give the same result as fp, 
#and the difference doesn't look like it's due to rounding
    
def fp3(A, N=1000, eps=0.001, f = lambda t: 1./np.float(t+1)):
    m = len(A) #number of rows
    n = len(A[0]) #number of columns of A
    AT = np.copy(np.transpose(A))
    
    #initialize vectors
    gap = np.float('inf')
    ATxs = []
    Ays = []
    
    #Do one iteration to avoid "null" initial values for plot data
    eta = f(0)
    ATx = (1-eta)*(1./m)*np.array([np.sum(AT[i]) for i in range(n)]) + eta*A[0] #column sums of A
    #Ay = (1-eta)**np.dot(A,y) + eta*AT[0] #row sums of A
    Ay = (1-eta)*(1./n)*np.array([np.sum(A[i]) for i in range(m)]) + eta*AT[0] #row sums of A
    v = (1./eta)*ATx #v_1
    u = (1./eta)*Ay #u_1
    t = 1
    
    while not (gap <= eps): #use this for nan
        if t > N-1:
            break
        etap = eta #keep previous
        eta = f(t)
        i = np.argmin(u)
        j = np.argmax(v)
        ATxs.append(eta*v[j])
        Ays.append(eta*u[i])
        gap = eta*(v[j]-u[i])
        #v = (etap/eta - etap)*v + A[i]
        v = (1-eta)*etap/eta*v + A[i] #update to v_{t+1} after using v_t
        #note that A[i] = row i of A = column i of AT = AT*p
        #u = (etap/eta - etap)*u + AT[j]
        u = (1-eta)*etap/eta*u + AT[j]
        #note that AT[j] = row j of AT = column j of A = A*q
        t = t+1
    plt.xlabel('t')
    plt.ylabel('eta*max v_{t+1} in red, eta*min u_{t+1} in blue')
    plt.plot(range (1, t), ATxs, 'r.', range (1, t), Ays, 'b.', ms=2)
    plt.show()
    return [t, gap, ATxs[len(ATxs)-1], Ays[len(Ays)-1]]
    
    
def test4():
    A = randMatrix()
    print A
    print fp(A)
    print fp3(A)
    return

    
    
    
    
#%% 3.5 
    
#By induction, x_{t+1} is a convex combination of 1/m 1, p_0, p_1, ..., p_t. 
#p_t's coefficient (without duplicates) is eta_t, so greater eta_t gives it more weight. 
#Of those mentioned, eta_t = (t+1)^(-1/2) gives the greatest weight to p_t
    
# Testing for 3.5, using exponents 0, 1/3, 2/3, 1, 4/3, 5/3 and 2, 
#as well as 1/log2(t+2) and 1/2(**t+1)
#On the test A given, 1 works best, 1/3 and 2/3 seem to be converging, but slowly
#and the others don't seem to be converging at all
#Note that exponent 0 gives full weight to p_t and q_t
#in this case, Ay is fixed at 0, while ATx goes sqrt(2), sqrt(2), 1, 1, sqrt(2), sqrt(2), 1, 1, ...


def test5fixed(): #uses the fixed test A
    for i in range (0, 7):
        print '1/(t+1)**'+str(i)+'/3'
        print fp(A, 1000, 0.001, f = lambda t: 1./np.float(t+1)**(np.float(i)/3.))
    print '1/log2(t+2)'
    print fp(A, 1000, 0.001, f = lambda t: 1./np.log2(np.float(t+2)))
    print '1/2**(t+1)'
    print fp(A, 1000, 0.001, f = lambda t: 1./2.**np.float(t+1))
    return
   

#often those that give greatest weights to later terms work very quickly (within a couple dozen iterations)
#but often they don't work at all
#1/(t+1) is most consistent

def test5(): #uses random A
    A = randMatrix()
    for i in range (0, 7):
        print '1/(t+1)**'+str(i)+'/3'
        print fp(A, 1000, 0.001, f = lambda t: 1./np.float(t+1)**(np.float(i)/3.))
    print '1/log2(t+2)'
    print fp(A, 1000, 0.001, f = lambda t: 1./np.log2(np.float(t+2)))
    print '1/2**(t+1)'
    print fp(A, 1000, 0.001, f = lambda t: 1./2.**np.float(t+1))
    return
    
    
#%% extra functions for 3.3   

#same as fp, but no plot 
    
def fp_no_plot(A, N=1000, eps=0.001):
    #A is the matrix, N is the maximum number of iterates and eps isan upper bound for the error
    #Fictitious play algorithm
    m = len(A) #number of rows
    n = len(A[0]) #number of columns of A
    AT = np.copy(np.transpose(A))
    #n = len(AT) 
    
    #initialize vectors
    x = np.array([1./m for i in range(m)]) #unnecessary for eta = 1 (so 1-eta=0)
    y = np.array([1./n for i in range(n)]) #same
    #u = np.array([0. for i in range(m)])
    #v = np.array([0. for i in range(n)])
    #ATx = np.array([np.float('nan') for i in range(n)]) #for maxv
    #Ay = np.array([np.float('nan') for i in range(m)]) #for minv
    #p = np.array([])
    #q = np.array([])
    
    gap = np.float('inf')
    #minv = np.float('inf')
    #maxv = np.float('-inf')
    ATxs = []
    Ays = []
    
    
    #Do one iteration to avoid "null" initial values for plot data
    eta = 1.
    p = e(0, m) #u_0 = 0, so any vector will do
    x = (1-eta)*x + eta*p #x=p since 1-eta = 0
    #v=v_0 here
    q = e(0, n) #v_0 = 0, so any vector will do
    y = (1-eta)*y + eta*q #y=q
    ATx = np.dot(AT, x)
    Ay = np.dot(A, y)
    v = ATx #v_1
    u = Ay #u_1
    t = 1
    
    while not (gap <= eps): #use this for nan
        if t > N-1:
            break
        eta = 1./np.float(1+t)
        i = np.argmin(u)
        p = e(i, m)
        x = (1-eta)*x + eta*p
        #x = np.array([(1-eta)*z for z in x]) + np.array([eta*z for z in p])
        j = np.argmax(v) #v=v_t here
        
        
        #for plotting
        #if ATx[j] > maxv:
        #    maxv = ATx[j]
        #if Ay[i] < minv:
        #    minv = Ay[i]
            
            
        #get added while ATx[j] = Ay[i] = nan without separate loop
        ATxs.append(ATx[j])
        Ays.append(Ay[i])
        
        
        #first element of each will be nan
        gap = ATx[j]-Ay[i]
        q = e(j, n)
        y = (1-eta)*y + eta*q
        #y = np.array([(1-eta)*z for z in y]) + np.array([eta*z for z in q])
        ATx = np.dot(AT, x)
        Ay = np.dot(A, y)
        v = (t+1)*ATx #update to v_{t+1} after using v_t
        u = (t+1)*Ay
        t = t+1
    return [t, gap, ATxs[len(ATxs)-1], Ays[len(Ays)-1]]


#same as fp2, but no plot

def fp2_no_plot(A, N=1000, eps=0.001):
    #Fictitious play algorithm for #3
    #seems to converge faster in general
    m = len(A) #number of rows
    n = len(A[0]) #number of columns of A
    AT = np.copy(np.transpose(A))
    #n = len(AT) 
    
    #initialize vectors
    x = np.array([1./m for i in range(m)]) #unnecessary for eta = 1 (so 1-eta=0)
    y = np.array([1./n for i in range(n)]) #same

    gap = np.float('inf')
    ATxs = []
    Ays = []
    
    
    #Do one iteration to avoid "null" initial values for plot data
    eta = 1.
    p = e(0, m) #u_0 = 0, so any vector will do
    x = (1-eta)*x + eta*p #x=p since 1-eta = 0
    ATx = np.dot(AT, x)
    v = (1./eta)*ATx #use v_1
    j = np.argmax(v)
    q = e(j, n)
    y = (1-eta)*y + eta*q #y=q
    ATx = np.dot(AT, x)
    Ay = np.dot(A, y)
    
    u = Ay #u_1
    t = 1
    
    while not (gap <= eps): #use this for nan
        if t > N-1:
            break
        eta = 1./np.float(1+t)
        i = np.argmin(u)
        p = e(i, m)
        x = (1-eta)*x + eta*p
        #x = np.array([(1-eta)*z for z in x]) + np.array([eta*z for z in p])
        
        ATx = np.dot(AT, x)
        v = (t+1)*ATx #update v to v_{t+1} first
        j = np.argmax(v)
            
        ATxs.append(ATx[j])
        Ays.append(Ay[i])
        
        
        #first element of each will be nan
        gap = ATx[j]-Ay[i]
        q = e(j, n)
        y = (1-eta)*y + eta*q
        #y = np.array([(1-eta)*z for z in y]) + np.array([eta*z for z in q])
        Ay = np.dot(A, y)
        u = (t+1)*Ay
        t = t+1
    return [t, gap, ATxs[len(ATxs)-1], Ays[len(Ays)-1]]