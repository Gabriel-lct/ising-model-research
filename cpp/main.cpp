#include "metropolis.hpp"
#include "utils.hpp"
#include <iostream>

int main()
{
  // Paramètres pour l'analyse de transition de phase du sudoku
  int n = 16; // Grille 9x9
  double J = 1.0;
  double B = 0.0;
  double T_start = 0.001;              // Température initiale
  double T_end = 20.0;                 // Température finale
  int nb_inter_T = 3000;               // Nombre de points de température
  int nb_metropolis_iteration = 40000; // Nombre d'itérations Metropolis par température
  int nb_data_to_keep = 10000;         // Nombre de données d'énergie à garder depuis la fin

  std::cout << "Début de l'analyse de transition de phase pour un sudoku " << n << "x" << n << std::endl;
  std::cout << "Températures : " << T_start << " à " << T_end << " avec " << nb_inter_T << " points" << std::endl;
  std::cout << "Itérations Metropolis par température : " << nb_metropolis_iteration << std::endl;
  std::cout << "Données d'énergie conservées : " << nb_data_to_keep << " dernières valeurs" << std::endl;

  std::vector<float> C_T = metropolis_transition(
      n, J, B, T_start, T_end, nb_inter_T, nb_metropolis_iteration, nb_data_to_keep,
      "../data/sudoku/transition.h5");

  std::cout << "Analyse terminée avec succès!" << std::endl;

  return 0;
}
