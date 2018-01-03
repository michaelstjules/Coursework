#Note that cov and var use n-1 as the denominators

# Q3:
normalize <- function(x) {
  return ((x - mean(x)) / sd(x)) }

# Load, normalize and organize the data:
coris = read.csv("coris.dat")
coris[["row.names"]] = NULL
coris[["famhist"]] = NULL
coris = coris[-c(1,2),] # Discard first two rows (explanatory text)
normcoris = as.data.frame(lapply(coris[,1:8], normalize))  # normalize all predictors
normcoris$chd = coris$chd
data = data.matrix(normcoris)
data = data[sample(nrow(data)),]  # randomise order of rows
N = nrow(data)
traindata = data[1:(N/2),]
testdata = data[(N/2+1):N,]
X = traindata[,1:8]
Y = traindata[,9]
Xtest = testdata[,1:8]
Ytest = testdata[,9]
X0 = X[Y==0,]
X1 = X[Y==1,]
Pi0 = sum(Y==0)/length(Y)
Pi1 = sum(Y==1)/length(Y)
# Finished data loading and preprocessing


# 3. (a)
require(mvtnorm)
require(gmodels)

Xmean = colMeans(X)
Xcov = cov(X)

X0 = X[Y==0,]
Xmean0 = colMeans(X0)
Xcov0 = cov(X0)

X1 = X[Y==1,]
Xmean1 = colMeans(X1)
Xcov1 = cov(X1)


#i. QDA 

likeli0.QDA=dmvnorm(Xtest,mean=Xmean0,sigma=Xcov0)
likeli1.QDA=dmvnorm(Xtest,mean=Xmean1,sigma=Xcov1)

PX.QDA = likeli0.QDA*Pi0+likeli1.QDA*Pi1 #p(x=x_0)
PY.QDA = matrix(c(likeli0.QDA*Pi0, likeli1.QDA*Pi1)/PX.QDA, nrow = length(Ytest), ncol = 2) 
#posterior distributions p(y=c|x=x_0), i,j-th entry is p(y=j-1|x=x_i)

Ypred.MAP.QDA = vector(length = length(Ytest))

for(i in 1:length(Ypred.MAP.QDA)){
  if(PY.QDA[i,2] >= PY.QDA[i,1]){Ypred.MAP.QDA[i]=1} #choose y=1 on the boundary
  else {Ypred.MAP.QDA[i]=0}
}






#ii. LDA

likeli0.LDA=dmvnorm(Xtest,mean=Xmean0,sigma=Xcov)
likeli1.LDA=dmvnorm(Xtest,mean=Xmean1,sigma=Xcov)

PX.LDA = likeli0.LDA*Pi0+likeli1.LDA*Pi1 #p(x=x_0)
PY.LDA = matrix(c(likeli0.LDA*Pi0, likeli1.LDA*Pi1)/PX.QDA, nrow = length(Ytest), ncol = 2) 
#posterior distributions p(y=c|x=x_0), i,j-th entry is p(y=j-1|x=x_i)

Ypred.MAP.LDA = vector(length = length(Ytest))

for(i in 1:length(Ypred.MAP.LDA)){
  if(PY.LDA[i,2] >= PY.LDA[i,1]){Ypred.MAP.LDA[i]=1}
  else {Ypred.MAP.LDA[i]=0}
}





#iii. GNB with per-class variances 

Xcov0.diag.vector = vector(length = ncol(X))
Xcov1.diag.vector = vector(length = ncol(X))
for(j in 1:ncol(X)){
  Xcov0.diag.vector[j] = var(X0[,j])
  Xcov1.diag.vector[j] = var(X1[,j])
}
Xcov0.diag = diag(Xcov0.diag.vector)
Xcov1.diag = diag(Xcov1.diag.vector)

likeli0.GNBc=dmvnorm(Xtest,mean=Xmean0,sigma=Xcov0.diag)
likeli1.GNBc=dmvnorm(Xtest,mean=Xmean1,sigma=Xcov1.diag)

PX.GNBc = likeli0.GNBc*Pi0+likeli1.GNBc*Pi1 #p(x=x_0)
PY.GNBc = matrix(c(likeli0.GNBc*Pi0, likeli1.GNBc*Pi1)/PX.GNBc, nrow = length(Ytest), ncol = 2) 
#posterior distributions p(y=c|x=x_0), i,j-th entry is p(y=j-1|x=x_i)

Ypred.MAP.GNBc = vector(length = length(Ytest))

for(i in 1:length(Ypred.MAP.GNBc)){
  if(PY.GNBc[i,2] >= PY.GNBc[i,1]){Ypred.MAP.GNBc[i]=1}
  else {Ypred.MAP.GNBc[i]=0}
}




#iv. GNB with pooled variances

Xcov.diag.vector = vector(length = ncol(X)) 
for(j in 1:ncol(X)){
  Xcov.diag.vector[j] = var(X[,j])
}
#these should all be close to 1, since we normalized the data, but the training data (without the test data) may not be normalized
Xcov.diag = diag(Xcov.diag.vector)

likeli0.GNBp=dmvnorm(Xtest,mean=Xmean0,sigma=Xcov.diag)
likeli1.GNBp=dmvnorm(Xtest,mean=Xmean1,sigma=Xcov.diag)

PX.GNBp = likeli0.GNBp*Pi0+likeli1.GNBp*Pi1 #p(x=x_0)
PY.GNBp = matrix(c(likeli0.GNBp*Pi0, likeli1.GNBp*Pi1)/PX.GNBp, nrow = length(Ytest), ncol = 2) 
#posterior distributions p(y=c|x=x_0), i,j-th entry is p(y=j-1|x=x_i)

Ypred.MAP.GNBp = vector(length = length(Ytest))

for(i in 1:length(Ypred.MAP.GNBp)){
  if(PY.GNBp[i,2] >= PY.GNBp[i,1]){Ypred.MAP.GNBp[i]=1}
  else {Ypred.MAP.GNBp[i]=0}
}



#v. Regularised LDA, with LDA and pooled GNB covariance matrices

lambda = 0.5
Xcov.RDA = lambda*Xcov + (1-lambda)*Xcov.diag

likeli0.RDA=dmvnorm(Xtest,mean=Xmean0,sigma=Xcov.RDA)
likeli1.RDA=dmvnorm(Xtest,mean=Xmean1,sigma=Xcov.RDA)

PX.RDA = likeli0.RDA*Pi0+likeli1.RDA*Pi1 #p(x=x_0)
PY.RDA = matrix(c(likeli0.RDA*Pi0, likeli1.RDA*Pi1)/PX.RDA, nrow = length(Ytest), ncol = 2) 
#posterior distributions p(y=c|x=x_0), i,j-th entry is p(y=j-1|x=x_i)

Ypred.MAP.RDA = vector(length = length(Ytest))

for(i in 1:length(Ypred.MAP.RDA)){
  if(PY.RDA[i,2] >= PY.RDA[i,1]){Ypred.MAP.RDA[i]=1}
  else {Ypred.MAP.RDA[i]=0}
}





#vi. kNN, k=3, https://stat.ethz.ch/R-manual/R-devel/library/class/html/knn.html
#install.packages("class")
require(class)
#?knn
kNN = knn(X, Xtest, Y, k=3, l=0, prob = TRUE, use.all = TRUE)
Ypred.kNN = kNN
attributes(Ypred.kNN) <- NULL #remove attributes, but replaces 1 with 2 and 0 with 1
Ypred.kNN = Ypred.kNN-1 #back to a vector of 0s and 1s
PY.kNN = matrix(c(attributes(kNN)$prob*as.numeric(Ypred.kNN==0)+(1-attributes(kNN)$prob)*as.numeric(Ypred.kNN==1), attributes(kNN)$prob*as.numeric(Ypred.kNN==1)+(1-attributes(kNN)$prob)*as.numeric(Ypred.kNN==0)), nrow = length(Ytest), ncol = 2)



#my own implementation (which doesn't work): 
#use sorted (by distance) list to find and keep track of the nearest neighbours
#I use a linked list of the k-nearest neighbours seen so far, sorted from farthest to closest
#storing the training data X in a spatial data partitioning tree (https://en.wikipedia.org/wiki/Spatial_database#Spatial_index, e.g. a k-d tree) would be much faster if there are many test points in Xtest
#https://www.rforge.net/doc/packages/RANN/nn.html


#Linked list stuff
setRefClass("LLNode", fields=list(data = "ANY", nextNode = "ANY"))

newLLNode <- function(x, p){ #adds a new node with data x after node p
  y = new("LLNode", data=x, nextNode=NULL) 
  if(!is.null(p)){
    y$nextNode = p$nextNode
    p$nextNode = y
  }
  return(y)
}

newLL <- function(){
  head = newLLNode("head", NULL) 
  #tail = newLLNode("tail", head)
  return(c("head"=head, "length"=0)) #return(c("head"=head, "tail"=tail, "length"=0))
}

LL2vector <- function(list){
  v = vector(length=list$length)
  z = list$head$nextNode
  for(i in 1:length(v)){
    v[i] = z$data
    z = z$nextNode
  }
  return(v)
}

#this doesn't work :(
find_kNNs <- function(x, xtest, y, k){ #returns the indices of the k nearest neighbours of xtest (a single data point) in x
  #sorted from farthest to closest
  neighbours = newLL()
  #newLLNode(1, neighbours$head) #add the first node, for x[1,], to start
  #neighbours$length = 1
  for(i in 1:nrow(x)){
    z = neighbours$head
    while(!is.null(z$nextNode)){ #while(!identical(z$nextNode,neighbours$tail)){
      #find the last node z s.t. x[i,] is closer to xtest than z is; z$nextNode will be the first closer node (or at same distance)
      if(sum((x[i,]-xtest)^2) < sum((x[z$nextNode$data,]-xtest)^2)){ #separate from while condition, to avoid trying to access "data" of a NULL node
        #if x[i,] is strictly closer to xtest than the next node is, continue to the next node
        z = z$nextNode
      }
      else{
        break
      }
    }
    if(neighbours$length==k & identical(z,neighbours$head)){ #if x[i,] wasn't closer to xtest than any of those in the list, and the list is full
      next
    }
    #otherwise
    u = newLLNode(i,z) #add i after z
    if(neighbours$length==k){ #if the list was full before adding i
      neighbours$head$nextNode <- neighbours$head$nextNode$nextNode #remove the first node
    }
    else{
      neighbours$length = neighbours$length+1
    }
  }
  return(neighbours)
  #return(LL2vector(neighbours))
}

my.kNN <- function(x, xtest, y, k){ #here xtest will be all the test data points, not just one
  py.kNN = matrix(ncol = 3, nrow = nrow(xtest))
  for(i in 1:nrow(xtest)){
    neighbours = LL2vector(find_kNNs(x, xtest[i], y, k))
    #neighbour.classes = vector(length=length(neighbours))
    #neighbours = find_kNNs(x, xtest[i], y, k)
    for(j in 1:length(neighbours)){
      neighbours[j] = y[neighbours[j]] #replace the index by its class
    }
    py.kNN[i,2] = sum(neighbours)/k #probability of class 1
  }
  py.kNN[,1] = 1-py.kNN[,2] #probability of class 0
  py.kNN[,3] = as.numeric(py.kNN[,2] >= 1/2) #break ties by choosing 1
  return(py.kNN)
}

i2d <-function(i, xtest){
  return(sqrt(sum((X[i,]-xtest)^2)))
}

n2d <- function(ns, xtest){
  v = vector(length=length(ns))
  for(i in 1:length(ns)){
    v[i] = i2d(i, xtest)
  }
  return(v)
}

#test
neighbours = find_kNNs(X, Xtest[3,], Y, 231)
neighbours
nv = LL2vector(neighbours)
nv
n2d(nv, Xtest[3,]) #n2d(find_kNNs(X, Xtest[3,], Y, 5), Xtest[3,])

my.kNN = my.kNN(X, Xtest, Y, 3)
PY.my.kNN = my.kNN[,1:2]
Ypred.my.kNN = my.kNN[,3]



#Here is an impementation using a nearest neighbour search tree package

install.packages("RANN")
require(RANN)
nn2(data=X, query=Xtest, k=3, searchtype = 'priority')

RANN.kNN <-function(x, xtest, y, k){
  py.kNN = matrix(ncol = 3, nrow = nrow(xtest))
  nns = nn2(data=x, query=xtest, k, searchtype = 'priority')$nn.idx
  for(i in 1:nrow(xtest)){
    for(j in 1:min(k, nrow(xtest))){
      nns[i,j] = y[nns[i,j]] #replace the index by its class
    }
    py.kNN[i,2] = sum(nns[i,])/k #probability of class 1
  }
  py.kNN[,1] = 1-py.kNN[,2] #probability of class 0
  py.kNN[,3] = as.numeric(py.kNN[,2] >= 1/2) #break ties by choosing 1
  return(py.kNN)
}

RANN.kNN = RANN.kNN(X, Xtest, Y, 3)
PY.RANN.kNN = RANN.kNN[,1:2]
Ypred.RANN.kNN = RANN.kNN[,3]







# (b)

H <- function(tc,py) { #cross-entropy, tc for true class, py for prediction probabilities
  return(ifelse(tc==0, -log2(py[,1]), -log2(py[,2])))
}

MCE <- function(tc,py) { #mean cross-entropy, tc for true class, py for prediction probabilities
  return(mean(H(tc, py)))
}

#i.
MCE.QDA = MCE(Ytest, PY.QDA)

#ii.
MCE.LDA = MCE(Ytest, PY.LDA)

#iii.
MCE.GNBc = MCE(Ytest, PY.GNBc)

#iv.
MCE.GNBp = MCE(Ytest, PY.GNBp)

#v.
MCE.RDA = MCE(Ytest, PY.RDA)

#vi.
MCE.kNN = MCE(Ytest, PY.kNN)
#expect Inf, since PY.kNN[i,c] can be 0, but Ytest[i] not class c, i.e. a probability of 0 is assigned to the true class, so that H(Ytest[i],PY.kNN[i,]) = -log2(0) = Inf
MCE.my.kNN = MCE(Ytest, PY.my.kNN)
MCE.RANN.kNN = MCE(Ytest, PY.RANN.kNN)


MCEs = c("QDA" = MCE.QDA, "LDA" = MCE.LDA, "GNBc" = MCE.GNBc, "GNBp" = MCE.GNBp, "RDA" = MCE.RDA, "kNN" = MCE.kNN, "my.kNN" = MCE.my.kNN, "RANN.kNN" = MCE.RANN.kNN)
MCEs

# (c)

#i.
accuracy.QDA = sum(Ytest==Ypred.MAP.QDA)/length(Ytest)

#ii.
accuracy.LDA = sum(Ytest==Ypred.MAP.LDA)/length(Ytest)

#iii.
accuracy.GNBc = sum(Ytest==Ypred.MAP.GNBc)/length(Ytest)

#iv.
accuracy.GNBp = sum(Ytest==Ypred.MAP.GNBp)/length(Ytest)

#v.
accuracy.RDA = sum(Ytest==Ypred.MAP.RDA)/length(Ytest)

#vi.
accuracy.kNN = sum(Ytest==Ypred.kNN)/length(Ytest)
accuracy.my.kNN = sum(Ytest==Ypred.my.kNN)/length(Y)
accuracy.RANN.kNN = sum(Ytest==Ypred.RANN.kNN)/length(Y)


accuracy = c("QDA" = accuracy.QDA, "LDA" = accuracy.LDA, "GNBc" = accuracy.GNBc, "GNBp" = accuracy.GNBp, "RDA" = accuracy.RDA, "kNN" = accuracy.kNN, "my.kNN" = accuracy.my.kNN, "RANN.kNN" = accuracy.RANN.kNN)
accuracy

#We want to beat this (the accuracy of the deterministic classifier which always chooses 0):
1-sum(Ytest)/length(Ytest)