# Nom de l'exécutable
TARGET = main

# Compilateur
CXX = clang++

# Chemin HDF5 (Homebrew)
HDF5_DIR = /opt/homebrew/opt/hdf5

# Chemin OpenMP (Homebrew)
OMP_DIR = /opt/homebrew/opt/libomp

# Options de compilation
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -I$(HDF5_DIR)/include -I$(OMP_DIR)/include -Xpreprocessor -fopenmp

# Flags pour HDF5
LDFLAGS = -L$(HDF5_DIR)/lib -lhdf5_cpp -lhdf5 -L$(OMP_DIR)/lib -lomp

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
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Créer le dossier build s'il n'existe pas
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Nettoyer les fichiers intermédiaires
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Cible pratique
run: $(TARGET)
	./$(TARGET)