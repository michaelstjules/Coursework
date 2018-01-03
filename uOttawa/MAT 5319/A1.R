# 4. (c)

F.inverse <- function(beta, u){
  return(-beta*log(1-u))
}
n = 100;
beta = 50;
u <- runif(n, 0, 1)
x = F.inverse(beta, u)
hist(x);



# 5.
# (a)
P = matrix(c(-10,1,1,10), 2, 2) #orthogonal
D = matrix(c(1,0,0,16), 2, 2)
sigma = P %*% D %*% solve(P)

#install.packages("mvtnorm")
require(mvtnorm)
norm.dat = rmvnorm(2000, sigma=sigma) 
#RStudio couldn't handle 10000 data points for the product of the marginals (10000^2 data points)

#install.packages("gplots")
require(gplots)
df = data.frame(x=norm.dat[,1],y=norm.dat[,2])
hist2d(df, nbins=100)



# (b)


#norm.dat[,1] gives the marginal for x
#norm.dat[,2] gives the marginal for y

h1 <- hist(norm.dat[,1], plot=FALSE)
h2 <- hist(norm.dat[,2], plot=FALSE)


top <- max(h1$counts, h2$counts)
oldpar <- par()
par(mar=c(3,3,1,1))
layout(matrix(c(2,0,1,3),2,2,byrow=T),c(3,1), c(1,3))
hist2d(df, nbins=100)
par(mar=c(0,2,1,0))
barplot(h1$counts, axes=F, ylim=c(0, top), space=0, col='red')
par(mar=c(2,0,0.5,1))
barplot(h2$counts, axes=F, xlim=c(0, top), space=0, col='red', horiz=T)




# (c)

margProd.dat = expand.grid(x=norm.dat[,1],y=norm.dat[,2]) #cartesian product of the x and y values
df2 = data.frame(x=margProd.dat[,1],y=margProd.dat[,2])
par(mar=c(3,3,1,1))
layout(matrix(c(2,0,1,3),2,2,byrow=T),c(3,1), c(1,3))
hist2d(df2, nbins=100)
#Can I avoid having to replot the x and y histograms?
par(mar=c(0,2,1,0))
barplot(h1$counts, axes=F, ylim=c(0, top), space=0, col='red')
par(mar=c(2,0,0.5,1))
barplot(h2$counts, axes=F, xlim=c(0, top), space=0, col='red', horiz=T)

#Notice that the the histogram is no longer diagonal; 
#the product of the marginals does give a Gaussian distribution, but its covariance matrix is diagonal.
#The covariance of the marginal distribition of X is the top left entry of the covariance of (X,Y),
#while that of Y is the bottom right entry, so
#the covariance of the product of marginals is the covariance of (X,Y), but with the non-diagonal entries set to 0.