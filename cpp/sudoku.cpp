#include "metropolis.hpp"
#include "utils.hpp"
#include <iostream>
#include <iomanip>
#include <random>
#include <vector>
#include <chrono>
#include <fstream>
#include <cmath>
#include <omp.h>

using grid = std::vector<std::vector<int>>;

static std::mt19937 rng(std::random_device{}());
static std::uniform_real_distribution<double> dist(0.0, 1.0);

void print_grid(grid config)
{
    int N2 = config.size();
    int N = sqrt(N2);
    int l = 0;
    for (auto &line : config)
    {
        if (l == N)
        {
            for (int i = 0; i < 4 * N2; i++)
            {
                std::cout << "-";
            }
            l = 0;
        }
        l++;
        std::cout << std::endl;
        int c = 0;
        for (auto &cell : line)
        {
            if (c == N)
            {
                std::cout << "|  ";
                c = 0;
            }
            std::cout << cell << "  ";
            c++;
        }
        std::cout << std::endl;
    }
}

// We denote N the size of the squares.
// Possible values is then N*N
grid init_sudoku(int N)
{
    grid config(N * N, std::vector<int>(N * N));
    for (auto &row : config)
    {
        for (auto &cell : row)
        {
            cell = ceil(dist(rng) * N * N);
        }
    }
    return config;
}

int calc_energy(grid config)
{
    int errors = 0;
    int N = config.size();
    int sN = sqrt(1. * N);
    // Define trackers for every line, column and square
    std::vector<std::vector<int>> seen_line(N, std::vector<int>(N));
    std::vector<std::vector<int>> seen_column(N, std::vector<int>(N));
    std::vector<std::vector<int>> seen_squares(N, std::vector<int>(N));
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            int value = config[i][j];
            // Check line
            if (seen_line[i][value - 1] == 1)
            {
                errors++;
            }
            else
            {
                seen_line[i][value - 1] = 1;
            }
            // Check column
            if (seen_column[j][value - 1] == 1)
            {
                errors++;
            }
            else
            {
                seen_column[j][value - 1] = 1;
            }
            // Check square
            int xsquare = floor(j / sN);
            int ysquare = floor(i / sN);
            if (seen_squares[ysquare * sN + xsquare][value - 1] == 1)
            {
                errors++;
            }
            else
            {
                seen_squares[ysquare * sN + xsquare][value - 1] = 1;
            }
        }
    }
    return errors;
}

std::pair<std::vector<grid>, std::vector<double>> sudoku(double T, int sN, int max_iteration, bool solve)
{
    int N = sN * sN;
    grid conf = init_sudoku(sN);
    std::vector<grid> sudokus(max_iteration);
    double energy = calc_energy(conf);
    std::vector<double> energies = {energy};
    bool announced = true;
    // return 0;
    int i;
    for (i = 0; i < max_iteration; i++)
    {
        // Change random site
        int x = floor(dist(rng) * N);
        int y = floor(dist(rng) * N);
        int old = conf[y][x];
        conf[y][x] = ceil(dist(rng) * N);
        int new_energy = calc_energy(conf);
        int dE = new_energy - energy;
        double p = exp(-(1. * dE) / T);
        // std::cout << p << std::endl;
        // std::cout << conf[y][x] << std::endl;
        if (new_energy < energy || dist(rng) <= p)
        {
            energy = new_energy;
        }
        else
        {
            conf[y][x] = old;
        }
        // std::cout << energy << std::endl;
        if (energy < N and !announced)
        {
            std::cout << "Energy < " << N << " in: " << i << " iterations" << std::endl;
            announced = true;
        }

        energies.push_back(energy);
        sudokus.push_back(conf);

        if (solve && energy == 0)
        {
            break;
        }
    }
    return {sudokus, energies};
}

std::vector<float> sudoku_transition(int n, double T_start, double T_end, int nb_inter_T, int nb_metropolis_iteration, int nb_data_to_keep, const std::string &filename)
{
    // Initialize the C_T array
    std::vector<float> C_T(nb_inter_T);

    // Counter for completed tasks
    int completed = 0;

// Parallelize the loop over temperatures
#pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < nb_inter_T; i++)
    {
        float T = T_start + i * (T_end - T_start) / (nb_inter_T - 1);

        std::vector<double> Es = sudoku(T, n, nb_metropolis_iteration, false).second;

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
    save_transitions(C_T, filename, T_start, T_end, nb_inter_T, nb_metropolis_iteration, n);

    return C_T;
}