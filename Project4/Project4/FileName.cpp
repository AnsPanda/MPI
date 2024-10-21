#include <iostream>
#include <mpi.h>

using namespace std;

struct ProcData {
    int id;
    float weightedRank;
};

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int processRank, totalProcesses;
    MPI_Comm_rank(MPI_COMM_WORLD, &processRank);
    MPI_Comm_size(MPI_COMM_WORLD, &totalProcesses);

    ProcData currentData, incomingData;
    currentData.id = processRank;
    currentData.weightedRank = processRank * 1.1f;

    int aggregatedInt = currentData.id;
    float aggregatedFloat = currentData.weightedRank;

    MPI_Datatype customDataType;
    MPI_Aint typeOffsets[2] = { offsetof(ProcData, id), offsetof(ProcData, weightedRank) };
    int blockLengths[2] = { 1, 1 };
    MPI_Datatype dataTypes[2] = { MPI_INT, MPI_FLOAT };

    MPI_Type_create_struct(2, blockLengths, typeOffsets, dataTypes, &customDataType);
    MPI_Type_commit(&customDataType);

    int nextProcess = (processRank + 1) % totalProcesses;
    int previousProcess = (processRank - 1 + totalProcesses) % totalProcesses;

    for (int turn = 0; turn < totalProcesses - 1; ++turn) {
        MPI_Sendrecv(&currentData, 1, customDataType, nextProcess, 0,
            &incomingData, 1, customDataType, previousProcess, 0,

            MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        aggregatedInt += incomingData.id;
        aggregatedFloat += incomingData.weightedRank;

        currentData = incomingData;

        cout << "Turn " << turn << " - Process " << processRank
            << ": Int sum = " << aggregatedInt << ", Float sum = " << aggregatedFloat << endl;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    cout << "Final result at process " << processRank << ": Total int sum = " << aggregatedInt
        << ", Total float sum = " << aggregatedFloat << endl;

    MPI_Type_free(&customDataType);
    MPI_Finalize();
    return 0;
}
