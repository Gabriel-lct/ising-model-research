import numpy as np
import matplotlib.pyplot as plt
from numba import njit
import os

# J > 0: Ferromagnetic, J < 0: Antiferromagnetic
# Takes a configuration and the coefficient J and computes the energy.
@njit
def energy(conf, J, B):
    X, Y = conf.shape
    E = 0
    for x in range(X):
        for y in range(Y):
            E -= J*conf[x,y]*conf[(x+1)%X,y]
            E -= J*conf[x,y]*conf[x,(y+1)%Y]
            E -= B*conf[x,y]
    return E

# Computes the variation in energy for a spin flip given the configuration
# and the coordinates of the flipping site.
@njit
def calc_dE(conf, x, y, J, B):
    X, Y = conf.shape
    dE = 0
    dE += conf[(x-1)%X,y]+conf[(x+1)%X,y]+conf[x,(y-1)%Y]+conf[x,(y+1)%Y]
    dE *= 2*J
    dE -= 2*B
    dE *= conf[x,y]
    
    return dE

@njit
def step_metropolis(conf, J, B, T, speed):
    N = conf.shape[0]
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
    else:
        dE = 0
    return conf, dE

@njit
def run_metropolis(conf, N, J, B, T, it, samples, show=False):
    E = energy(conf, J, B)
    Es = []
    confs = []
    for i in range(it):
        conf, dE = step_metropolis(conf, J, B, T, 1/(N*N))
        E += dE
        Es.append(E)
        if i%(it//(samples-1))==0:
            confs.append(conf.copy())
    confs.append(conf.copy())
    return Es, confs

@njit
def aimantation(conf, T, N, J, B):
    Es, confs = run_metropolis(conf, N, J, B, T, N*10000, N*10000)
    confs = confs[-1000:]
    M = sum([sum([sum(line) for line in conf]) for conf in confs])/1000
    m = M/(N*N)
    return m

# conf = np.random.choice([-1, 1], (100,100))
# print(aimantation(conf, 0.1, 100, 1, 0))

# def C(T, N, J, B):
#     EE = 0
#     EE2 = 0
#     X = 100000
#     Es = run_metropolis(np.random.choice([-1, 1], (N,N)), N, J, B, T, 2000000)
#     EE = sum(Es[-X:])
#     EE2 = sum(Es[-X:]**2)

#     EE /= X
#     EE2 /= X
#     #print(Es)
#     Cv = (EE2 - (EE**2))/(T**2*N*N)

#     #return (np.mean(Es2)-np.mean(Es)**2)/(T**2*N*N)
#     return Cv

def min_en(N, J, B):
    conf = np.ones((N,N))
    return energy(conf, J, B)

# # J = 1
# # Ts = np.linspace(0.1, 10, 15000)
# # Ct = [C(T, 30, J, 0) for T in Ts]
# # plt.scatter(Ts, Ct, marker='.')
# # plt.axvline(x=2*J/np.log(1+np.sqrt(2)))
# # # plt.xscale('log')
# # plt.yscale('log')
# # plt.show()

N = 200
J = 1
B = 0
T = 3
n = 5000000
samples = 2

it = np.arange(n)
# Es = run_metropolis(np.random.choice([-1, 1], (200,200)), 200, -1, 0, 1.5, 70000)

enmin = min_en(N, J, B)/(N*N)

directory = f"../figures/metropolis/evolution/{N}_{J}_{B}_{T}"

if not os.path.exists(directory):
    os.makedirs(directory)

Es, confs = run_metropolis(np.random.choice([-1, 1], (N,N)), N, J, B, T, n, samples, show=True)
ts = np.arange(samples)*(n//(samples-1))
Es = np.array(Es)/(N*N)
plt.plot(it, Es, label="Énergie par site")
plt.plot(it, np.ones_like(it)*enmin, color='red', linestyle='dashed',label="Énergie minimale")
plt.xlabel("Itération")
plt.ylabel("Énergie")
plt.legend()
plt.tight_layout()
plt.savefig(directory+f"/energy_{N}_{J}_{B}_{T}.png")
#plt.show()
plt.clf()
plt.close()
for i in range(samples):
    print(ts[i])
    plt.imshow(confs[i], cmap='bwr', interpolation='none')
    #plt.title(f"État du système après {ts[i]} itérations")
    plt.axis('off')
    plt.savefig(directory+f"/it_{ts[i]}_{N}_{J}_{B}_{T}.png", bbox_inches='tight', pad_inches=0)
    #plt.show()
    plt.clf()
    plt.close()

print(Es[-1])
print(enmin)