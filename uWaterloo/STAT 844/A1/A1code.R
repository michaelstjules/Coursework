# 2. 

myDirectory <- "/Users/Mike/Desktop/CM 764/A1/"
datafile <- "fakePairs.csv"
completePathname <- paste0(myDirectory, datafile)
fakePairs <- read.csv(completePathname)


# a. 
# i. 
cor1 <- cor(fakePairs$x1, fakePairs$y1)
cor2 <- cor(fakePairs$x2, fakePairs$y2)
cor3 <- cor(fakePairs$x3, fakePairs$y3)
cor4 <- cor(fakePairs$x4, fakePairs$y4)

# ii.
#angle brackets don't work???
#fit<1> <- lm(fakePairs$y1~fakePairs$x1)
#fit<2> <- lm(fakePairs$y2~fakePairs$x2)
#fit<3> <- lm(fakePairs$y3~fakePairs$x3)
#fit<4> <- lm(fakePairs$y4~fakePairs$x4)

fit1 <- lm(fakePairs$y1~fakePairs$x1)
fit2 <- lm(fakePairs$y2~fakePairs$x2)
fit3 <- lm(fakePairs$y3~fakePairs$x3)
fit4 <- lm(fakePairs$y4~fakePairs$x4)


summary.lm(fit1)
summary.lm(fit2)
summary.lm(fit3)
summary.lm(fit4)

#plot(fakePairs$x1, fakePairs$y1)


# iii.

#alpha_hat = fiti$coefficients[1], or (Intercept) Estimate
#beta_hat = fiti$coefficients[2], or Fiti$fitted.values Estimate


# iv. 

#(Intercept) Pr(>|t|); if low, evidence against


# v.

#fiti$fitted.values Pr(>|t|); if low, evidence against

# vi.

#Multiple R-squared, https://en.wikipedia.org/wiki/Coefficient_of_determination
# between 0 and 1, where 1 means perfect fit
# "R-squared percent of the variance in the response variable can be explained by the explanatory variables. The remaining thirty percent can be attributed to unknown, lurking variables or inherent variability."

# All R-squared values are roughly the same, 0.6646 to 0.6666, i.e. about 2/3


# b. 






# c.

#attach(mtcars)
par(mfrow=c(2,2))

x1 <- fakePairs$x1
y1 <- fakePairs$y1
plot(x1,y1,col=adjustcolor("black", alpha.f = 0.01))
title("(x1, y1) pairs")
abline(fit1)

x2 <- fakePairs$x2
y2 <- fakePairs$y2
plot(x2,y2,col=adjustcolor("black", alpha.f = 0.01))
title("(x2, y2) pairs")
abline(fit2)

x3 <- fakePairs$x3
y3 <- fakePairs$y3
plot(x3,y3,col=adjustcolor("black", alpha.f = 0.01))
title("(x3, y3) pairs")
abline(fit3)

x4 <- fakePairs$x4
y4 <- fakePairs$y4
plot(x4,y4,col=adjustcolor("black", alpha.f = 0.01))
title("(x4, y4) pairs")
abline(fit4)



# d. 

# For the first 3 models, since the y value is repeated for clustered values of x, 
# it's difficult to attribute any deviation from the line (the residuals) to mean 0 errors, and
# if there's a true model, it's definitely not a line (or more independent variates are necessary). 
# Piecewise linear functions connecting all of the points actually seem like better guesses, 
# although we can't say what should happen in the gaps.

# (x1,y1) looks relatively linear, but the residuals may actually indicate some wavy function (as in my general comment above)

# (x2, y2) looks like a quadratic, rather than a line, but because the slope never gets very steep,
# it's not surprising that a line fits relatively well, with R-squared so high. 


# (x3,y3) the second last cluster, the one with the greatest y3 value, seems like an outlier, but
# but because it's a cluster and not a single point, it seems unlikely this can be attributed to random noise.
# Two possibilities come to mind: it could be a "systematic error" or the y3 values could really be that way.
# Ignoring that cluster, however, a straight line model seems like it would be a perfect fit.
# I would have expected R-squared to be larger than ~2/3. 

# (x4,y4) looks like a line could be the "least wrong" for it if we assume that intermediate x4 values are possible, 
# but only because the only x4 values are 8 and 19, so a line is pretty useless with such a large gap. 
# Basically all of the variance in y4 happens at x4=8, and it's not hard to fit a line to two points. 




















# 3.

myDirectory <- "/Users/Mike/Desktop/CM 764/A1/"
datafile <- "Advertising.csv"
completePathname <- paste0(myDirectory, datafile)
Advertising <- read.csv(completePathname)

# a.

plot(Advertising$TV, Advertising$Sales)

# i.

fit1 <- lm(Advertising$Sales~Advertising$TV)

# ii.

abline(fit1)


# iii.

Xorder <- order(Advertising$TV)

pred <- predict(fit1, interval = c("prediction"), level = 0.95)

lines(Advertising$TV[Xorder],pred[Xorder,2])
lines(Advertising$TV[Xorder],pred[Xorder,3])


# b.

# i.

# Divide each variate by k(x_i)


# iv. 

#prediction interval lines are thick?? duplicated?



#w_i = x_i; need to give the inverse to predict?
plot(Advertising$TV, Advertising$Sales)
fit2 <- lm(Advertising$Sales~Advertising$TV, weights=Advertising$TV)
abline(fit2)
pred2 <- predict(fit2, interval = c("prediction"), level = 0.95, weights=Advertising$TV)
lines(Advertising$TV[Xorder], pred2[Xorder,2], lty=2)
lines(Advertising$TV[Xorder], pred2[Xorder,3], lty=2)

#This makes sense: we gave greater weight to larger x_i, so their prediction intervals should be smaller


# w_i = 1/x_i; need to give the inverse of this to predict?
plot(Advertising$TV, Advertising$Sales)
fit3 <- lm(Advertising$Sales~Advertising$TV, weights=1./Advertising$TV)
abline(fit3)
pred3 <- predict(fit3, interval = c("prediction"), level = 0.95, weights=1./Advertising$TV)
lines(Advertising$TV[Xorder], pred3[Xorder,2], lty=2)
lines(Advertising$TV[Xorder], pred3[Xorder,3], lty=2)

#This makes sense: we gave greater weight to smaller x_i, so their prediction intervals should be smaller


# c.

# i.

fit1 <- lm(Advertising$Sales~Advertising$TV)
ab_res1 <- abs(fit1$residuals)

# ii.
plot(fit1$fitted.values,ab_res1)

# iii.
sfit <- lm(ab_res1~fit1$fitted.values)
abline(sfit)

# iv.

fit2 <- lm(Advertising$Sales~Advertising$TV, weights=1/sfit$fitted.values^2)
plot(Advertising$TV, Advertising$Sales)
abline(fit2)
pred2 <- predict(fit2, interval = c("prediction"), level = 0.95, weights=1/sfit$fitted.values^2)
lines(Advertising$TV[Xorder], pred2[Xorder,2], lty=2)
lines(Advertising$TV[Xorder], pred2[Xorder,3], lty=2)







#Repeat this until convergence??

fit1 <- fit2
ab_res1 <- abs(fit1$residuals)
plot(fit1$fitted.values,ab_res1)
sfit <- lm(ab_res1~fit1$fitted.values)
abline(sfit)
fit2 <- lm(Advertising$Sales~Advertising$TV, weights=1/sfit$fitted.values^2)
plot(Advertising$TV, Advertising$Sales)
abline(fit2)
pred2 <- predict(fit2, interval = c("prediction"), level = 0.95, weights=1/sfit$fitted.values^2)
lines(Advertising$TV[Xorder], pred2[Xorder,2], lty=2)
lines(Advertising$TV[Xorder], pred2[Xorder,3], lty=2)





#d.

