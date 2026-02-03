import numpy as np
import matplotlib.pyplot as plt

def Z(N, beta, J, B):
    racine = np.sqrt(np.exp(2*beta*J)*np.sinh(beta*B)**2 + np.exp(-2*beta*J))
    l1 = np.exp(beta*J)*np.cosh(beta*B) + racine
    l2 = np.exp(beta*J)*np.cosh(beta*B) - racine
    return l1**N + l2**N

B = 0.1
J = 1
N = 50
betas = np.linspace(0.1, 10, 1000)

#plt.xscale('function', functions=(np.tanh, np.atanh))
#plt.yscale('log')
plt.xscale('log')

plt.plot(betas, -np.log(Z(N, betas, J, B))/betas)
plt.xlabel("$\\beta$ $(K^{-1})$")
plt.ylabel("$F$")
plt.savefig("../figures/F_beta_1d.png")
plt.show()