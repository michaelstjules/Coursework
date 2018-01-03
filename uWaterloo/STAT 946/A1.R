#library("boot")

# 3. theta and theta.hat seem pretty consistently off; even taking n larger doesn't help

pX <- function(){ #returns (p_X(0),p_X(0))
  return(c(1/2,1/2))
}

pY_X <- function(x){ #returns (p_{Y|X}(0|x), p_{Y|X}(1|x))
  p1 = 1/(2^(x+1.))
  return(c(1-p1,p1))
}

pZ_Y <- function(y){ #returns (p_{Z|Y}(0|y),p_{Z|Y}(1|y),p_{Z|Y}(2|y))
  p0 = 1/(3^(y+1))
  p12 = (1-p0)/2
  return(c(p0,p12,p12))
}

theta <- function(i,j,k){ #returns \theta_{ijk}
  pX()[i+1]*pY_X(i)[j+1]*pZ_Y(j)[k+1] #indexing
}

#estimate theta using samples
n=1000
x <- vector('numeric',n)
y <- vector('numeric',n)
z <- vector('numeric',n)

for(i in 1:n){
  x[i] <- sample(c(0,1), 1, prob = pX())
  y[i] <- sample(c(0,1), 1, prob = pY_X(x[i]))
  z[i] <- sample(c(0,1,2), 1, prob = pZ_Y(y[i]))
}

theta.hat <- array(0,dim=c(2,2,3))

for(i in 1:2){
  for(j in 1:2){
    for(k in 1:3){
      theta.hat[i,j,k] <- sum(x==i-1 & y==j-1 & z==k-1)/n #R doesn't like 0 indices
    }
  }
}

# implement basic bootstrap by hand; with replacement
n.bootstrap.samples=1000 #number of boostrap samples
size.bootstrap=1000 #size of bootstrap samples
bootstrap.sample.averages <- array(data = NA, dim = c(n.bootstrap.samples,2,2,3))
se <- array(data = NA, dim = c(2,2,3))
CI <- array(data = NA, dim = c(2,2,3,2))

a = 0.05 #95% confidence interval

for(l in 1:n.bootstrap.samples){
  #bootstrap.sample <- xyz[sample(n, size.bootstrap, replace=TRUE),]
  boostrap.indices <- sample(n, size.bootstrap, replace=TRUE)
  x.b <- x[boostrap.indices]
  y.b <- y[boostrap.indices]
  z.b <- z[boostrap.indices]
  for(i in 1:2){
    for(j in 1:2){
      for(k in 1:3){
        #x.b <- bootstrap.sample[,1]
        #y.b <- bootstrap.sample[,2]
        #z.b <- bootstrap.sample[,3]
        bootstrap.sample.averages[l,i,j,k] <- 
          sum(x.b==i-1 & y.b==j-1 & z.b==k-1)/size.bootstrap
      }
    }
  }
}

#theta.hat.sum <- 0 #should be 1
#theta.sum <- 0 #should be 1
in.CI <- 0 #should be 95% of 12 ~ 11.4

for(i in 1:2){
  for(j in 1:2){
    for(k in 1:3){
       se[i,j,k] <- sd(bootstrap.sample.averages[,i,j,k])
       CI[i,j,k,] <- 2*theta.hat[i,j,k] - quantile(bootstrap.sample.averages[,i,j,k], probs = c(1-a/2, a/2))
       print(paste("theta", i-1,j-1,k-1," = ",theta(i,j,k), sep=""))
       print(paste("theta.hat", i-1,j-1,k-1," = ",theta.hat[i,j,k], sep=""))
       print(paste("CI", i-1,j-1,k-1," = [",CI[i,j,k,1],", ", CI[i,j,k,2],"]", sep=""))
       print(paste("se", i-1,j-1,k-1," = ", se[i,j,k], sep=""))
       #theta.hat.sum <- theta.hat.sum + theta.hat[i,j,k]
       #theta.sum <- theta.sum + theta(i,j,k)
       in.CI = in.CI + (theta(i,j,k)>=CI[i,j,k,1] & theta(i,j,k)<=CI[i,j,k,2])
    }
  }
}

index=1
thetas <- vector('numeric',12)
theta.hats <- vector('numeric',12)
ses <- vector('numeric',12)
CI.L <- vector('numeric',12)
CI.U <- vector('numeric',12)
in.CI <- vector('logical',12)
cols <- c("theta", "theta.hat", "L", "U", "theta in CI", "se")
rows <- vector('character',12)
for(i in 1:2){
  for(j in 1:2){
    for(k in 1:3){
      ses[index] <- sd(bootstrap.sample.averages[,i,j,k])
      CI.L[index] <- 2*theta.hat[i,j,k] - 
        quantile(bootstrap.sample.averages[,i,j,k], probs = c(1-a/2))
      CI.U[index] <- 2*theta.hat[i,j,k] - 
        quantile(bootstrap.sample.averages[,i,j,k], probs = c(a/2))
      thetas[index] <- theta(i-1,j-1,k-1) #indexing
      theta.hats[index] <- theta.hat[i,j,k]
      in.CI[index] <- (thetas[index] <= CI.U[index] & thetas[index] >= CI.L[index])
      rows[index] <- paste(i-1,j-1,k-1, sep="")
      index=index+1
    }
  }
}
output <- data.frame(thetas, theta.hats, CI.L, CI.U, in.CI, ses, row.names=rows)
colnames(output) <- cols
print(output)