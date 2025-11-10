#include "metropolis.hpp"
#include <cmath>
#include <random>
#include <iostream>

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

double energy(const configuration &config, double J)
{
  int X = config.size();
  int Y = config[0].size();

  double E = 0.0;

  for (int x = 0; x < X - 1; x++)
  {
    for (int y = 0; y < Y - 1; y++)
    {
      E += J * config[x][y] * config[x + 1][y];
      E += J * config[x][y] * config[x][y + 1];
    }
  }
  return E;
}

double delta_energy(const configuration &config, double J, int x, int y)
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

  return 2.0 * J * spin * sum_neighbors;
}

std::vector<configuration> metropolis(configuration config, double J, double T, int nb_iteration, int nb_intermediate_config)
{
  int X = config.size();
  int Y = config[0].size();

  // Create an index to save intermediate config
  int idx = std::floor(nb_iteration / nb_intermediate_config);
  std::cout << "idx: " << idx << std::endl;
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
    double d_energy = delta_energy(config, J, x, y);
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