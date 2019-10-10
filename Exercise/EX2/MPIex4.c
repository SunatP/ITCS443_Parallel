#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int totalSize = 8; // Declare slave value as 8
    int mpiCommSize;
    int rank; 
    int i, j;

    MPI_Status status;
    int data[totalSize][totalSize];
    int receive[totalSize];

    int COMM_TAG = 1234;

    int localSum, total = 0;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &mpiCommSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) 
    {
        for (i = 0; i < totalSize; i++) // use for loop to increment value from 0 to 8 (7 elements)
        {
            for (j = 0; j < totalSize; j++) 
            {
                data[i][j] = i + j;
            }
        }

        for (i = 1; i <= totalSize; i++) // use for loop to send and receive value to each slave
        {
            MPI_Send(&data[i - 1], totalSize, MPI_INT, i, COMM_TAG, MPI_COMM_WORLD);
            MPI_Recv(&localSum, 1, MPI_INT, i, COMM_TAG, MPI_COMM_WORLD, &status);
            total += localSum;
        }
        printf("%d\n", total);

    } 
    else 
    {
        localSum = 0;
        MPI_Recv(&receive, totalSize, MPI_INT, 0, COMM_TAG, MPI_COMM_WORLD, &status); // Receive value from each slave
        for (i = 0; i < totalSize; i++) // using for loop to calculate total value from each value
        {
            localSum += receive[i];
        }
        MPI_Send(&localSum, 1, MPI_INT, 0, COMM_TAG, MPI_COMM_WORLD); // send value to each slave
    }
    MPI_Finalize(); // End MPI process
}