#include <iostream>
#include "mpi.h"
#include <cmath>
#include <time.h>
using namespace std;

int main(int argc, char** argv) {
    int ProcNum, ProcRank;
    double local_product = 1.0, global_product = 1.0;
    double MyPi = 0;
    double TotalTime;

    long long N = 1000; 
    int n = 10; 

    if (argc > 1) {
        N = atoll(argv[1]); 
    }
    if (argc > 2) {
        n = atoi(argv[2]); 
    }

    clock_t StartClock, EndClock; 
    StartClock = clock(); 

    MPI_Init(&argc, &argv); 
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum); 
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    for (long long k = ProcRank + 1; k <= N; k += ProcNum) {
        local_product *= (static_cast<double>(k * k)) / (static_cast<double>(k * k) - 0.25);
    }

    MPI_Reduce(&local_product, &global_product, 1, MPI_DOUBLE, MPI_PROD, 0, MPI_COMM_WORLD); 

    


    if (ProcRank == 0) {
        EndClock = clock(); 

        global_product = pow(global_product, 1.0 / ProcNum); 
        MyPi = 4 * (sqrt(2) / 2) * global_product; 

        printf("Number of iterations = %lld\n", N);
        printf("Obtained value of Pi = %.*f\n", n, MyPi); 

        double reference_pi = 3.1415926535897932384626433832795; 
        printf("Reference value of Pi = %.*f\n", n, reference_pi);

        TotalTime = (double)(EndClock - StartClock) / CLOCKS_PER_SEC; 
        printf("Computing time = %f sec\n", TotalTime);
    }
    return 0;
}

