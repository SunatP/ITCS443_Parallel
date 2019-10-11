# Lab 3 Collective Communication (MPI)

### ข้อ 1

```C
#include"mpi.h"
#include<stdio.h>
#include<math.h>
int main(int argc, char *argv[]) 
{ 
    int n, myid, numprocs, i; 
    double PI25DT = 3.141592653589793238462643; 
    double pi, w, sum, x, a; 
    MPI_Init(&argc, &argv); 
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs); 
    MPI_Comm_rank(MPI_COMM_WORLD, &myid); 
    if (myid == 0) // ถ้าเป็นโปรเซสที่ 0
    { 
        printf("Enter the number of intervals: (0 quits) "); 
        fflush(stdout);  /* Force screen output now */
        scanf("%d", &n); /* Only process 0 can use scanf */
    } 
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD); // ประกาศค่า แล้วส่งแค่ 1 ค่าไปให้ทุกโปรเซส
    if (n > 0) // ถ้าโปรเซสไม่ใช่ 0
    { 
        w = 1.0 / (double)n; sum = 0.0; 
        for (i = myid + 1; i <= n; i += numprocs) 
        { 
            x = w * ((double)i - 0.5); 
            sum += w*4.0 / (1.0 + x * x); 
        } 
        MPI_Reduce(&sum, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD); // รวมค่ามาที่โปรเซสเดียว
        if (myid == 0) 
        printf("pi is approximately %.16f, Error is %.16f\n",  pi, fabs(pi - PI25DT)); 
        } 
    MPI_Finalize(); // จบการทำงานของ MPI
}
```

### ข้อ 2. แก้จากแลปที่แล้วให้

```C
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
	{
        /* for last block */
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
```