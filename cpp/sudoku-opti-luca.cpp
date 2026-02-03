#include <iostream>
#include <cmath>
#include <random>
#include <cassert>

constexpr int power(int base, int exp) {
    return (exp == 0) ? 1 : base * power(base, exp - 1);
}

// DONT CHANGE THIS FOR NOW!!!
const int N = 25*25; // number of digits
const int sN = (int) std::sqrt(N);
const int d = 3; // number of dimensions (standard 3)
const int n = power(N, d-1); // number of sites


// Random generator
static std::mt19937 rng(std::random_device{}());
static std::uniform_int_distribution<int> digit_dist(0, N - 1);
static std::uniform_int_distribution<int> site_dist(0, n-1);
static std::uniform_real_distribution<double> dist(0.0, 1.0);

// Stores digit count in the class
struct classe
{
    int digit_count[N];
    int energy;
};

// Stores value and pointers to classes
struct site
{
    int value;
    classe* classes[d]; // pointer to classes
};

// Stores sites and classes
struct sudoku
{
    site* sites[n];
    classe classes[N*d]; // stored as: rows, columns, boxes
};

sudoku init_sudoku() // random sudoku
{
    sudoku S;

    for (int i = 0; i < N*d; i++) {
        for (int j = 0; j < N; j++) {
            S.classes[i].digit_count[j] = 0;
        }
    }

    for (int i=0; i<n; i++){
        site* s = new site();
        s->value = digit_dist(rng);

        // row
        s->classes[0] = &S.classes[(int) i/N];
        S.classes[(int) i/N].digit_count[s->value] ++;

        // column
        s->classes[1] = &S.classes[i%N + N];
        S.classes[i%N + N].digit_count[s->value] ++;

        // box
        s->classes[2] = &S.classes[(int)(i / (N * sN)) * sN + (i % N) / sN + 2*N];
        S.classes[(int)(i / (N * sN)) * sN + (i % N) / sN + 2*N].digit_count[s->value] ++;

        /*
        for (int j=0; j<d; j++){
            if (j==0){
                s->classes[0] = &S.classes[i%N];
                S.classes[i%N].digit_count[s->value] ++;
            } else {
                s->classes[j] = &S.classes[(int)i/power(N, j)];
                S.classes[(int)i/power(N, j)].digit_count[s->value] ++;
            }
        }
        */
        S.sites[i] = s;
    }
    std::cout << "SUDOKU INIT OK" << std::endl;
    return S;
}

int calc_energy(sudoku* S){
    int E = 0;
    for (int i=0; i<N*d; i++){
        for (int c: S->classes[i].digit_count){
            if (c==0){
                E++;
            }
        }
    }
    return E;
}

// Energy defined as non-used digits in the class
int dE(int idx, int new_value, sudoku* S){
    site* s = S->sites[idx];
    int de = 0;
    int old_value = s->value;
    //std::cout << old_value << std::endl;

    for (int i = 0; i < d; i++){
        if (s->classes[i]->digit_count[old_value] == 1){
            de ++;
        }
        if (s->classes[i]->digit_count[new_value] == 0){
            de --;
        }
    }
    return de;
}

void change_value(int idx, int new_value, sudoku* S){
    site* s = S->sites[idx];
    int old_value = s->value;
    //std::cout << new_value << std::endl;
    //std::cout << old_value << std::endl;
    if (new_value == old_value) return;


    for (int i = 0; i < d; i++){
        s->classes[i]->digit_count[old_value] -= 1;
        s->classes[i]->digit_count[new_value] += 1;
    }
    s->value = new_value;
}

void print_Sudoku(sudoku* S){
    for (int i = 0; i<n; i++){
        std::cout << S->sites[i]->value << " ";
        if ((i+1) % N == 0){
            std::cout << std::endl;
        }
    }
}

int main(int argc, char const *argv[])
{
    sudoku Sudoku = init_sudoku();
    
    double T = 0.1;
    double beta = 1/T;
    int total_dE = 0;

    //print_Sudoku(&Sudoku);
    int energy = calc_energy(&Sudoku);
    std::cout << "SIZE: " << N << std::endl;
    std::cout << "Energy: " << energy << std::endl;
    // Metropolis
    int it = 500000;
    for (int i = 0; i < it; i++){
        //std::cout << "STARTING ITERATION: " << i << std::endl;
        int idx = site_dist(rng);
        int new_value = digit_dist(rng);
        if (new_value == Sudoku.sites[idx]->value){
            continue;
        }
        int de = dE(idx, new_value, &Sudoku);
        //std::cout << "dE CALCULATED OK: dE = " << de << std::endl;
        double p = dist(rng);

        if (de < 0 || p <= std::exp(-de*beta)){
            //std::cout << "CHANGING VALUE" << std::endl;
            change_value(idx, new_value, &Sudoku);
            //std::cout << "VALUE CHANGED" << std::endl;
            total_dE += de;
        } 
        else {
            //std::cout << "SKIPPING FLIP" << std::endl;
        }
        //std::cout << total_dE << std::endl;
        //std::cout << "ITERATION DONE: " << i << std::endl;
    }
    std::cout << std::endl;

    //print_Sudoku(&Sudoku);
    energy = calc_energy(&Sudoku);
    std::cout << "Energy: " << energy << std::endl;
    std::cout << "Total dE: " << total_dE << std::endl;

    return 0;
}
