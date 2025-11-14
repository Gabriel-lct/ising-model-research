#include "metropolis.hpp"
#include "utils.hpp"
#include <iostream>

int X = 200;
int Y = 200;
int J = 1;
double T = 0.00001;
int N = 20000000;
int nb_intermediate_config = 200;

int main()
{
  configuration config(X, std::vector<int>(Y));
  init_configuration(config);

  double E = energy(config, J);
  std::cout << "Init Config Energy: " << E << std::endl;

  std::vector<configuration> steps = metropolis(config, J, T, N, nb_intermediate_config);
  double f_E = energy(steps.back(), J);
  std::cout << "Final Config Energy: " << f_E << std::endl;

  save_states(steps, "../data/antifer-config-froid.h5", J, T, N, steps.size());
}