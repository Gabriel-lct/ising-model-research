#define UTILS_HPP
#include <vector>
#include <string>

using configuration = std::vector<std::vector<int>>;

// Print the configuration given in the terminal
void show_configuration(const configuration &);

// Save data in a csv to be exploited in python
bool save_states(const std::vector<configuration> &, const std::string filename, double J, double T, int N, int nb_intermediate_config);
