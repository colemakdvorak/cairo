#include <gtest/gtest.h>
#include "ShufflePrice.h"
#include <vector>
#include <iostream>

// Helper to allocate and initialize a 2D C array
double** make_input_data(int N, int T) {
    double** data = new double*[N];
    for (int n = 0; n < N; ++n) {
        data[n] = new double[T];
        for (int t = 0; t < T; ++t) {
            data[n][t] = n * 10 + t;
        }
    }
    return data;
}

void free_input_data(double** data, int N) {
    for (int n = 0; n < N; ++n) {
        delete[] data[n];
    }
    delete[] data;
}

TEST(ShufflePriceTest, ShuffleChangesData) {
    int N = 2, T = 5, S = 1;
    double** input_data = make_input_data(N, T);

    // Copy original for comparison
    std::vector<std::vector<double>> original(N, std::vector<double>(T));
    for (int n = 0; n < N; ++n)
        for (int t = 0; t < T; ++t)
            original[n][t] = input_data[n][t];

    ShufflePrice shuffler(T, N, S, input_data, true, 42);
    shuffler.shuffle();

    // Print for visual inspection (optional)
    std::cout << "Shuffled data:\n";
    for (int n = 0; n < N; ++n) {
        for (int t = 0; t < T; ++t)
            std::cout << input_data[n][t] << " ";
        std::cout << std::endl;
    }

    // Check that at least one value changed
    bool changed = false;
    for (int n = 0; n < N; ++n)
        for (int t = 0; t < T; ++t)
            if (input_data[n][t] != original[n][t])
                changed = true;
    EXPECT_TRUE(changed);

    free_input_data(input_data, N);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
