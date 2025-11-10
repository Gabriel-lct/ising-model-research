# Ising Model Research

Étude computationnelle du modèle d'Ising et des transitions de phase en physique statistique

## 📋 Table des matières

- [À propos](#à-propos)
- [Caractéristiques](#caractéristiques)
- [Structure du projet](#structure-du-projet)
- [Installation](#installation)
- [Auteurs](#auteurs)

## 🔬 À propos

Blablabla Alix est génial.

### Objectifs scientifiques

- Simulation de transitions de phase ferromagnétique/paramagnétique
- Détermination de la température critique
- Calcul des exposants critiques
- Étude de l'équilibration thermodynamique
- Analyse des effets de taille finie

## ✨ Caractéristiques

- 📊 **Modèles 1D et 2D** avec interactions de plus proches voisins
- ⚡ **Implémentation C++** haute performance
- 📈 **Notebooks Python** interactifs pour l'analyse
- 🎨 **Visualisations** des configurations de spins
- 💾 **Stockage HDF5** pour les données volumineuses

## 📁 Structure du projet

```
ising-model-research/
│
├── 📄 README.md                  # Ce fichier (mais nan)
├── 📄 requirements.txt           # Dépendances Python
├── 📄 .gitignore                 # Fichiers à ignorer
│
├── 🐍 python/                    # Code Python
│   ├── metropolis.ipynb          # Modèle 2D avec Metropolis
│   ├── modelA.ipynb              # Modèle 1D (chaîne)
│   └── notebooks/                # Analyses supplémentaires
│
├── ⚙️  cpp/                       # Code C++
│   ├── main.cpp                  # Programme principal
│   ├── metropolis.cpp/hpp        # Algorithme de Metropolis
│   ├── utils.cpp/hpp             # Fonctions utilitaires
│   ├── Makefile                  # Configuration de build
│   └── build/                    # Binaires compilés
│
├── 💾 data/                      # Données de simulation
│   └── configs.h5                # Configurations au format HDF5
│
├── 🎬 animations/                # Sorties de visualisation
│
└── 🔧 .vscode/                   # Configuration VS Code
```

## 🚀 Installation

### Prérequis

- **Python** 3.8 ou supérieur
- **C++** compilateur C++11+ (g++, clang++)
- **HDF5** bibliothèque C++ (libhdf5-dev)
- **Jupyter** Notebook/Lab (optionnel)
- **Git** pour cloner le dépôt

### Étapes d'installation

1. **Cloner le dépôt**

```bash
git clone https://github.com/votre-username/ising-model-research.git
cd ising-model-research
```

2. **Installer la bibliothèque HDF5**

Sur macOS :

```bash
brew install hdf5
```

Sur Linux (Ubuntu/Debian) :

```bash
sudo apt-get install libhdf5-dev
```

3. **Créer un environnement virtuel Python**

```bash
python3 -m venv venv
source venv/bin/activate  # macOS/Linux
```

4. **Installer les dépendances Python**

```bash
pip install --upgrade pip
pip install -r requirements.txt
```

5. **Compiler le code C++**

```bash
cd cpp
make
cd ..
```

> **Note pour les utilisateurs de VS Code** : Si vous utilisez VS Code, ajoutez le chemin d'installation de HDF5 dans le fichier `.vscode/c_cpp_properties.json` :
>
> ```json
> {
>   "configurations": [
>     {
>       "includePath": [
>         "${workspaceFolder}/**",
>         "/opt/homebrew/include" // macOS (Apple Silicon)
>         // ou "/usr/local/include" pour macOS (Intel)
>         // ou "/usr/include" pour Linux
>       ]
>     }
>   ]
> }
> ```
>
> Si nécessaire, modifiez également les chemins dans le `Makefile` (section `cpp/`) pour correspondre à votre installation de HDF5.

## 👥 Auteurs

**Luca Bozzzzzìì Mais-nan-dez (a.k.a. Le Chilien)**

- GitHub: [@izzobacul](https://github.com/izzobacul)
- Email: luca-bozzi@chili-con-carne.com

**Anna Petite (elle n'est pas grande mais on ne lui en veut pas)**

- GitHub: [@annapetit](https://github.com/annapetit)
- Email: anna-petit@sorbonne.musicologie.trompette-magique.com

**Gaybriel Lancelot (Le premier chevalier homosexuel)**

- GitHub: [@Gabriel-lct](https://github.com/Gabriel-lct)
- Email: gabriel-lancelot@??.com

## 🙏 Remerciements

- Alxi Delaporte pour la patience de son enseignement.

---

<div align="center">

**[⬆ Retour en haut](#ising-model-research)**

</div>
