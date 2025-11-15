#include <iostream>
#include <random>
#include <vector>
#include <chrono>

using grid = std::vector<std::vector<int>>;

static std::mt19937 rng(std::random_device{}());
static std::uniform_real_distribution<double> dist(0.0, 1.0);

void print_grid(grid config){
    int N2 = config.size();
    int N = sqrt(N2);
    int l = 0;
    for (auto &line : config){
        if (l == N){
            for (int i=0; i<4*N2; i++){std::cout << "-";}
            l = 0;
        }
        l++;
        std::cout << std::endl;
        int c = 0;
        for (auto &cell : line){
            if (c == N){
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
grid init_config(int N){
    grid config(N*N, std::vector<int>(N*N));
    for (auto &row : config)
    {
        for (auto &cell : row)
        {
        cell = ceil(dist(rng)*N*N);
        }
    }
    return config;
}

int calc_energy(grid config){
    int errors = 0;
    int N = config.size();
    int sN = sqrt(1.*N);
    // Define trackers for every line, column and square
    std::vector<std::vector<int>> seen_line(N, std::vector<int>(N));
    std::vector<std::vector<int>> seen_column(N, std::vector<int>(N));
    std::vector<std::vector<int>> seen_squares(N, std::vector<int>(N));
    for (int i=0; i<N; i++){
        for (int j=0; j<N; j++){
            int value = config[i][j];
            // Check line
            if (seen_line[i][value-1]==1){
                errors++;
            } else{
            seen_line[i][value-1] = 1;
            }
            // Check column
            if (seen_column[j][value-1]==1){
                errors++;
            } else{
            seen_column[j][value-1] = 1;
            }
            // Check square
            int xsquare = floor(j/sN);
            int ysquare = floor(i/sN);
            if (seen_squares[ysquare*sN + xsquare][value-1] == 1){
                errors++;
            } else{
                seen_squares[ysquare*sN + xsquare][value-1] = 1;
            }
        }
    }
    return errors;
}

int main(int argc, char const *argv[])
{
    double T = 0.1;
    if (argc > 1){
        T = std::stod(argv[1]);
    }
    int sN = 3;
    if (argc > 2){
        sN = std::stoi(argv[2]);
    }
    int it = 10000;
    if (argc > 3){
        it = std::stoi(argv[3]);
    }
    bool solve = true;
    auto start = std::chrono::high_resolution_clock::now();

    int N = sN*sN;
    grid conf = init_config(sN);
    print_grid(conf);
    int energy = calc_energy(conf);
    std::cout << "Energy: " << energy << std::endl;
    bool announced = false;
    //return 0;
    for (int i=0; i<it; i++){
        // Change random site
        int x = floor(dist(rng)*N);
        int y = floor(dist(rng)*N);
        int old = conf[y][x];
        conf[y][x] = ceil(dist(rng)*N);
        int new_energy = calc_energy(conf);
        int dE = new_energy - energy;
        double p = exp(-(1.*dE)/T);
        //std::cout << p << std::endl;
        //std::cout << conf[y][x] << std::endl;
        if (new_energy < energy || dist(rng) <= p){
            energy = new_energy;
        }
        else{
            conf[y][x] = old;
        }
        //std::cout << energy << std::endl;
        if (energy < N and !announced){
            std::cout << "Energy < "<< N <<" in: " << i << " iterations" << std::endl;
            announced = true;
        }
        if (solve && energy==0){
            break;
        }
    }
    std::cout<<std::endl;
    print_grid(conf);
    std::cout << "Energy: " << energy << std::endl;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> exec = end - start;
    std::cout << "Exec time: " << exec.count() << std::endl;
    std::cout << "Iterations: " << it << std::endl;

    return 0;
}
