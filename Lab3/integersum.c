#include <stdio.h>
#include <mpi.h>
int main(int argc, char *argv[])
{
 int rank, size,LEFT,RIGHT;
 MPI_Status status;
 int interval;
 int number, start, end, sum, GrandTotal;
 int proc;
 MPI_Init( &argc, &argv ); 
 MPI_Comm_rank( MPI_COMM_WORLD, &rank );
 MPI_Comm_size( MPI_COMM_WORLD, &size );
  

 if (rank == 0) 
 {
	printf("Enter the number of left:");
 	fflush(stdout);
 	scanf("%d", &LEFT); 
 	printf("Enter the number of right:");
 	fflush(stdout); 
 	scanf("%d", &RIGHT);
 	GrandTotal = 0;
 	
 
 }
 MPI_Bcast(&LEFT, 1, MPI_INT, 0, MPI_COMM_WORLD); 
 MPI_Bcast(&RIGHT, 1, MPI_INT, 0, MPI_COMM_WORLD); 
 if(rank>0) 
 {
 	interval=(RIGHT-LEFT+1)/(size-1);
 	start=(rank-1)*interval+LEFT;
 	end=start+interval-1;
 	if (rank == (size-1)) 
	{/* for last block */
	    end = RIGHT;
 	}
 	sum=0; /*Sum locally on each proc*/
	 
	for (number=start; number<=end; number++)
 	{
		sum = sum+number;
	}
	printf("Rank %d : start %d end %d local sum %d \n",rank,start,end,sum);
 	/*send local sum to Master process*/

 }
MPI_Reduce(&sum, &GrandTotal, 1, MPI_INT, MPI_SUM, 0,MPI_COMM_WORLD); 
if(rank==0)
printf("Grand total = %d \n", GrandTotal);
 MPI_Finalize();
} 