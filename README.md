# Ising Model Research

A computational physics project exploring different implementations and variations of the Ising model for studying phase transitions and critical phenomena in statistical mechanics.

## Overview

The Ising model is a mathematical model in statistical mechanics that describes ferromagnetism. This project implements various versions of the Ising model to study:

- **Phase transitions** between ordered (ferromagnetic) and disordered (paramagnetic) states
- **Critical temperature** and critical exponents
- **Magnetization dynamics** and equilibrium properties
- **Monte Carlo simulations** using Metropolis algorithm
- **Finite-size scaling** effects

## Project Structure

```
ising-model-research/
├── README.md
├── requirements.txt
├── python/
│   ├── main.py              # Main simulation runner
│   ├── utils.py             # Utility functions and analysis tools
│   └── notebooks/           # Jupyter notebooks for visualization and analysis
└── cpp/                     # C++ implementations for performance-critical simulations
```

## Features

### Ising Model Variations

- **1D Ising Model**: One-dimensional chain with nearest-neighbor interactions
- **2D Ising Model**: Two-dimensional square lattice (classical model)

### Algorithms

- **Metropolis-Hastings Algorithm**: Standard Monte Carlo sampling

### Analysis Tools

- Magnetization and susceptibility calculations
- Energy and specific heat measurements
- Correlation functions and correlation length
- Autocorrelation time analysis
- Critical exponent estimation
- Finite-size scaling analysis

## Installation

### Requirements

- Python 3.8+
- NumPy
- Matplotlib
- SciPy

### Setup

1. Clone the repository:

```bash
git clone <repository-url>
cd ising-model-research
```

2. Create a virtual environment (recommended):

```bash
python -m venv venv
source venv/bin/activate  # On macOS/Linux
```

3. Install dependencies:

```bash
pip install -r requirements.txt
```

_Last updated: November 2025_
