#include "metropolis.hpp"
#include "utils.hpp"
#include <cmath>
#include <random>
#include <iostream>
#include <iomanip>
#include <omp.h>

// Random generator
static std::mt19937 rng(std::random_device{}());
static std::uniform_real_distribution<double> dist(0.0, 1.0);

void init_configuration(configuration &config)
{
  for (auto &row : config)
  {
    for (auto &spin : row)
    {
      spin = (dist(rng) > 0.5) ? 1 : -1;
    }
  }
}

double energy(const configuration &config, double J, double B)
{
  int X = config.size();
  int Y = config[0].size();

  double E = 0.0;

  for (int x = 0; x < X - 1; x++)
  {
    for (int y = 0; y < Y - 1; y++)
    {
      E -= J * config[x][y] * config[x + 1][y];
      E -= J * config[x][y] * config[x][y + 1];
      E += B * config[x][y];
    }
  }
  return E;
}

double delta_energy(const configuration &config, double J, double B, int x, int y)
{
  int X = config.size();
  int Y = config[0].size();

  int spin = config[x][y];
  int sum_neighbors = 0;

  // Left
  sum_neighbors += config[(x - 1 + X) % X][y];
  // Right
  sum_neighbors += config[(x + 1) % X][y];
  // Top
  sum_neighbors += config[x][(y + 1) % Y];
  // Bottom
  sum_neighbors += config[x][(y - 1 + Y) % Y];

  sum_neighbors *= J;
  // Field
  sum_neighbors -= B;

  return 2.0 * spin * sum_neighbors;
}

std::vector<configuration> metropolis(configuration config, double J, double B, double T, int nb_iteration, int nb_intermediate_config)
{
  int X = config.size();
  int Y = config[0].size();

  // Create an index to save intermediate config
  int idx = std::floor(nb_iteration / nb_intermediate_config);
  std::vector<configuration> intermediate_configs;
  intermediate_configs.push_back(config);

  // random generator
  std::uniform_int_distribution<int>
      dist_x(0, X - 1);
  std::uniform_int_distribution<int> dist_y(0, Y - 1);

  for (int i = 0; i < nb_iteration; i++)
  {
    // Select a random spin in the configuration
    int x = dist_x(rng);
    int y = dist_y(rng);

    // Calculate the delta energy
    double d_energy = delta_energy(config, J, B, x, y);
    // Define the probability to accept the new state
    double p = exp(-d_energy / T);

    if (d_energy <= 0 || dist(rng) <= p)
    {
      // Accept the new configuration
      config[x][y] *= -1;
    }

    // Save intermediate config
    if (i % idx == 0)
    {
      intermediate_configs.push_back(config);
    }
  }

  return intermediate_configs;
}

std::vector<double> get_energies_metropolis(configuration config, double J, double B, double T, int nb_iteration)
{
  int X = config.size();
  int Y = config[0].size();

  std::vector<double> Es;
  Es.push_back(energy(config, J, B));

  // random generator
  std::uniform_int_distribution<int>
      dist_x(0, X - 1);
  std::uniform_int_distribution<int> dist_y(0, Y - 1);

  for (int i = 0; i < nb_iteration; i++)
  {
    // Select a random spin in the configuration
    int x = dist_x(rng);
    int y = dist_y(rng);

    // Calculate the delta energy
    double d_energy = delta_energy(config, J, B, x, y);
    // Define the probability to accept the new state
    double p = exp(-d_energy / T);

    if (d_energy <= 0 || dist(rng) <= p)
    {
      // Accept the new configuration
      config[x][y] *= -1;
    }

    // Save config's energy
    Es.push_back(energy(config, J, B));
  }

  return Es;
}

std::vector<float> metropolis_transition(int n, double J, double B, double T_start, double T_end, int nb_inter_T, int nb_metropolis_iteration, int nb_data_to_keep, const std::string &filename)
{
  // Initialize the C_T array
  std::vector<float> C_T(nb_inter_T);

  // Counter for completed tasks
  int completed = 0;

#pragma omp parallel for schedule(dynamic)
  for (int i = 0; i < nb_inter_T; i++)
  {
    float T = T_start + i * (T_end - T_start) / (nb_inter_T - 1);

    configuration config(n, std::vector<int>(n));
    init_configuration(config);

    std::vector<double> Es = get_energies_metropolis(config, J, B, T, nb_metropolis_iteration);

    // Garder seulement les nb_data_to_keep dernières énergies (après thermalisation)
    size_t start_idx = (Es.size() > static_cast<size_t>(nb_data_to_keep)) ? Es.size() - nb_data_to_keep : 0;
    std::vector<double> Es_kept(Es.begin() + start_idx, Es.end());

    std::vector<double> EsSquare = std::vector<double>(Es_kept.size());
    for (size_t j = 0; j < Es_kept.size(); ++j)
    {
      EsSquare[j] = Es_kept[j] * Es_kept[j];
    }

    double var_E = mean_double(EsSquare) - mean_double(Es_kept) * mean_double(Es_kept);
    double beta = 1.0 / T;

    C_T[i] = beta * beta * var_E;

    // Increment completed counter and display progress
#pragma omp critical
    {
      completed++;
      std::cout << "Progress: " << completed << "/" << nb_inter_T
                << " (" << std::fixed << std::setprecision(2)
                << (static_cast<double>(completed) / nb_inter_T * 100) << "%)" << std::endl;
    }
  }

  // Save the transitions data
  save_transitions(C_T, filename, T_start, T_end, nb_inter_T, nb_metropolis_iteration, n, J, B);

  return C_T;
}

std::vector<float> metropolis_sensibility(int n, double J, double B, double T_start, double T_end, int nb_inter_T, int nb_metropolis_iteration, int nb_data_to_keep, const std::string &filename)
{
  // Initialize the susceptibility array
  std::vector<float> susceptibility(nb_inter_T);

  // Counter for completed tasks
  int completed = 0;

#pragma omp parallel for schedule(dynamic)
  for (int i = 0; i < nb_inter_T; i++)
  {
    float T = T_start + i * (T_end - T_start) / (nb_inter_T - 1);

    configuration config(n, std::vector<int>(n));
    init_configuration(config);

    // Run metropolis and get all configurations
    std::vector<configuration> configs = metropolis(config, J, B, T, nb_metropolis_iteration, nb_metropolis_iteration);

    // Garder seulement les nb_data_to_keep dernières configurations (après thermalisation)
    size_t start_idx = (configs.size() > static_cast<size_t>(nb_data_to_keep)) ? configs.size() - nb_data_to_keep : 0;
    std::vector<configuration> configs_kept(configs.begin() + start_idx, configs.end());

    // Calculer la magnétisation pour chaque configuration
    std::vector<double> magnetizations(configs_kept.size());
    std::vector<double> magnetizations_sq(configs_kept.size());

    for (size_t k = 0; k < configs_kept.size(); ++k)
    {
      double M = 0.0;
      for (const auto &row : configs_kept[k])
      {
        for (int spin : row)
        {
          M += spin;
        }
      }
      magnetizations[k] = M;
      magnetizations_sq[k] = M * M;
    }

    // Calculer la variance de la magnétisation: <M²> - <M>²
    double mean_M = mean_double(magnetizations);
    double mean_M_sq = mean_double(magnetizations_sq);
    double var_M = mean_M_sq - mean_M * mean_M;

    // Susceptibilité magnétique: χ = β * Var(M)
    double chi = var_M / T;

    susceptibility[i] = chi;

    // Increment completed counter and display progress
#pragma omp critical
    {
      completed++;
      std::cout << "Progress: " << completed << "/" << nb_inter_T
                << " (" << std::fixed << std::setprecision(2)
                << (static_cast<double>(completed) / nb_inter_T * 100) << "%)" << std::endl;
    }
  }

  // Save the susceptibility data
  save_transitions(susceptibility, filename, T_start, T_end, nb_inter_T, nb_metropolis_iteration, n, J, B);

  return susceptibility;
}

std::vector<double> metropolis_correlation(int n, double J, double B, double T, int nb_iteration, int nb_data_to_keep, const std::string &filename){
  configuration config(n, std::vector<int>(n));
  init_configuration(config);
  std::cout << "Starting metropolis" << std::endl;
  std::vector<configuration> configs = metropolis(config, J, B, T, nb_iteration, nb_iteration);
  std::cout << "Metropolis done" << std::endl;
  std::cout << "Computing correlations" << std::endl;
  std::vector<double> correlations = std::vector<double>(2*n-1);
  std::vector<double> count = std::vector<double>(2*n-1);
  for (int i=nb_iteration-nb_data_to_keep; i < nb_iteration; i++){
    configuration conf = configs[i];
    for (int y1 = 0; y1 < n; y1++){
      for (int x1=0; x1 < n; x1++){
        int spin = conf[y1][x1];
        for (int y2 = y1; y2 < n; y2++){
          for (int x2 = y2; x2 < n; x2++){
            int dist = abs((x2-x1)) + abs((y2-y1));
            correlations[dist] += spin*conf[y2][x2];
            count[dist] ++;
          }
        }
      }
    }
  }
  for (int i=0; i<2*n-1; i++){
    correlations[i] /= count[i];
    std::cout << correlations[i] << std::endl;
  }

  save_correlations(correlations, filename, T, nb_iteration, n, J, B);

  return correlations;
}