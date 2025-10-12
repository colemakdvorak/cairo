#include <random>
#include <algorithm>
#include "ShufflePrice.h"

// malloc vs new. might as well use new if we're on C++
ShufflePrice::ShufflePrice(int t, int n, int s, double **input_buffer, bool init, int sd)
    : initialized(init), T(t), N(n), pivot(s), input_data(input_buffer), seed(sd)
{
    int pivot_adj = pivot + 1;

    basis = new double[N];
    delta = new double*[N];
    for (int n_i = 0; n_i < N; ++n_i)
        delta[n_i] = new double[T];

    for (int n_i = 0; n_i < N; ++n_i) {
        basis[n_i] = input_data[n_i][pivot];
        for (int t_i = pivot_adj; t_i < T; ++t_i)
            delta[n_i][t_i] = input_data[n_i][t_i] - input_data[n_i][t_i-1];
    }
    initialized = true;
}
// Clean up
ShufflePrice::~ShufflePrice() {
    delete[] basis;
    for (int n_i = 0; n_i < N; ++n_i)
        delete[] delta[n_i];
    delete[] delta;
}

// Fischer-Yates Shuffle
void ShufflePrice::shuffle()
{
    int i, j, t_i, n_i;
    int pivot_adj = pivot + 1;

    std::random_device rd;
    std::mt19937 gen(seed == -1 ? rd() : seed);
    std::uniform_real_distribution<> dist(0.0, 1.0);

    i = N - pivot_adj;
    while(i > 1) {
        j = (int) (i * dist(gen));
        --i;
        for(n_i=0;n_i<N;n_i++)
            std::swap(delta[n_i][pivot + i], delta[n_i][pivot + j]);
    }

    // Rebuild prices
    for(n_i=0; n_i < N; n_i++) {
        input_data[n_i][pivot-1] = basis[n_i];
        for(t_i=pivot; t_i<T; t_i++)
            input_data[n_i][t_i] = input_data[n_i][t_i-1] + delta[n_i][t_i];
    }
}
