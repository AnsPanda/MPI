#include <mpi.h>
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int input = 0;
    int receivedValue1 = 0, receivedValue2 = 0;

    if (rank == 0) {
        input = 73;
        cout << "Process " << rank << " otpravla " << input << " procedure 3." << endl;
        MPI_Send(&input, 1, MPI_INT, 3, 0, MPI_COMM_WORLD);

        MPI_Recv(&receivedValue1, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        cout << "Process " << rank << " poluchil " << receivedValue1 << " ot processa 1." << endl;

        MPI_Recv(&receivedValue2, 1, MPI_INT, 3, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        cout << "Process " << rank << " poluchil " << receivedValue2 << " ot processa 3." << endl;

        int sum = receivedValue1 + receivedValue2;
        cout << "Summa: " << sum << endl;

    }
    else if (rank == 1) {
        MPI_Recv(&input, 1, MPI_INT, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        cout << "Process " << rank << " poluchil " << input << " ot processa 2." << endl;

        MPI_Send(&input, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        cout << "Process " << rank << " otpravlaet " << input << " procedure 0." << endl;

    }
    else if (rank == 2) {
        input = 5;
        MPI_Send(&input, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        cout << "Process " << rank << " otpravlaet " << input << " procedure 1." << endl;

    }
    else if (rank == 3) {
        MPI_Recv(&input, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        cout << "Process " << rank << " poluchil " << input << " ot processa 0." << endl;

        MPI_Send(&input, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        cout << "Process " << rank << " otpravlaet " << input << " procedure 0." << endl;
    }

    MPI_Finalize();
    return 0;
}



