#include "metropolis.hpp"
#include "sudoku.hpp"
#include "sudoku-magnetic.hpp"
#include "utils.hpp"
#include <iostream>
#include <chrono>

int main(int argc, char const *argv[])
{
  // Paramètres pour l'analyse de transition de phase du sudoku
  /* int n = 4; // Grille 9x9
  double J = 1.0;
  double B = 0.0;
  double T_start = 0.001;              // Température initiale
  double T_end = 20.0;                 // Température finale
  int nb_inter_T = 3000;               // Nombre de points de température
  int nb_metropolis_iteration = 50000; // Nombre d'itérations Metropolis par température
  int nb_data_to_keep = 5000 */
  // Nombre de données d'énergie à garder depuis la fin

  auto start = std::chrono::high_resolution_clock::now();

  int sN = 3;
  double T_start = 0.0001;
  double T_end = 5;
  int nb_inter_T = 8000;
  int nb_metropolis_iteration = 50000;
  int nb_data_to_keep = 1000;

  if (argc > 1)
  {
    sN = std::stoi(argv[1]);
  }

  std::string filename = "../data/sudoku/" + std::to_string(sN * sN) + "-magnetic.h5";

  std::vector<float>
      susceptibility = sudoku_magnetic(sN, T_start, T_end, nb_inter_T, nb_metropolis_iteration, nb_data_to_keep, filename);

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> exec = end - start;
  std::cout << "Exec time: " << exec.count() << std::endl;

  return 0;
}
