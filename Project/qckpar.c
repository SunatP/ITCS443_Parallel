/**
 * Parallel Quicksort via MPI
 * Coding by Visual Studio Code
 * Tested by MPI Lab from MUICT and Ubuntu 19.10 based on Virtual Machine
 * Nutgamol Mahachai 6088061
 * Poonkasem Kasemsri Na Ayutthaya 6088071
 * Sunat Praphanwong 6088130
 * Barameerak Koonmongkon 6088156
**/
  #include <mpi.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include "math.h"
  #include <time.h>
  #ifdef _WIN32
  #include <Windows.h>
  #else
  #include <unistd.h>
  #endif
  #include <stdio.h>

  // #define N 1000000
  #define X 10000
  #define  TRUE 1
  // Number of sorting runs
  #define NUM 4 // We Change from 10 to 4 
int N;
  void clrscr(void)
  {
      system("clear");
  }
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
  void swapqck(int *first, int *second)
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
              swapqck(&arr[i], &arr[j]);
          }
      }

      swapqck(&arr[i + 1], &arr[upper]); // places the last element i.e, the pivot to its correct position

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

  /**------------------ Recursive Zone ---------------------**/
  void quicksort(int *, int, int); 
  int partition(int *, int, int);
  int choosePivot(int *, int, int);
  void swap(int *, int *);
  /**------------------ Recursive Zone ---------------------**/

  int main(int argc, char ** argv)
  {
    int rank, size, pivot, partner, recvSize;
    double start, end;
    MPI_Status status;

    MPI_Init(&argc, &argv); //  initialize the MPI execution environment
    N = atoi(argv[1]);
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Returns the rank of the calling process in the communicator
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // determines the number of processes 

    srand(1432427398); // Seed Random
    int * newArr;
    int * arr = (int *) malloc(sizeof(int)*N/size *(size - 1)); 
    // Create Array to allocate the memory by using N size multiply by sizeof integer
    // int * arr = (int *) malloc(sizeof(int)*size *(size-1));
    // int * arr = (int *) malloc(sizeof(int)*(size - 1));
    
    int * recvBuffer = (int *) malloc(sizeof(int)*(N/size )* (size - 1) );
  // Create Buffer to allocate the memory by using N size multiply by sizeof integer
    int i, j;
    for(i = 0; i < N/size; i++)
      arr[i] = rand()%X; // Mod value with range 0 - 9999

    
    if(rank == 0) // Rank 0 is Master Process
    {
      clrscr();
      printf("Initial Quick Sort with MPI\n");
      sleep(3);
      fflush(stdout);
      printf("---------- Our Group Member ----------\n");
      printf(" Nutgamol M. 6088061 \n Poonkasem K. 6088071 \n Sunat P. 6088130 \n Barameerak K. 6088156\n");
      printf("---------- Our Group Member ----------\n");
      sleep(3);
      printf("Initial Data with allocate memory %d sizes\n",N);
      sleep(1.5);
      start = MPI_Wtime(); // MPI_Wtime() is Wall clock time use to stopwatch
      pivot = choosePivot(arr, 0, N/size-1); // Select value by using choosePivot Function
    }


    MPI_Bcast(&pivot, 1, MPI_INT, 0, MPI_COMM_WORLD); // Broadcast process to slave process

    //Assume that the number of processes is a power of 2

    int storeIdx = 0;
    int arrSize = N/size;

    for(partner = size/2; partner > 0; partner = partner >> 1)
    {
      storeIdx = 0;
      for(i = 0; i < arrSize; i++)
      {
        if(arr[i] < pivot)
        {
          swap(&arr[i], &arr[storeIdx]);
          storeIdx++;
        }
      }
      // printf("storeIdx = %d in process %d\n", storeIdx, rank);
  // 0...7 8...15 16/2 = 8
  //
  // rank / (size/partner) 
  // 0 vs 1
  //
  // 0..3   4..7   8..11 12..15 16/4 = 4
  // lower upper   lower upper
  // 0      1       2     3
      int flag = 0;
      MPI_Request request, requestSend;
      // MPI_REQUEST represents a handle on a non-blocking operation to know when the non-blocking operation handled completes
      if( (rank / partner) % 2 == 0) // if Value is master (0)
      {

      //   printf("rank + partner is %d\n", rank + partner);

        int sendVal = arrSize-storeIdx;
        recvSize = 0;
        MPI_Isend(&sendVal, 1, MPI_INT, rank+partner, partner+size, MPI_COMM_WORLD, &requestSend);
        MPI_Irecv(&recvSize, 1, MPI_INT, rank+partner, partner+size, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, &status);


        if(arrSize-storeIdx > 0)
        {
          MPI_Isend(arr+storeIdx, arrSize - storeIdx, MPI_INT, rank + partner, partner,MPI_COMM_WORLD, &requestSend);
        }
      // MPI_Isend is immediate return value when compare MPI_Send
      //   printf("recvsize is %d in process %d\n", recvSize, rank);

        if(recvSize > 0) // If recvSize has a value
        {
          free((void *) recvBuffer); // Use function Free to release memory
          recvBuffer = (int *) malloc(sizeof(int)*recvSize); // Allocate Memory for RecvBuffer

          MPI_Irecv(recvBuffer, recvSize, MPI_INT, rank + partner, partner,
                          MPI_COMM_WORLD, &request);
          // MPI Receive with Immediate return
          MPI_Wait(&request, &status); //  returns when the operation identified by request is complete
        }
      }
      else // If Rank is Slave
      {
        int sendVal = storeIdx;
        recvSize = 0;
        MPI_Isend(&sendVal, 1, MPI_INT, rank-partner, partner+size, MPI_COMM_WORLD, &requestSend);
        MPI_Irecv(&recvSize, 1, MPI_INT, rank-partner, partner+size, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, &status);
      //   printf("rank - size is %d\n", rank - partner);

        if(storeIdx > 0)
        {
          MPI_Isend(arr, storeIdx, MPI_INT, rank - partner, partner, MPI_COMM_WORLD, &requestSend);
        }

      //   printf("recvsize is %d in process %d\n", recvSize, rank);
        if(recvSize > 0)
        {
          free((void *) recvBuffer);
          recvBuffer = (int *) malloc(sizeof(int)*recvSize);

          MPI_Irecv(recvBuffer, recvSize, MPI_INT, rank - partner, partner,
                          MPI_COMM_WORLD, &request);

          MPI_Wait(&request, &status);
        }
      }

      MPI_Barrier(MPI_COMM_WORLD); // Blocks until all processes in the communicator have reached this routine
      if(recvSize > 0)
      {
        // Merge arrays
        if((rank / partner) % 2 == 0) // Keep smaller elements
        {
          newArr = (int *) malloc(sizeof(int)*(recvSize+storeIdx));
          for(i = 0; i < storeIdx; i++)
            newArr[i] = arr[i];
          for(j = 0, i = storeIdx; i < recvSize+storeIdx; i++, j++)
            newArr[i] = recvBuffer[j];
          free((void *) arr);
          arr = newArr;
          newArr = NULL;
          arrSize = recvSize+storeIdx;
        }
        else  // Keep larger elements
        {
          newArr = (int *) malloc(sizeof(int)*(recvSize+(arrSize-storeIdx)));
          for(j = 0, i = storeIdx; i < arrSize; i++, j++)
            newArr[j] = arr[i];
          for(j = 0, i = arrSize-storeIdx; i < recvSize+(arrSize-storeIdx); i++, j++)
            newArr[i] = recvBuffer[j];
          free((void *) arr);
          arr = newArr;
          newArr = NULL;
          arrSize = recvSize+(arrSize-storeIdx);
        }
      }
      else
      {
        arrSize = 0;
      }

      if(rank % partner == 0) // Master Rank (0)
      {
        pivot = choosePivot(arr, 0, arrSize-1);
      
        for(i = 1; i < partner; i++)
          MPI_Send(&pivot, 1, MPI_INT, rank+i, partner+1, MPI_COMM_WORLD);
      }
      else // Slave Process
      {
        MPI_Recv(&pivot, 1, MPI_INT, partner*(rank/partner), partner+1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      }

    }

    if(arrSize > 0)
      quicksort(arr, 0, arrSize-1);

    int * sizeArr, * fullArr, * displacement;
    if(rank == 0)
    {
      sizeArr = (int *) malloc(sizeof(int)*size);
      fullArr = (int *) malloc(sizeof(int)*N);
      displacement = (int *) malloc(sizeof(int)*size);
    }

    MPI_Gather(&arrSize, 1, MPI_INT, sizeArr, 1, MPI_INT, 0, MPI_COMM_WORLD);
  // spreading elements from one process to many processes
    if(rank == 0)
    {
      i = 0;
      displacement[0] = 0;
      // printf("displacement[%d] = %d\n", i, displacement[i]);
      // Perform a scan on sizeArr to determine the displacement of each data location.
      for(i = 1; i < size; i++)
      {
        displacement[i] = sizeArr[i-1] + displacement[i-1];
      //   printf("displacement[%d] = %d\n", i, displacement[i]);
      }
    }

    MPI_Gatherv(arr, arrSize, MPI_INT, fullArr, sizeArr, displacement, MPI_INT, 0, MPI_COMM_WORLD);
    // MPI_Gatherv allowing a varying count of data from each process, since recvcounts is now an array
    MPI_Barrier(MPI_COMM_WORLD);
    if(arrSize > 0)
      free((void *) arr);

    if(rank == 0)
    {
      end = MPI_Wtime();
      // printf("Time required was %lf seconds\n", end-start);
      printf("Average time taken by Parallel Quicksort: %f seconds\n", end-start);
      free((void *) fullArr);
      free((void *) sizeArr);
      free((void *) displacement);
      // int arr[N], dup[N];
      int *arr = (int *)malloc(sizeof(int) * N);
    // seed for random input
      srand(1432427398);
  
    // to measure time taken by optimized and non-optimized Quicksort 
      clock_t begin, endseq;
      double time1 = 0.0, time2 = 0.0,ratio = 0.0,diff = 0.0;

      // generate random input
      for (int i = 0; i < N; i++)
      {
        arr[i] = rand() % 10000;
      // printf("%d ",arr[i]);
      }
        
      // Perform non-optimized Quicksort on arr
      begin = clock();
      // display(arr,N);
      QuickSort(arr, 0, N-1);
      // display(arr,N);
      endseq = clock();

      // calculate time taken by Non-Optimized QuickSort
      time1 = (double)(endseq - begin) / CLOCKS_PER_SEC;
      ratio = time1 / (double)(end - start) ;
      diff = time1 - (end - start);
    printf("Process Used in Sequential : %d \n",rank);
    printf("Average time taken by Sequential Quicksort: %f seconds\n",time1);
    printf("Difference time is %f seconds (In Parallel Quicksort is %.2f times faster than Sequential)\n",diff,ratio);
    sleep(1.5);
    printf("Quicksort with Sequential and Parallel Finished\n");
    sleep(1.5);
    printf("Press Enter to Exit...");
    fflush(stdout);
    getchar();
    }
    if(rank!= 0){printf("Process Used in Parallel : %d \n",rank);}
    MPI_Finalize();

    return 0;
  }
  /**---------- Quicksort for Parallel ----------**/
  void quicksort(int * arr, int lo, int hi)
  {
    if(lo < hi)
    {
      int p = partition(arr, lo, hi);
      quicksort(arr, lo, p - 1);
      quicksort(arr, p + 1, hi);
    }
  }

  int partition(int * arr, int lo, int hi)
  {
    int i;
    int pivotIdx = choosePivot(arr, lo, hi);
    int pivotVal = arr[pivotIdx];

    swap(&arr[pivotIdx], &arr[hi]);

    int storeIdx = lo;

    for(i = lo; i < hi; i++)
    {
      if(arr[i] < pivotVal)
      {
        swap(&arr[i], &arr[storeIdx]);
        storeIdx++;
      }
    }

    swap(&arr[storeIdx], &arr[hi]);
    return storeIdx;
  }

  void swap(int * x, int * y)
  {
    int temp = *x;
    *x = *y;
    *y = temp;
  }

  //Select the median of arr[lo], arr[hi], and arr[(lo+hi)/2]
  int choosePivot(int * arr, int lo, int hi)
  {
    int mid = (lo+hi)/2;

    int temp;
    if(arr[lo] > arr[hi])
    {
      temp = lo;
      lo = hi;
      hi = temp;
    }
    if(arr[mid] < arr[lo])
    {
      temp = mid;
      mid = lo;
      lo = temp;
    }
    if(arr[hi] < arr[mid])
    {
      temp = mid;
      mid = hi;
      hi = temp;
    }
    return arr[mid];
  }
