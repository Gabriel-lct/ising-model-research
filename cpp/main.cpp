#include "metropolis.hpp"
#include "utils.hpp"
#include <iostream>

int X = 400;
int Y = 400;
int J = 1;
double T = 1;
int N = std::numeric_limits<int>::max();
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

  save_states(steps, "../data/configs.h5", J, T, N, steps.size());
}