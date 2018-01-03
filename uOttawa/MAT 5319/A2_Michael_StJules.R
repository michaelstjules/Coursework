#3.(a)

# === a) Read in the data, break it into "train" and "test" subsets, 
#   and create data matrices X and Y for each:

# The following line may be typed, or a similar one produced in RStudio by clicking "Import Dataset":
coris = read.csv("coris.dat")

# Discard first column (subject ID), column "famhist" (binary)
coris[["row.names"]] = NULL
coris[["famhist"]] = NULL

# Convert to matrix. 
data = data.matrix(coris)
# dim(data)  # returns "[1] 464  11". 
data = data[3:464,]   # Discard first two rows (they were explanatory text, before matrix conversion)
data = data[sample(nrow(data)),]  # randomise order of rows
N = nrow(data)
M = ncol(data)
# Finished data loading, cleaning and randomising


traindata = data[1:floor(N/2),]

testdata = data[(floor(N/2)+1):N,]

X = traindata[,1:M-1]
Y = traindata[,M]

Xtest = testdata[,1:M-1]
Ytest = testdata[,M]




#(b)

Xmean = colMeans(X)
Xcov = cov(X)




#(c)


X0 = X[Y==0,]
Xmean0 = colMeans(X0)
Xcov0 = cov(X0)


X1 = X[Y==1,]
Xmean1 = colMeans(X1)
Xcov1 = cov(X1)

# Sanity check
for(i in 1:M-1){
  print((Xmean0[i] <= Xmean[i] & Xmean[i] <= Xmean1[i]) | (Xmean1[i] <= Xmean[i] & Xmean[i] <= Xmean0[i]))
}



#(d)

require(mvtnorm)
likeli0=dmvnorm(Xtest,mean=Xmean0,sigma=Xcov0)
#probability density for X = Xtest[i,] for each i, where X ~ N(Xmean0, Xcov0)
likeli1=dmvnorm(Xtest,mean=Xmean1,sigma=Xcov1)

Ypred_MLE = vector(length = length(Ytest))

for(i in 1:length(Ypred_MLE)){
  if(likeli1[i] >= likeli0[i]){Ypred_MLE[i]=1}
  else {Ypred_MLE[i]=0}
}




#(e)

p0 = sum(Y==0)/length(Ypred_MLE) #p(Y=0)
p1 = sum(Y==1)/length(Ypred_MLE) #p(Y=1)

Ypred_MAP = vector(length = length(Ytest))

for(i in 1:length(Ypred_MAP)){
  if(likeli1[i]*p1 >= likeli0[i]*p0){Ypred_MAP[i]=1}
  else {Ypred_MAP[i]=0}
}




#(f)

#install.packages("gmodels")
require(gmodels)

CrossTable(Ytest, Ypred_MLE, prop.chisq=FALSE)
pMLE = sum(Ypred_MLE==Ytest)/length(Ytest)
#The prediction accuracy of the MLE method is#
pMLE
#This is also the sum of the bottom values of the 0,0 and 1,1 entries of the table

CrossTable(Ytest, Ypred_MAP, prop.chisq=FALSE)
pMAP = sum(Ypred_MAP==Ytest)/length(Ytest)
#The prediction accuracy of the MAP method is
pMAP
#This is also the sum of the bottom values of the 0,0 and 1,1 entries of the table

#To compare these the accuracies directly:
pMLE
pMAP
