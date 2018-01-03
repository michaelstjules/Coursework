#7.6-20

x = c(32,23,23,23,26,30,17,29,17,18,26,16,21,24,30)
y = c(28,25,24,32,31,27,23,30,18,18,32,22,28,31,26)
n=15
sxx = var(x)
sxy = cov(x,y)
hat.alpha = mean(y)
hat.beta = sxy/sxx
y.fitted = hat.alpha + hat.beta*(x-mean(x))
hat.sigma.squared = sum((y-y.fitted)^2)/n
somme = sxx*(n-1)
alpha.L = hat.alpha - qt(.975, df=n-2)*sqrt(hat.sigma.squared/(n-2))
alpha.U = hat.alpha + qt(.975, df=n-2)*sqrt(hat.sigma.squared/(n-2))
alpha.interval = c(alpha.L, alpha.U)
alpha.interval
beta.L = hat.beta - qt(.975, df=n-2)*sqrt(n*hat.sigma.squared/((n-2)*somme))
beta.U = hat.beta + qt(.975, df=n-2)*sqrt(n*hat.sigma.squared/((n-2)*somme))
beta.interval = c(beta.L, beta.U)
beta.interval

#8.1-12

a = c(265,272,246,260,274,263,255,258,276,274,274,269,244,212,235,254,224)
b = c(252,276,243,246,275,246,244,245,259,260,267,267,251,222,235,255,231)
d = a-b
n=17
#t est
d.bar = mean(d)
sd = sd(d)
t = mean(d)/(sd(d)/sqrt(n))
t
#la valeur-p est
1-pt(t, df=n-1)
#et donc on rejette H_0 au niveau alpha = 0.05
#t_{0.05}(n-1) est
qt(.95, df=n-1)
#et donc t > t_{0.05}(n-1) (et on rejette H_0 au niveau alpha = 0.05)

#8.2-10
x = c(5.04, 4.52, 6.18,7.01,4.36,1.81,6.22,5.7)
y = c(4.67,4.21,2.18,3.91,4.09,5.24,2.94,4.71,4.04,5.79,3.8,4.38)
sd(x)^2
sd(y)^2

#http://www.wolframalpha.com/input/?i=((2.591486)%2F8+%2B+(0.914206)%2F12)%5E2%2F(1%2F7*(2.591486%2F8)%5E2%2B1%2F11(0.914206%2F12)%5E2)
n = 8
m=12
r = ((sd(x)^2)/n+(sd(y)^2)/m)^2/((1/(n-1))*((sd(x)^2)/n)^2+(1/(m-1))*((sd(y)^2)/m)^2)