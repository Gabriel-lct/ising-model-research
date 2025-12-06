import numpy as np
from numba import njit
import matplotlib.pyplot as plt

# ISING:
def energy(conf, J, B):
    N = conf.shape[0]
    E = 0
    for x in range(N):
        for y in range(N):
            for z in range(N):
                E -= J*conf[x,y,z]*conf[(x+1)%N,y, z]
                E -= J*conf[x,y,z]*conf[x,(y+1)%N, z]
                E -= J*conf[x,y,z]*conf[x, y, (z+1)%N]
                E += B*conf[x,y,z]
    return E

@njit
def calc_dE(conf, x, y, z, J, B):
    N = conf.shape[0]
    dE = 0
    dE += conf[(x-1)%N, y, z]+conf[(x+1)%N, y, z]+conf[x,(y-1)%N, z]+conf[x,(y+1)%N, z] + conf[x, y, (z-1)%N] + conf[x, y, (z+1)%N]
    dE *= 2*J
    dE -= 2*B
    dE *= conf[x,y,z]
    return dE

@njit
def step_metropolis(conf, J, B, T, speed, E):
    N = conf.shape[0]
    for _ in range(int(speed*N*N)):
        # A site is chosen at random
        x = np.random.randint(0, N)
        y = np.random.randint(0, N)
        z = np.random.randint(0, N)

        # Compute the change in energy if we flip that site
        dE = calc_dE(conf, x, y, z, J, B)
        # Transition probability
        p = np.exp(-dE/T)
        # If the energy decreases, the site flips
        if dE <= 0:
            conf[x,y,z] = -conf[x,y,z]
        # If the energy increases, the site flips with probability p.
        elif np.random.rand() <= p:
                conf[x,y,z] = -conf[x,y,z]
    return conf, E+dE

def show_conf(conf):
    voxels = np.ones_like(conf, dtype=bool)
    # Map conf to colors
    colors = np.empty(conf.shape, dtype=object)
    colors[conf == 1] = 'red'
    colors[conf == -1] = 'blue'

    fig = plt.figure(figsize=(8, 8))
    ax = fig.add_subplot(111, projection='3d')

    ax.voxels(voxels, facecolors=colors, edgecolor='k')

    ax.set_xlabel('x')
    ax.set_ylabel('y')
    ax.set_zlabel('z')
    plt.show()

def run_metropolis(N, J, B, T, it, show=False, conf=None):
    N = 20
    it = 1000
    J = 1
    B = 0
    T = 1
    Es = np.zeros(it)
    if not conf:
        conf = np.random.choice([-1, 1], (N,N,N))
    E = energy(conf, J, B)
    for i in range(it):
        Es[i] = E
        conf, E = step_metropolis(conf, J, B, T, 1, E)

    if show:
        show_conf(conf)
    
    return Es


Es = run_metropolis(20, 1, 0, 1, 1000, True)

# plt.scatter(np.arange(1000), Es)
# plt.show()