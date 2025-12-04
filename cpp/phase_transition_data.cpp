#include "metropolis.hpp"
#include "utils.hpp"
#include <iostream>
#include <fstream>

std::vector<double> energies;
int X = 200;
int Y = 200;
int J = 1;
double B = 0;
int N = 200000;

int main(int argc, char const *argv[])
{
  configuration config(X, std::vector<int>(Y));
  init_configuration(config);
  for (int i = 0; i < 1000; i++)
  {
    double T = 0.001 * i + 2;
    std::vector<configuration> steps = metropolis(config, J, B, T, N, 2);
    energies.push_back(energy(steps[1], J, B));
  }
  std::ofstream out("../data/phase_transition.bin", std::ios::binary);
  out.write(reinterpret_cast<char *>(energies.data()),
            energies.size() * sizeof(double));
  return 0;
}
