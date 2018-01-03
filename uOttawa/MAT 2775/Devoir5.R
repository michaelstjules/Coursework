#8.5-4 (d)
mu = seq(80, 90, 0.01)
k = 1-pnorm((83-mu)/2)
matplot(mu, k, type = "p")

#X = scan("")