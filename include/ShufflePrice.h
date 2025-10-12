class ShufflePrice {
public:
    ShufflePrice(int T, int N, int S, double **output_buffer, bool init, int seed);
    ~ShufflePrice();
    void shuffle();

private:
    bool initialized; // initialization flag
    int T; // maximum time in time series
    int N; // number of market
    int pivot; // the starting point of time series
    int seed; // randomization seed
    double *basis; // Initial vector of asset prices at time pivot
    double **input_data; // N X T time series data
    double **delta; // 
};
