#include <stdio.h> 
#include <stdlib.h> 
#include <mpi.h>

int main(int argc, char *argv[]) { 
    int A[100];
    int local_A[25];
    int B[100];
    int local_B[25];
    int C[100];
    int local_C[25];
    int i;
    int chunk_size;
    int rank, size;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    chunk_size = 100 / size;
    if(rank == 0){
        srand(1234);
        for(i=0; i<100; i++){
            A[i] = rand()%1000;
            B[i] = rand()%1000;
        }
    }

    MPI_Scatter(A, chunk_size, MPI_INT, local_A, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(B, chunk_size, MPI_INT, local_B, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    printf("Rank %d, local_A & local_B = A & B[%d to %d]\n", rank, rank*chunk_size, (rank+1)*chunk_size);
    for(i=0; i<chunk_size; i++){
        local_C[i] = local_A[i] + local_B[i];
    }

    MPI_Gather(local_C, chunk_size, MPI_INT, C, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);
    if(rank == 0){
        		
		for(i=0;i<100;i++){
            printf("%d ", C[i]);
        }
    }
    MPI_Finalize();
}