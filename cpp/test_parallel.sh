#!/bin/bash

# Script pour tester la performance du parallélisme

echo "=== Test de performance parallélisme OpenMP ==="
echo ""

# Nombre de cœurs
CORES=$(sysctl -n hw.ncpu)
echo "Nombre de cœurs détectés: $CORES"
echo ""

# Créer un programme de test simple
cat > test_perf.cpp << 'EOF'
#include <iostream>
#include <chrono>
#include <omp.h>

int main() {
    const int N = 1000;
    int num_threads = omp_get_max_threads();
    
    std::cout << "Nombre de threads: " << num_threads << std::endl;
    
    // Test sans parallélisme
    auto start = std::chrono::high_resolution_clock::now();
    double sum = 0;
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < 100000; j++) {
            sum += i * j * 0.001;
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration_seq = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Temps séquentiel: " << duration_seq << " ms" << std::endl;
    
    // Test avec parallélisme
    start = std::chrono::high_resolution_clock::now();
    sum = 0;
    #pragma omp parallel for reduction(+:sum)
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < 100000; j++) {
            sum += i * j * 0.001;
        }
    }
    end = std::chrono::high_resolution_clock::now();
    auto duration_par = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Temps parallèle: " << duration_par << " ms" << std::endl;
    
    double speedup = (double)duration_seq / duration_par;
    std::cout << "Accélération: " << speedup << "x" << std::endl;
    std::cout << "Efficacité: " << (speedup / num_threads * 100) << "%" << std::endl;
    
    return 0;
}
EOF

# Compiler
echo "Compilation du test..."
clang++ -std=c++17 -O2 -I/opt/homebrew/opt/libomp/include -Xpreprocessor -fopenmp \
    test_perf.cpp -o test_perf -L/opt/homebrew/opt/libomp/lib -lomp

# Exécuter
echo ""
echo "Exécution du test..."
./test_perf

# Nettoyer
rm test_perf.cpp test_perf

echo ""
echo "=== Conclusion ==="
echo "Si l'accélération est proche de $CORES, le parallélisme fonctionne bien !"
