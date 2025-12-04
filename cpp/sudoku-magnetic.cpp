#include "sudoku.hpp"
#include "metropolis.hpp"
#include "utils.hpp"
#include <cmath>
#include <random>
#include <iostream>
#include <iomanip>

std::vector<int> count_sigma(const grid &config)
{
  int X = config.size();
  std::vector<int> counter(X, 0);

  for (int i = 0; i < X; i++)
  {
    for (int j = 0; j < X; j++)
    {
      counter[config[i][j] - 1] += 1;
    }
  }
  return counter;
}

std::vector<float> sudoku_magnetic(int n, double T_start, double T_end, int nb_inter_T, int nb_metropolis_iteration, int nb_data_to_keep, const std::string &filename)
{
  // Initialize the C_T array
  std::vector<float> susceptibility;
  susceptibility.reserve(nb_inter_T);

  for (int i = 0; i < nb_inter_T; i++)
  {
    float T = T_start + i * (T_end - T_start) / (nb_inter_T - 1);

    std::cout << "Progress: " << std::fixed << std::setprecision(2)
              << (static_cast<double>(i) / nb_inter_T * 100) << "%" << std::endl;

    std::vector<grid> sudokus = sudoku(T, n, nb_metropolis_iteration, false).first;

    // Garder seulement les nb_data_to_keep dernières énergies (après thermalisation)
    size_t start_idx = (sudokus.size() > static_cast<size_t>(nb_data_to_keep)) ? sudokus.size() - nb_data_to_keep : 0;
    std::vector<grid> sudokus_kept(sudokus.begin() + start_idx, sudokus.end());

    // mean_counts[k] stockera <N_k> et mean_sq_counts[k] stockera <N_k^2>
    std::vector<double> mean_counts(n, 0.0);
    std::vector<double> mean_sq_counts(n, 0.0);

    for (auto &conf : sudokus_kept)
    {
      std::vector<int> counter = count_sigma(conf);

      for (int digit = 0; digit < n; digit++)
      {
        double val = static_cast<double>(counter[digit]);
        mean_counts[digit] += val;
        mean_sq_counts[digit] += val * val;
      }
    }

    double total_variance = 0.0;

    for (int i = 0; i < n; i++)
    {
      double avg_N = mean_counts[i] / sudokus_kept.size();
      double avg_sq_N = mean_sq_counts[i] / sudokus_kept.size();

      total_variance += avg_sq_N - (avg_N * avg_N);
    }

    double chi = total_variance / T;

    susceptibility.push_back(chi);
  }
  // Save the transitions datas
  save_transitions(susceptibility, filename, T_start, T_end, nb_inter_T, nb_metropolis_iteration, n);
}