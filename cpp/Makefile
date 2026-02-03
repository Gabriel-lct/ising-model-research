# Nom de l'exécutable
TARGET = main

# Compilateur
CXX = g++

# OpenMP flags for macOS (Homebrew libomp)
OPENMP_CFLAGS = -Xpreprocessor -fopenmp -I$(shell brew --prefix libomp)/include
OPENMP_LIBS = -L$(shell brew --prefix libomp)/lib -lomp

# Options de compilation
CXXFLAGS = -std=c++17 -Wall -Wextra -O3 $(OPENMP_CFLAGS)

# Flags pour HDF5 (utilise pkg-config pour trouver automatiquement les chemins)
HDF5_CFLAGS = $(shell pkg-config --cflags hdf5)
HDF5_LIBS = $(shell pkg-config --libs hdf5)

# Flags de linkage
LDFLAGS = $(HDF5_LIBS) -lhdf5_cpp $(OPENMP_LIBS)

# Dossier pour les fichiers intermédiaires
BUILD_DIR = build

# Fichiers source
SRCS = main.cpp metropolis.cpp utils.cpp sudoku.cpp sudoku-magnetic.cpp

# Fichiers objets (un par .cpp) dans build/
OBJS = $(SRCS:%.cpp=$(BUILD_DIR)/%.o)

# Règle principale : compilation complète
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Compilation de chaque fichier .cpp
$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(HDF5_CFLAGS) -c $< -o $@

# Créer le dossier build s'il n'existe pas
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Nettoyer les fichiers intermédiaires
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Cible pratique
run: $(TARGET)
	./$(TARGET)

.PHONY: clean run