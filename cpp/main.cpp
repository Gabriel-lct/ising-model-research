#include "metropolis.hpp"
#include "sudoku.hpp"
#include "sudoku-magnetic.hpp"
#include "utils.hpp"
#include <iostream>
#include <chrono>

int main(int argc, char const *argv[])
{
  auto start = std::chrono::high_resolution_clock::now();
  // Paramètres pour l'analyse de transition de phase de metropolis
  // int n = 20;                           // Taille grille
  // double J = 1.0;                       // Coupling constant
  // double B = 0.0;                       // Magnetic field
  // double T_start = 0.001;               // Température initiale
  // double T_end = 15.0;                  // Température finale
  // int nb_inter_T = 20000;               // Nombre de points de température
  // int nb_metropolis_iteration = 500000; // Nombre d'itérations Metropolis par température
  // int nb_data_to_keep = 10000;
  // double T = 4;

  // metropolis_correlation(n, J, B, T, 100000, 1000, "../data/metropolis/correlation/T"+std::to_string((int)T)+"n"+std::to_string(n)+".h5");
  // return 0;
  // std::string state;
  // if (J > 0)
  // {
  //   state = "fer/";
  // }
  // else
  // {
  //   state = "antifer/";
  // }
  // std::string filename = "../data/metropolis/" + state + "transition/" + std::to_string(n) + "-config.h5";

  // metropolis_transition(n, J, B, T_start, T_end, nb_inter_T, nb_metropolis_iteration, nb_data_to_keep, filename);

  // Code pour l'analyse de transition de phase de sudoku
  // int sN = 3;
  // double T_start = 0.00001;
  // double T_end = 1;
  // int nb_inter_T = 20000;
  // int nb_metropolis_iteration = 40000;
  // int nb_data_to_keep = 8000;

  // if (argc > 1)   
  // {
  //   sN = std::stoi(argv[1]);
  // }

  // std::string filename = "../data/sudoku/" + std::to_string(sN * sN) + "-transition-gigazoom.h5";

  // std::vector<float>
  //     susceptibility = sudoku_transition(sN, T_start, T_end, nb_inter_T, nb_metropolis_iteration, nb_data_to_keep, filename);
  
  sudoku_transition()

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> exec = end - start;
  std::cout << "Exec time: " << exec.count() << std::endl;

  return 0;
}
