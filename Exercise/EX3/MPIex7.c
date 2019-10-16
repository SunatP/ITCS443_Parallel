#include <stdio.h> 
#include <stdlib.h> 
#include <mpi.h>

int main(int argc, char *argv[]) { 
    int A[8][8];
    int i, j;
    int chunk_size;
    int rank, size;
    int local_sum = 0;
    int global_sum = 0;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    chunk_size = 8 / size;
    int local[8*chunk_size];

    if(rank == 0){
        srand(1234);
        for(i=0; i<8; i++){
            for(j=0; j<8; j++){
                A[i][j] = rand()%1000;
            }
        }
    }
    MPI_Scatter(A, 8*chunk_size, MPI_INT, local, 8*chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    for(i=0; i<8*chunk_size; i++){
        local_sum = local_sum + local[i];
    }
    printf("Rank: %d, sum = %d\n", rank, local_sum);

    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if(rank == 0){
		printf("Parallel total sum: %d\n", global_sum);	
	}
    MPI_Finalize();
}