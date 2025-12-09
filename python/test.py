import numpy as np
import matplotlib.pyplot as plt
import time
from numba import njit

def initial_state(N):
    """Generates a random spin configuration for initial condition."""
    return 2 * np.random.randint(2, size=(N, N)) - 1

def mcmc_move(config, beta):
    """
    Performs one Monte Carlo step (N*N attempted flips) 
    using the Metropolis algorithm.
    """
    N = config.shape[0]
    for _ in range(N * N):
        # Pick a random site
        a = np.random.randint(0, N)
        b = np.random.randint(0, N)
        s = config[a, b]
        
        # Calculate energy change if we flip this spin
        # Periodic boundary conditions using modulo operator %
        neighbors = config[(a+1)%N, b] + config[(a-1)%N, b] + \
                   config[a, (b+1)%N] + config[a, (b-1)%N]
        
        dE = 2 * s * neighbors
        
        # Metropolis acceptance criterion
        if dE < 0 or np.random.rand() < np.exp(-dE * beta):
            s *= -1
            config[a, b] = s
            
    return config

def calculate_energy(config):
    """Computes the total energy of the given configuration."""
    N = config.shape[0]
    energy = 0
    for i in range(N):
        for j in range(N):
            S = config[i, j]
            neighbors = config[(i+1)%N, j] + config[(i, (j+1)%N)]
            energy += -S * neighbors
    return energy

def simulate_ising(N, temp_range, steps_equil, steps_meas):
    E_means = []
    E_squared_means = []
    Cv_list = []
    
    print(f"Simulating {N}x{N} lattice over {len(temp_range)} temperature points...")
    
    # Initialize lattice
    config = initial_state(N)
    
    for T in temp_range:
        beta = 1.0 / T
        
        # 1. Equilibration (Burn-in phase)
        # We let the system settle into the new temperature
        for _ in range(steps_equil):
            mcmc_move(config, beta)
            
        # 2. Measurement phase
        E_accum = 0
        E_sq_accum = 0
        
        for _ in range(steps_meas):
            mcmc_move(config, beta)
            E = calculate_energy(config)
            E_accum += E
            E_sq_accum += E**2
            
        # Averages
        E_avg = E_accum / steps_meas
        E_sq_avg = E_sq_accum / steps_meas
        
        # Calculate Specific Heat (Cv) using Fluctuation-Dissipation Theorem
        # Cv = ( <E^2> - <E>^2 ) / (N^2 * T^2)
        # Note: We divide by N*N to get specific heat *per site*
        var_E = E_sq_avg - (E_avg**2)
        cv = var_E / ((N * N) * (T ** 2))
        
        Cv_list.append(cv)
        print(f"T = {T:.2f} | Cv = {cv:.4f}")

    return Cv_list

# --- Parameters ---
N_sites = 16            # Lattice size (NxN). Larger = sharper peak, but slower.
steps_equil = 1000      # Steps to wait before measuring
steps_meas = 1000       # Steps to measure for averaging
temperatures = np.linspace(1.5, 3.5, 25) # Range covering the critical temp (~2.27)

# --- Run Simulation ---
start_time = time.time()
specific_heats = simulate_ising(N_sites, temperatures, steps_equil, steps_meas)
print(f"Simulation completed in {time.time() - start_time:.2f} seconds.")

# --- Plotting ---
plt.figure(figsize=(10, 6))
plt.plot(temperatures, specific_heats, 'o-', color='crimson', label='Specific Heat ($C_v$)')
plt.axvline(x=2.269, color='grey', linestyle='--', label='Critical Temp ($T_c \\approx 2.27$)')
plt.title(f'Specific Heat per Site vs Temperature (2D Ising Model, {N_sites}x{N_sites})')
plt.xlabel('Temperature ($T$)')
plt.ylabel('Specific Heat ($C_v$)')
plt.grid(True, which='both', linestyle='--', alpha=0.7)
plt.legend()
plt.show()