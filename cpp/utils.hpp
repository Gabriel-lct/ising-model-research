#define UTILS_HPP
#include <vector>
#include <string>

using configuration = std::vector<std::vector<int>>;

// Print the configuration given in the terminal
void show_configuration(const configuration &);

// Save data in a csv to be exploited in python
bool save_states(const std::vector<configuration> &, const std::string filename, double J, double T, int N, int nb_intermediate_config);

// Calculate the mean of a vector of doubles
double mean_double(const std::vector<double> &vec);

// Save phase transition data (C_T values) to HDF5
bool save_transitions(const std::vector<float> &C_T, const std::string filename, double T_start, double T_end, int nb_inter_T, int nb_metropolis_iteration, int n, double J = 0.0, double B = 0.0);

bool save_correlations(const std::vector<double> &correlations, const std::string filename, double T, int nb_metropolis_iteration, int n, double J, double B);