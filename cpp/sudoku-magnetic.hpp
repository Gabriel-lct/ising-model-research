#ifndef SUDOKU_MAGNETIC_HPP
#define SUDOKU_MAGNETIC_HPP

#include <vector>
#include <string>

using grid = std::vector<std::vector<int>>;

std::vector<int> count_sigma(const grid &config);
std::vector<float> sudoku_magnetic(int n, double T_start, double T_end,
                                   int nb_inter_T, int nb_metropolis_iteration,
                                   int nb_data_to_keep, const std::string &filename);

#endif // SUDOKU_MAGNETIC_HPP