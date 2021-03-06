# 1. 

pX <- function(){ #returns (p_X(0),p_X(0))
  return(c(1/2,1/2))
}

pY_X <- function(x){ #returns (p_{Y|X}(0|x), p_{Y|X}(1|x))
  p1 = 1/(1+exp(-4*x+2))
  return(c(1-p1,p1))
}

pZ_XY <- function(x,y){ #returns (p_{Z|XY}(0|x,y),p_{Z|XY}(1|x,y))
  p1 = 1/(1+exp(-2*(x+y)+2))
  p0 = 1-p1
  return(c(p0,p1))
}

pXYZ <- function(x,y,z){ #returns p_{XYZ}(x,y,z)
  pX()[x+1]*pY_X(x)[y+1]*pZ_XY(x,y)[z+1] #indexing
}

# (b)
n=10000
pZ1_Y1 <- vector('numeric',n)
Y1_count = 0 # count of Y=1
Z1_Y1_count = 0 # count of Y=1 and Z=1
for(i in 1:n){
  x <- sample(c(0,1), 1, prob = pX())
  y <- sample(c(0,1), 1, prob = pY_X(x))
  z <- sample(c(0,1), 1, prob = pZ_XY(x,y))
  if (y==1){
    Y1_count = Y1_count + 1
    if (z==1){
      Z1_Y1_count = Z1_Y1_count + 1
    }
  }
  pZ1_Y1[i] = Z1_Y1_count/Y1_count #may divide by 0; plot excludes these
}
plot(1:n,pZ1_Y1,type="l")
print(pZ1_Y1[n])




# (d)
pZ1_doY1 <- vector('numeric',n)
Z1_count = 0
for(i in 1:n){
  x <- sample(c(0,1), 1, prob = pX())
  z <- sample(c(0,1), 1, prob = pZ_XY(x,1)) #Y=1
  if (z==1){
      Z1_count = Z1_count + 1
  }
  pZ1_doY1[i] = Z1_count/i
}
plot(1:n,pZ1_doY1,type="l")
print(pZ1_doY1[n])
