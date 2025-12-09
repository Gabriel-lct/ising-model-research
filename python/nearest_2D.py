import numpy as np
import matplotlib.pyplot as plt
from numba import njit

# J > 0: Ferromagnetic, J < 0: Antiferromagnetic
# Takes a configuration and the coefficient J and computes the energy.
@njit
def energy(conf, J, B):
    X, Y = conf.shape
    E = 0
    for x in range(X):
        for y in range(Y):
            E += J*conf[x,y]*conf[(x+1)%X,y]
            E += J*conf[x,y]*conf[x,(y+1)%Y]
            E += B*conf[x,y]
    return E

# Computes the variation in energy for a spin flip given the configuration
# and the coordinates of the flipping site.
@njit
def calc_dE(conf, x, y, J, B):
    N = conf.shape[0]
    dE = 0
    neighbours = conf[(x-1)%N,y]+conf[(x+1)%N,y]+conf[x,(y-1)%N]+conf[x,(y+1)%N]
    dE = 2 * J * neighbours * conf[x,y]
    # dE *= 2*J
    # dE -= 2*B
    # dE *= conf[x,y]
    
    return dE

def step_metropolis(conf, J, B, T, speed):
    N = conf.shape[0]
    for _ in range(int(speed*N*N)):
        # A site is chosen at random
        x = np.random.randint(0, N)
        y = np.random.randint(0, N)

        # Compute the change in energy if we flip that site
        dE = calc_dE(conf, x, y, J, B)
        # Transition probability
        p = np.exp(-dE/T)
        # If the energy decreases, the site flips
        if dE <= 0:
            conf[x,y] = -conf[x,y]
        # If the energy increases, the site flips with probability p.
        elif np.random.rand() <= p:
                conf[x,y] = -conf[x,y]
    return conf

@njit
def run_metropolis(conf, N, J, B, T, it, show=False):
    E = energy(conf, J, B)
    Es = np.zeros(it)
    for i in range(it):
        # A site is chosen at random
        x = np.random.randint(0, N)
        y = np.random.randint(0, N)

        # Compute the change in energy if we flip that site
        dE = calc_dE(conf, x, y, J, B)
        # Transition probability
        p = np.exp(-dE/T)
        # If the energy decreases, the site flips
        if dE <= 0:
            conf[x,y] = -conf[x,y]
            E += dE
        # If the energy increases, the site flips with probability p.
        elif np.random.rand() <= p:
                conf[x,y] = -conf[x,y]
                E += dE
        Es[i] = E
    return Es

def C(T, N, J, B):
    EE = 0
    EE2 = 0
    X = 100000
    Es = run_metropolis(np.random.choice([-1, 1], (N,N)), N, J, B, T, 2000000)
    EE = sum(Es[-X:])
    EE2 = sum(Es[-X:]**2)

    EE /= X
    EE2 /= X
    #print(Es)
    Cv = (EE2 - (EE**2))/(T**2*N*N)

    #return (np.mean(Es2)-np.mean(Es)**2)/(T**2*N*N)
    return Cv

J = 1
Ts = np.linspace(0.1, 10, 15000)
Ct = [C(T, 30, J, 0) for T in Ts]
plt.scatter(Ts, Ct, marker='.')
plt.axvline(x=2*J/np.log(1+np.sqrt(2)))
# plt.xscale('log')
plt.yscale('log')
plt.show()

# it = np.arange(70000)
# Es = run_metropolis(np.random.choice([-1, 1], (200,200)), 200, -1, 0, 1.5, 70000)
# plt.plot(it, Es)
# plt.show()