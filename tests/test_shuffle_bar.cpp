#include <gtest/gtest.h>
#include "ShuffleBar.h"
#include <vector>
#include <iostream>

// Helper to allocate and initialize 2D OHLC arrays
void make_ohlc_data(int N, int T, double*** open, double*** high, double*** low, double*** close) {
    *open  = new double*[N];
    *high  = new double*[N];
    *low   = new double*[N];
    *close = new double*[N];

    for (int n = 0; n < N; ++n) {
        (*open)[n]  = new double[T];
        (*high)[n]  = new double[T];
        (*low)[n]   = new double[T];
        (*close)[n] = new double[T];
        for (int t = 0; t < T; ++t) {
            double base = n * 10 + t;
            (*open)[n][t]  = base;
            (*high)[n][t]  = base + 1.0;
            (*low)[n][t]   = base - 1.0;
            (*close)[n][t] = base + 0.5;
        }
    }
}

void free_ohlc_data(double** open, double** high, double** low, double** close, int N) {
    for (int n = 0; n < N; ++n) {
        delete[] open[n];
        delete[] high[n];
        delete[] low[n];
        delete[] close[n];
    }
    delete[] open;
    delete[] high;
    delete[] low;
    delete[] close;
}

TEST(ShuffleBarTest, ShuffleChangesData) {
    int N = 2, T = 5, pivot = 1;
    double **open, **high, **low, **close;
    make_ohlc_data(N, T, &open, &high, &low, &close);

    // Copy original for comparison
    struct OHLC {
        std::vector<double> open, high, low, close;
    };
    std::vector<OHLC> original(N, {std::vector<double>(T), std::vector<double>(T),
                                   std::vector<double>(T), std::vector<double>(T)});
    for (int n = 0; n < N; ++n)
        for (int t = 0; t < T; ++t) {
            original[n].open[t]  = open[n][t];
            original[n].high[t]  = high[n][t];
            original[n].low[t]   = low[n][t];
            original[n].close[t] = close[n][t];
        }

    ShuffleBar shuffler(T, N, pivot, open, high, low, close, 42);
    shuffler.shuffle();

    // Print for visual inspection (optional)
    std::cout << "Shuffled Bar Data:\n";
    for (int n = 0; n < N; ++n) {
        std::cout << "Market " << n << ":\n";
        for (int t = 0; t < T; ++t) {
            std::cout << "O:" << open[n][t] << " H:" << high[n][t]
                      << " L:" << low[n][t] << " C:" << close[n][t] << "\n";
        }
    }

    // Check that at least one bar changed
    bool changed = false;
    for (int n = 0; n < N && !changed; ++n)
        for (int t = 0; t < T && !changed; ++t) {
            if (open[n][t]  != original[n].open[t] ||
                high[n][t]  != original[n].high[t] ||
                low[n][t]   != original[n].low[t]  ||
                close[n][t] != original[n].close[t]) {
                changed = true;
            }
        }

    EXPECT_TRUE(changed);
    free_ohlc_data(open, high, low, close, N);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
