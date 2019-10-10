#include "mpi.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>


int main(int argc,char *argv[]) {
    int i=0,total_size = 100, size = 25,reciver[size];
    int min[size],max[size],sum[size],avg=0;
    int A[total_size];
    int rank, numprocs;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    //int chunk_size=100/numprocs;
    if(rank == 0)
    {
        srand(1234);
        for (i=0; i < total_size; i++)
        {
            A[i] = rand()%1000;
            //printf("%d  \n",A[i]);
        }
    }
    MPI_Scatter(A,size,MPI_INT,&reciver,size,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Reduce ( &reciver, &min, size, MPI_INT,MPI_MIN, 0,MPI_COMM_WORLD );
    MPI_Reduce ( &reciver, &max, size, MPI_INT,MPI_MAX, 0,MPI_COMM_WORLD );
    MPI_Reduce ( &reciver, &sum, size, MPI_INT,MPI_SUM, 0,MPI_COMM_WORLD );
    if(rank==0)
    {
        printf("Local maximum: ");
        for (i=0; i < size; i++) printf("%d\n",max[i]);
        printf("\n");
        printf("Local minimum: ");
        for (i=0; i < size; i++) printf("%d\n",min[i]);
        printf("\n");
        printf("Average (sum): ");
        for (i=0; i < size; i++) printf("%d\n",sum[i]/size);
        printf("\n");

        int lastmax=A[0];
        int lastmin=A[0];
        int lastsum=0;
        for(i=0;i<total_size;i++){
            if(A[i]>lastmax){
                lastmax=A[i];
            }
            if(A[i]<lastmin){
                lastmin=A[i];
            }
            lastsum+=A[i];
        }
        printf("Max of A %d\n",lastmax);
        printf("Min of A %d\n",lastmin);
        printf("Average (sum) of A %d\n",lastsum/total_size);
    }

    MPI_Finalize();

}
