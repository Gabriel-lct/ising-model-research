import numpy as np
from numba import njit
import matplotlib.pyplot as plt
import pygame
import tkinter as tk

# ISING:
def energy(conf, J, B):
    X, Y = conf.shape
    E = 0
    for x in range(X):
        for y in range(Y):
            E -= J*conf[x,y]*conf[(x+1)%X,y]
            E -= J*conf[x,y]*conf[x,(y+1)%Y]
            E += B*conf[x,y]
    return E

def calculate_energy(sudoku):
  # size of sudoku and inside-square
  N = len(sudoku)
  n = int(np.sqrt(N))

  # init energy at level 0
  E = 0

  for i in range(N):
    line = sudoku[i, :]
    col = sudoku[:, i]
    r, c = divmod(i, n)
    block = sudoku[r*n:(r+1)*n, c*n:(c+1)*n]
    
    _, line_count = np.unique(line, return_counts=True)
    _, col_count = np.unique(col, return_counts=True)
    _, block_count = np.unique(block, return_counts=True)

    E += np.sum(line_count - 1) + np.sum(col_count - 1) + np.sum(block_count - 1)
  
  return E

def calc_dE(conf, x, y, J, old, new):
    

@njit
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

def init_sudoku(n):
  N = n**2
  sudoku = np.random.randint(1, N+1, size=(N,N))
  return sudoku

# PYGAME:
def draw_state(screen, state):
    colors = (state+1)//2
    colors = colors.astype(np.uint8) * 255
    surf = pygame.surfarray.make_surface(
        np.repeat(np.repeat(colors, CELL_SIZE, axis=0), CELL_SIZE, axis=1)
    )
    screen.blit(surf, (0, 0))
    pygame.display.flip()

# CONSTANTS
J = 1
B = 0
T = 2.3
speed = 1
N = 400
conf = init_sudoku(9)
W = 800
CELL_SIZE = W//N

# TKINTER STUFF

root = tk.Tk()
root.title("Controls")

T_var = tk.DoubleVar(value=T)
B_var = tk.DoubleVar(value=B)
J_var = tk.DoubleVar(value=J)
S_var = tk.DoubleVar(value=speed)
paused_var = tk.BooleanVar(value=False)

tk.Label(root, text="Temperature T").pack()
tk.Scale(root, from_=0.1, to=5.0, resolution=0.01, orient="horizontal", variable=T_var).pack()
tk.Label(root, text="Magnetic Field B").pack()
tk.Scale(root, from_=-2.0, to=2.0, resolution=0.01, orient="horizontal", variable=B_var).pack()
tk.Label(root, text="Coupling Constant J").pack()
tk.Scale(root, from_=-2.0, to=2.0, resolution=0.01, orient="horizontal", variable=J_var).pack()
tk.Label(root, text="Speed").pack()
tk.Scale(root, from_=0.0, to=2.0, resolution=0.01, orient="horizontal", variable=S_var).pack()
tk.Button(root, text="Pause/Resume", command=lambda: paused_var.set(not paused_var.get())).pack()

pygame.init()
screen = pygame.display.set_mode((W, W))
pygame.display.set_caption("Sudoku")
clock = pygame.time.Clock()

def loop():
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            root.destroy()
            pygame.quit()
            return
    T = T_var.get()
    B = B_var.get()
    J = J_var.get()
    speed = S_var.get()
    if not paused_var.get():

        step_metropolis(conf, J, B, T, speed)

        draw_state(screen, conf)
    clock.tick(60)

    root.after(1, loop)

root.after(1, loop)
root.mainloop()