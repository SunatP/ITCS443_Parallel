#include "mpi.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main(int argc,char *argv[]) {
    int total_size = 100, size = 25;
    int i=0,A[total_size],reciver[size],input=-1;
    int count=0;
    int abc; // define as global variable in every node
    int id,numprocs;
    int chunk_size;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&id);
    chunk_size = 100/ numprocs; // create chunk size for separate value to each node
    if(id ==0) 
    { 
        srand(100);
        for (i=0; i < total_size; i++)
        {
            A[i] = rand()%1000; // Random values in Process 0 (Rank 0)
        }
        printf("Input 1-1000 ");
        fflush(stdout);
        scanf("%d", &input);
    }
    MPI_Bcast(&input, 1, MPI_INT, 0, MPI_COMM_WORLD); // Process 0 must Bcast to all process
    MPI_Scatter(A,total_size/numprocs,MPI_INT,&reciver,chunk_size,MPI_INT,0,MPI_COMM_WORLD); // Use Scatter for separate processes to all process

    for(i=0;i<total_size/numprocs;i++) // use for loop for increment value
    {
        if(A[i]<input) count++; // increment value's count 
    }
    // After calculate complete
    MPI_Reduce(&count,&abc,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD); // use reduce to calculate sum value
        /**
         * send,receive,send as 1 time, int type, using sum, root as 0
         * **/ 
   if (id == 0)
   {
       printf("Total number that less than %d is %d\n",input,abc);
   }
    MPI_Finalize();
}