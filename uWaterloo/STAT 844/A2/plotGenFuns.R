elementTimes  <- function(mat, vec, byCol = FALSE) {
  ## element wise multiplication
  if (byCol) t(t(mat) * vec) else mat * vec
}

plotGenFuns <- function(x, mat, vec, cols=NULL) {
  if(ncol(mat) != length(vec)) stop("mat and vec don't match")
  xOrder <- order(x)
  xx <- x[xOrder]
  mmat <- mat[xOrder, ]
  nfuns <- length(vec)
  ltys <- 1:nfuns
  if(is.null(cols))   cols <- rainbow(nfuns, s=0.75, alpha=0.5)
  for (i in 1:nfuns){
    if (i ==1) {
      plot(xx, sign(vec[i]) * mmat[, i], col=cols[i], lty=ltys[i], lwd=3,
           type="l",
           main="Generating functions", xlab="x", ylab="g(x)",
           ylim=extendrange(elementTimes(mat, sign(vec), byCol=TRUE))
      )
    } else
    {
      lines(xx, sign(vec[i]) * mmat[, i], col=cols[i],
            lty=ltys[i], lwd=3)
    }
  }
  ## Now for the weight applied to each function
  ##
  c_ylim <- c(0, round(1.05 * max(abs(vec)), 4))
  plot(abs(vec), pch=19, col=cols, type="b", ylim=c_ylim,
       xlab="generating function number", ylab="absolute weight",
       main="Weight for each generating function")
  ##
  ## Putting them both together
  for (i in 1:nfuns){
    if (i ==1) {
      sumFit <- vec[i] * mmat[, i]
    }
    else {
      sumFit <- sumFit + vec[i] * mmat[, i]
    }
  }
  for (i in 1:nfuns){
    if (i ==1) {
      plot(xx, vec[i] * mmat[, i], col=cols[i], lty=ltys[i], lwd=3,
           type="l",
           main="Generating functions to be summed",
           xlab="x", ylab="g(x)",
           ylim=extendrange(c(sumFit,
                              elementTimes(mat, vec, byCol=TRUE))
           )
      )
    } else
    {
      lines(xx, vec[i] * mmat[, i], col=cols[i], lty=ltys[i], lwd=3)
    }
    ## Overlay the sum of the fit
    lines(xx, sumFit, col="darkgrey", lty=1, lwd=2)
    legend("topleft", bty="n",
           legend=c("sum"),
           lty=c(1),
           lwd=c(2),
           col="darkgrey")
  }
}