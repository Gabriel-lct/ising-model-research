# Parallélisation avec OpenMP

## Configuration

Le code `sudoku_transition` est maintenant parallélisé avec OpenMP pour utiliser tous les cœurs disponibles de votre CPU.

## Contrôler le nombre de threads

Par défaut, OpenMP utilise tous les cœurs disponibles. Pour contrôler le nombre de threads :

```bash
# Utiliser tous les cœurs (défaut)
./main

# Utiliser 4 threads
export OMP_NUM_THREADS=4
./main

# Utiliser 8 threads
export OMP_NUM_THREADS=8
./main
```

## Voir le nombre de cœurs disponibles

```bash
# Sur macOS
sysctl -n hw.ncpu

# Définir automatiquement au maximum
export OMP_NUM_THREADS=$(sysctl -n hw.ncpu)
```

## Comment ça fonctionne

La boucle sur les températures est parallélisée avec `#pragma omp parallel for`. Chaque température est calculée indépendamment sur un thread différent, ce qui accélère considérablement les calculs.

## Performance

Avec N threads, vous devriez obtenir une accélération proche de N fois (pour nb_inter_T >= N).
