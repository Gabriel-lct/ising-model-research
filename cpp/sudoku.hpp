#ifndef SUDOKU_HPP
#define SUDOKU_HPP

#include <vector>
#include <string>

using grid = std::vector<std::vector<int>>;

void print_grid(grid config);
grid init_sudoku(int N);
int calc_energy(grid config);
std::pair<std::vector<grid>, std::vector<double>> sudoku(double T, int sN, int max_iteration, bool solve);
std::vector<float> sudoku_transition(int n, double T_start, double T_end, int nb_inter_T,
                                     int nb_metropolis_iteration, int nb_data_to_keep,
                                     const std::string &filename);

#endif // SUDOKU_HPP