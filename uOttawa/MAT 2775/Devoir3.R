#(a)

x = rnorm(60, 10, 5)
summary(x)
mu = mean(x)
sd = sd(x) #n-1 comme dénominateur
print(mu)
print(sd)


#(b)

X = matrix(rnorm(500*70, 10, 5), ncol = 70, nrow = 500)
L = rep(0,500); U = rep(0,500)
L.U_contient_mu = rep(0,500)
for(i in 1:500){
	L[i] = mean(X[i,]) - qnorm(0.975,0,1)*5/sqrt(70) #0.975 = 0.95+0.25, P(X>z_0.025) = 0.025 et P(X<=z_0.025) = 0.975
	U[i] = mean(X[i,]) + qnorm(0.975,0,1)*5/sqrt(70)
}
v = which(!(L<10 & 10<U)) #les intervalles qui ne contiennent pas mu=10
print(v)
m = sum(L<10 & 10<U)
coverage.prob = sum(m)/500
print(coverage.prob*100) #pourcentage des intervalles qui contiennent mu=10


#(c)

X2 = matrix(rnorm(500*70, 10, 5), ncol = 70, nrow = 500)
L2 = rep(0,500); U2 = rep(0,500)
L2.U2_contient_mu = rep(0,500)
for(i in 1:500){
	L2[i] = mean(X2[i,]) - qt(0.975,df=69)*sd(X2[i,])/sqrt(70) 
	U2[i] = mean(X2[i,]) + qt(0.975,df=69)*sd(X2[i,])/sqrt(70)
}
v2 = which(!(L2<10 & 10<U2)) #les intervalles qui ne contiennent pas mu=10
print(v2)
m2 = sum(L2<10 & 10<U2)
coverage.prob2 = sum(m2)/500
print(coverage.prob2*100) #pourcentage des intervalles qui contiennent mu=10




#L.U_contient_mu = as.numeric(L<10 & 10<U) #0 si l'intervalle ne contient pas mu et 1, si l'intervalle contient mu
#print(L.U_contient_mu)