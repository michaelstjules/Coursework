#I

x = runif(50, 0, 10) #1
e = rnorm(50, 0, 2.9) #2
y = 1 + 3*x + e #3
model = lm(y~x) #4
a1 = model$coef[1]
b = model$coef[2]
a1 + b*2.5 #5
plot(x,y) #6
abline(a1,b)


#II

a1 = rep(NA, 200) #0
b = rep(NA, 200) 
x = runif(50, 0, 10) #1
for(i in 1:200){
	e = rnorm(50, 0, 2.9)
	y = 1 + 3*x + e
	model = lm(y~x)
	a1[i] = model$coef[1]
	b[i] = model$coef[2]
}
mean(a1) #3
mean(b)

#3 What theoretical result can be justified using these values?
# La moyenne des valeurs d' a1 estimées et la moyenne des valeurs de b estimées sont proches de 1 et 3, les valeurs réelles d' a1 et de b. Le résultat théorique justifié par ces moyennes est que les estimateurs du maximum de vraisemblance pour a, a1 et b sont non-biasées: leurs moyennes sont les valeurs réelles d' a, d' a1 et de b. 

old.par <- par(mfrow=c(1, 2)) #4
hist(a1)
qqnorm(a1)
par(old.par)

#4 What theoretical result can be justified using these graphs?
# L'histogramme approxime une loi normale et le qq-plot semble linéaire. Ces graphiques justifient que l'e.m.v. pour a1 suit une loi normale avec moyenne a1 (mais que la variance est 1/n fois la variance des e n'est pas evident).