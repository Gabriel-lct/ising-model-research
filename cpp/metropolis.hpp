#define METROPOLIS_HPP
#include <vector>
#include <string>

using configuration = std::vector<std::vector<int>>;

// Initialize a configuration
void init_configuration(configuration &config);

// Define the energy function
double energy(const configuration &, double J, double B);

// Define the delta energy function
double delta_energy(const configuration &config, double J, double B, int x, int y);

// Define the powerful metropolis function
std::vector<configuration> metropolis(configuration config, double J, double B, double T, int nb_iteration, int nb_intermediate_config);

// Get energies during metropolis simulation
std::vector<double> get_energies_metropolis(configuration config, double J, double B, double T, int nb_iteration);

// Calculate heat capacity C_T for phase transition analysis
std::vector<float> metropolis_transition(int n, double J, double B, double T_start, double T_end, int nb_inter_T, int nb_metropolis_iteration, int nb_data_to_keep, const std::string &filename);

// Calculate magnetic susceptibility for phase transition analysis
std::vector<float> metropolis_sensibility(int n, double J, double B, double T_start, double T_end, int nb_inter_T, int nb_metropolis_iteration, int nb_data_to_keep, const std::string &filename);