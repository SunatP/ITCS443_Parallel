#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#define  TRUE 1
#include "math.h"
#include <time.h>
int IncOrder(const void *e1, const void *e2 )
    {
        return (*((int *)e1) - *((int *)e2));
    }

int DisplayError(char *str)
{
    printf("Error: %s \n",str);
}

int Partition(int *arr, int left, int right) {
    int i = left, j = right;
    int tmp;
    int pivot = arr[(left + right) / 2];
    /* partition */
    while (i <= j) 
    {
    while (arr[i] < pivot)
    i++;
    while (arr[j] > pivot)
        j--;
    if (i <= j) 
    {
        tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
        i++;
        j--;
    }
    }
    return j;
}

int PowerOf2(int num)
{
    int i;
    i=1;
    while(num>0)
    {
        num--;
        i=i*2;
    }
    return i;
}


int PQuickSort(int *Array,int start,int end,int m,int id,int MyRank)
{
    int j;
    int r;
    int LocalLength;
    int *tmp;
    //int newsize;
    MPI_Status status;
    LocalLength=-1;

    if(m==0)
{
    if(MyRank==id)

    qsort(Array, end-start, sizeof *Array, IncOrder);
    return 0;
}

if(MyRank==id)
{
    r=Partition(Array,start,end);
    LocalLength=end-r;
MPI_Send(&LocalLength,1,MPI_INT,id+PowerOf2(m-1),MyRank,MPI_COMM_WORLD);

if(LocalLength!=0)

MPI_Send(Array+r+1,LocalLength,MPI_INT,id+PowerOf2(m-1),MyRank,MPI_COMM_WORLD);
    }

    if(MyRank==id+PowerOf2(m-1))
    {
        MPI_Recv(&LocalLength,1,MPI_INT,id,id,MPI_COMM_WORLD,&status);

       if(LocalLength!=0)
        {
             tmp=(int *)malloc(LocalLength*sizeof(int));
             if(tmp==0)
                 DisplayError("Malloc memory error!");

MPI_Recv(tmp,LocalLength,MPI_INT,id,id,MPI_COMM_WORLD,&status);
        }
    }

    j=r-1-start;
    MPI_Bcast(&j,1,MPI_INT,id,MPI_COMM_WORLD);
    if(j>0)
        PQuickSort(Array,start,r-1,m-1,id,MyRank);

    j=LocalLength;
    MPI_Bcast(&j,1,MPI_INT,id,MPI_COMM_WORLD);
    if(j>0)
        PQuickSort(tmp,0,LocalLength-1,m-1,id+PowerOf2(m-1),MyRank);

    if((MyRank==id+PowerOf2(m-1)) && (LocalLength!=0))

MPI_Send(tmp,LocalLength,MPI_INT,id,id+PowerOf2(m-1),MPI_COMM_WORLD);

    if((MyRank==id) && (LocalLength!=0))
        MPI_Recv(Array+r+1,LocalLength,MPI_INT,id+PowerOf2(m-1),id+PowerOf2
(m-1),MPI_COMM_WORLD,&status);

}

int LogBase2(int num)
{
    int i, j;
    i=1;
    j=2;
    while(j<num)
    {
        j=j*2;
        i++;
    }
    if(j>num)
        i--;
    return i;
}


int main(int argc,char *argv[])
{
    int     ArraySize;
    int     *Array;
    int    MyRank, npes;
    int     i;
    int     m ;
    double  t1,t2,t3;
    
    
    //MPI_Status status;
    MPI_Init(&argc,&argv);

    MPI_Comm_rank(MPI_COMM_WORLD,&MyRank);

    MPI_Comm_size(MPI_COMM_WORLD,&npes);

    if(MyRank==0)
    {
        printf("Initial Quick Sort with MPI\n");
        ArraySize=1000000;
        printf("Array size is allocate for %d values \n ",ArraySize);
        Array=(int *)malloc(ArraySize*sizeof(int));

        if(Array==0)
            printf("Malloc memory error!");

        srand(1432427398);
        for(i=0;i<ArraySize;i++)
        {
            Array[i]=(int)rand()%10000;
            // printf("%d \n", Array[i]);
        }    
        printf("\n");
    }
    //int arr[ArraySize];
    m=LogBase2(npes);
    // MPI_Scatter(&ArraySize,1,MPI_INT,&arr,1,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Bcast(&ArraySize,1,MPI_INT,0,MPI_COMM_WORLD);
        t1=MPI_Wtime();
    PQuickSort(Array,0,ArraySize-1,m,0,MyRank);
        t2=MPI_Wtime();
        t3=t2-t1;

    if(MyRank==0)
    {
        for(i=0;i<ArraySize;i++)
        {
            printf("%10d",Array[i-1]);
                // printf("\t");
        }
        printf("\n");
    printf("Time used :%6.3f seconds \n",t3);

    }

    MPI_Finalize();
    return 0;
}