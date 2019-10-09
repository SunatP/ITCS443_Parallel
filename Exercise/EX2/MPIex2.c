#include <stdio.h>
#include <mpi.h>
#include <math.h>

int main(int argc, char *argv[])
{
    int rank,size,x,square;
    MPI_Status status;
    MPI_Init (&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // msgtag = 1234;
    if(rank == 0){
        // printf("Enter the number: ");
        // fflush(stdout);
        scanf("%d", &x); // receive data
        MPI_Send(&x, 1, MPI_INT, 1, 1234, MPI_COMM_WORLD); // Send parameter 
        printf("processor %d sent %d\n", rank, x);
        MPI_Recv(&square, 1, MPI_INT, 1, 1234, MPI_COMM_WORLD, &status); // Like math pow
        printf("processor %d got %d\n", rank, square);
    }
    if(rank == 1){
        MPI_Recv(&x, 1, MPI_INT, 0, 1234, MPI_COMM_WORLD, &status); // Receive parameter value
        printf("processor %d got %d\n", rank, x);
        square = x*x;
        MPI_Send(&square, 1, MPI_INT, 0, 1234, MPI_COMM_WORLD); // Send Parameter back
        printf("processor %d sent %d\n", rank, square);
    }

    MPI_Finalize(); // End MPI Process

    return 0;
}