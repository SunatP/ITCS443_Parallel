#include <stdio.h>
#include <mpi.h>
#include <math.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int msgtag;
    int x[10], i, mul[10];
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    msgtag = 1234;
    if (rank == 0) {
        for (i = 0; i < 10; i++) {
            // printf("Enter the number %d: ", i + 1);
            // fflush(stdout);
            scanf("%d", &x[i]);
            printf("%d \n",x[i]);
        }
        MPI_Send(x, 10, MPI_INT, 1, msgtag, MPI_COMM_WORLD);
//            printf("processor %d sent %d\n", rank, x);
        MPI_Recv(mul, 10, MPI_INT, 1, msgtag, MPI_COMM_WORLD, &status);
        for (i = 0; i < 10; i++) {
            printf("processor %d got %d\n", rank, mul[i]);
        }
    }
    if (rank == 1) {
        MPI_Recv(x, 10, MPI_INT, 0, msgtag, MPI_COMM_WORLD, &status);
//            printf("processor %d got %d\n", rank, x);
        for (i = 0; i < 10; i++) {
            mul[i] = x[i] * 10;
        }
        MPI_Send(mul, 10, MPI_INT, 0, msgtag, MPI_COMM_WORLD);
//            printf("processor %d sent %d\n", rank, mul);
    }

    MPI_Finalize();

    return 0;
}