#ifndef SHUFFLE_BAR_H
#define SHUFFLE_BAR_H

class ShuffleBar {
public:
    ShuffleBar(int T_, int N_, int pivot_, double **open_, double **high_, double **low_, double **close_, int seed_ = -1);
    ~ShuffleBar();

    void shuffle();

private:
    int T;
    int N;
    int pivot;
    int seed;
    bool initialized;

    double **open_ptr;
    double **high_ptr;
    double **low_ptr;
    double **close_ptr;

    double *basis_open;
    double *basis_high;
    double *basis_low;
    double *basis_close;

    double **rel_open;
    double **rel_high;
    double **rel_low;
    double **rel_close;
};

#endif // SHUFFLE_BAR_H
