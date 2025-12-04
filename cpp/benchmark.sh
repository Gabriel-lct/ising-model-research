#!/bin/bash

echo "=== Test de performance avec votre code sudoku ==="
echo "Nombre de cœurs: $(sysctl -n hw.ncpu)"
echo ""

# Test avec 1 thread
echo "Test SÉQUENTIEL (1 thread)..."
export OMP_NUM_THREADS=1
time_start=$(date +%s)
# Ici vous devriez lancer votre programme avec des paramètres de test
# Par exemple: ./main (si votre main appelle sudoku_transition)
echo "Lancez: OMP_NUM_THREADS=1 ./main"
echo ""

# Test avec tous les threads
echo "Test PARALLÈLE (10 threads)..."
export OMP_NUM_THREADS=10
echo "Lancez: OMP_NUM_THREADS=10 ./main"
echo ""

echo "=== Comment tester manuellement ==="
echo ""
echo "1. Avec 1 thread (séquentiel):"
echo "   OMP_NUM_THREADS=1 time ./main"
echo ""
echo "2. Avec 10 threads (parallèle):"
echo "   OMP_NUM_THREADS=10 time ./main"
echo ""
echo "3. Comparez les temps d'exécution !"
echo "   Si le temps parallèle ≈ temps séquentiel / 10, c'est parfait !"
