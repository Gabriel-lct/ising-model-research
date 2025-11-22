#define METROPOLIS_HPP
#include <vector>

using configuration = std::vector<std::vector<int>>;

// Initialize a configuration
void init_configuration(configuration &config);

// Define the energy function
double energy(const configuration &, double J, double B);

// Define the delta energy function
double delta_energy(const configuration &config, double J, double B, int x, int y);

// Define the powerful metropolis function
std::vector<configuration> metropolis(configuration config, double J, double B, double T, int nb_iteration, int nb_intermediate_config);