#include <random>
#include <algorithm>
#include "ShuffleBar.h"

ShuffleBar::ShuffleBar(int T_, int N_, int pivot_, double **open_, double **high_, double **low_, double **close_, int seed_)
    : T(T_), N(N_), pivot(pivot_), seed(seed_), open_ptr(open_), high_ptr(high_), low_ptr(low_), close_ptr(close_), initialized(false)
{
    int pivot_adj = pivot + 1;

    // Allocate basis arrays
    basis_open  = new double[N];
    basis_high  = new double[N];
    basis_low   = new double[N];
    basis_close = new double[N];

    // Allocate relative arrays
    rel_open  = new double*[N];
    rel_high  = new double*[N];
    rel_low   = new double*[N];
    rel_close = new double*[N];

    for (int n_i = 0; n_i < N; ++n_i) {
        rel_open[n_i]  = new double[T];
        rel_high[n_i]  = new double[T];
        rel_low[n_i]   = new double[T];
        rel_close[n_i] = new double[T];
    }

    // Compute relative movements
    for (int n_i = 0; n_i < N; ++n_i) {
        basis_open[n_i]  = open_ptr[n_i][pivot];
        basis_high[n_i]  = high_ptr[n_i][pivot];
        basis_low[n_i]   = low_ptr[n_i][pivot];
        basis_close[n_i] = close_ptr[n_i][pivot];

        for (int t_i = pivot_adj; t_i < T; ++t_i) {
            rel_open[n_i][t_i]  = open_ptr[n_i][t_i] - close_ptr[n_i][t_i - 1];  // inter-bar
            rel_high[n_i][t_i]  = high_ptr[n_i][t_i] - open_ptr[n_i][t_i];       // intra-bar
            rel_low[n_i][t_i]   = low_ptr[n_i][t_i] - open_ptr[n_i][t_i];
            rel_close[n_i][t_i] = close_ptr[n_i][t_i] - open_ptr[n_i][t_i];
        }
    }

    initialized = true;
}

ShuffleBar::~ShuffleBar() {
    delete[] basis_open;
    delete[] basis_high;
    delete[] basis_low;
    delete[] basis_close;

    for (int n_i = 0; n_i < N; ++n_i) {
        delete[] rel_open[n_i];
        delete[] rel_high[n_i];
        delete[] rel_low[n_i];
        delete[] rel_close[n_i];
    }

    delete[] rel_open;
    delete[] rel_high;
    delete[] rel_low;
    delete[] rel_close;
}

void ShuffleBar::shuffle() {
    if (!initialized) return;

    int pivot_adj = pivot + 1;
    int remaining = T - pivot_adj;

    std::random_device rd;
    std::mt19937 gen(seed == -1 ? rd() : seed);
    std::uniform_real_distribution<> dist(0.0, 1.0);

    // Shuffle the intra-bar trios
    int i = remaining;
    while (i > 1) {
        int j = static_cast<int>(dist(gen) * i);
        if (j >= i) j = i - 1;
        --i;
        for (int n_i = 0; n_i < N; ++n_i) {
            std::swap(rel_high[n_i][pivot + i], rel_high[n_i][pivot + j]);
            std::swap(rel_low[n_i][pivot + i], rel_low[n_i][pivot + j]);
            std::swap(rel_close[n_i][pivot + i], rel_close[n_i][pivot + j]);
        }
    }

    // Shuffle the inter-bar changes identically across markets
    i = remaining;
    while (i > 1) {
        int j = static_cast<int>(dist(gen) * i);
        if (j >= i) j = i - 1;
        --i;
        for (int n_i = 0; n_i < N; ++n_i) {
            std::swap(rel_open[n_i][pivot + i], rel_open[n_i][pivot + j]);
        }
    }

    // Rebuild permuted OHLC series
    for (int n_i = 0; n_i < N; ++n_i) {
        open_ptr[n_i][pivot - 1]  = basis_open[n_i];
        high_ptr[n_i][pivot - 1]  = basis_high[n_i];
        low_ptr[n_i][pivot - 1]   = basis_low[n_i];
        close_ptr[n_i][pivot - 1] = basis_close[n_i];

        for (int t_i = pivot_adj; t_i < T; ++t_i) {
            open_ptr[n_i][t_i]  = close_ptr[n_i][t_i - 1] + rel_open[n_i][t_i];
            high_ptr[n_i][t_i]  = open_ptr[n_i][t_i] + rel_high[n_i][t_i];
            low_ptr[n_i][t_i]   = open_ptr[n_i][t_i] + rel_low[n_i][t_i];
            close_ptr[n_i][t_i] = open_ptr[n_i][t_i] + rel_close[n_i][t_i];
        }
    }
}
