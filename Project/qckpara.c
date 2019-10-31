#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#include "math.h"
#include <time.h>
// #define N 1000000
#define  TRUE 1
// Number of sorting runs
#define NUM 4 // We Change from 10 to 4 
static int N = 10000000;
/**------------------ Sequential QUICKSORT ---------------------**/
void display(int arr[], int n)
{

    int i;
    for (i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }

    printf("\n");
}

/*Swap function to swap two values*/
void swap(int *first, int *second)
{

    int temp = *first;
    *first = *second;
    *second = temp;
}

/*Partition method which selects a pivot
  and places each element which is less than the pivot value to its left
  and the elements greater than the pivot value to its right
  arr[] --- array to be partitioned
  lower --- lower index 
  upper --- upper index
*/
int partitionqck(int arr[], int lower, int upper)
{

    int i = (lower - 1);

    int pivot = arr[upper]; // Selects last element as the pivot value

    int j;
    for (j = lower; j < upper; j++)
    {

        if (arr[j] <= pivot)
        { // if current element is smaller than the pivot

            i++; // increment the index of smaller element
            swap(&arr[i], &arr[j]);
        }
    }

    swap(&arr[i + 1], &arr[upper]); // places the last element i.e, the pivot to its correct position

    return (i + 1);
}

/*This is where the sorting of the array takes place
	arr[] --- Array to be sorted
	lower --- Starting index
	upper --- Ending index
*/
void QuickSort(int arr[], int lower, int upper)
{

    if (upper > lower)
    {

        // partitioning index is returned by the partition method , partition element is at its correct poition

        int partitionIndex = partitionqck(arr, lower, upper);

        // Sorting elements before and after the partition index
        QuickSort(arr, lower, partitionIndex - 1);
        QuickSort(arr, partitionIndex + 1, upper);
    }
}



/**------------------ Parallel QUICKSORT with MPI ---------------------**/
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
    
    //int arr[N], dup[N];
	
	// seed for random input
	srand(time(NULL));
 
	// to measure time taken by optimized and non-optimized Quicksort 
	clock_t begin, end;
	double time1 = 0.0, time2 = 0.0;
    int     ArraySize;
    int     *Array;
    int    MyRank, npes;
    int     i;
    int     m ;
    double  t1,t2,t3;
    

    int *arr = (int *)malloc(sizeof(int) * N);
    MPI_Status status;
    MPI_Init(&argc,&argv);

    MPI_Comm_rank(MPI_COMM_WORLD,&MyRank);

    MPI_Comm_size(MPI_COMM_WORLD,&npes);
		
    if(MyRank==0)
    {
        printf("Initial Quick Sort with MPI\n");
        ArraySize=1000000;
        printf("Array size is allocate for %d values \n ",N);
        Array=(int *)malloc(N*sizeof(int));

        if(Array==0)
            printf("Malloc memory error!");

        srand(1432427398);
        for(i=0;i<N;i++)
        {
            arr[i]=(int)rand()%10000;
            // printf("%d \n", Array[i]);
        }    
        printf("\n");
    }
    //int arr[ArraySize];
    m=LogBase2(npes);
    // MPI_Scatter(&ArraySize,1,MPI_INT,&arr,1,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Bcast((int *)&N,1,MPI_INT,0,MPI_COMM_WORLD);
    t1=MPI_Wtime();
    PQuickSort(arr,0,N-1,m,0,MyRank);
    t2=MPI_Wtime();
    t3=t2-t1;

    if(MyRank==0)
    {
        // for(i=0;i<ArraySize;i++)
        // {
        //     // printf("%10d",Array[i-1]);
        //         // printf("\t");
        // }
	
	
        // printf("\n");
		
	// generate random input
	for (int i = 0; i < N; i++)
		 arr[i] = rand() % 10000;

	// Perform non-optimized Quicksort on arr
	begin = clock();
	QuickSort(arr, 0, N-1);
	end = clock();

	// calculate time taken by Non-Optimized QuickSort
	time1 = (double)(end - begin) / CLOCKS_PER_SEC;
     

	// calculate time taken by optimized QuickSort
	// time2 += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Process %d Receive Result\n",MyRank);
    printf("Average time taken by Parallel Quicksort :%6.3f seconds \n",t3);
    printf("Average time taken by Seqential Quicksort: %f seconds\n",time1);
    }
	printf("Process %d \n",MyRank);
    MPI_Finalize();
    
    
    return 0;
}