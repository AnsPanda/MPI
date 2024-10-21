#include <iostream>
#include "mpi.h"
#include <cmath>
#include <time.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;

float fa(float x) {
    return exp((-1 * tan((0.84 * x)))) / (0.35 + cos(x));
}

float trapecoidal(float a, float b, int n) {
    float sum, h = (b - a) / n;
    sum = 0.5 * (fa(a) + fa(b));
    for (int i = 1; i < n; i++) {
        sum += fa(a + i * h);
    }
    return h * sum;
}

float simpson(float a, float b, int n) {
    float sum, h = (b - a) / n;
    sum = fa(a) + fa(b);
    for (int i = 1; i < n; i += 2) {
        sum += 4 * fa(a + i * h);
    }
    for (int i = 2; i < n - 1; i += 2) {
        sum += 2 * fa(a + i * h);
    }
    return h * sum / 3;
}

int main(int argc, char** argv) {
    int ProcNum, ProcRank;
    double a = 0, b = M_PI / 2;
    double sum_local = 0.0, sum_total = 0.0, TotalTime;
    int n = 1000000;
    double local_a, local_b, h;
    int local_n;

    clock_t StartClock, EndClock;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    h = (b - a) / n;
    local_n = n / ProcNum;
    local_a = a + ProcRank * local_n * h;
    local_b = local_a + local_n * h;

    StartClock = clock();
    sum_local = trapecoidal(local_a, local_b, local_n);
    MPI_Reduce(&sum_local, &sum_total, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    EndClock = clock();

    if (ProcRank == 0) {
        TotalTime = (double)(EndClock - StartClock) / CLOCKS_PER_SEC;
        cout << "Trapecia: " << sum_total << "\n";
        cout << "Trapecia Time (Parallel): " << TotalTime << " sec. \n";
    }

    StartClock = clock();
    sum_local = simpson(local_a, local_b, local_n);
    MPI_Reduce(&sum_local, &sum_total, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    EndClock = clock();

    if (ProcRank == 0) {
        TotalTime = (double)(EndClock - StartClock) / CLOCKS_PER_SEC;
        cout << "Simpson: " << sum_total << "\n";
        cout << "Simpson Time (Parallel): " << TotalTime << " sec. \n";
    }

    MPI_Finalize();

    if (ProcRank == 0) {
        StartClock = clock();
        double seq_trap = trapecoidal(a, b, n);
        EndClock = clock();
        TotalTime = (double)(EndClock - StartClock) / CLOCKS_PER_SEC;
        cout << "Trapecia (Sequential): " << seq_trap << "\n";
        cout << "Trapecia Time (Sequential): " << TotalTime << " sec. \n";

        StartClock = clock();
        double seq_simp = simpson(a, b, n);
        EndClock = clock();
        TotalTime = (double)(EndClock - StartClock) / CLOCKS_PER_SEC;
        cout << "Simpson (Sequential): " << seq_simp << "\n";
        cout << "Simpson Time (Sequential): " << TotalTime << " sec. \n";
    }

    return 0;
}
