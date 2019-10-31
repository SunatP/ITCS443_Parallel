#include <stdio.h>
#include <stdlib.h>
#include "math.h"
#include <time.h>

/*Displays the array, passed to this method*/
static int N = 10000000;
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
int partition(int arr[], int lower, int upper)
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
void quickSort(int arr[], int lower, int upper)
{

    if (upper > lower)
    {

        // partitioning index is returned by the partition method , partition element is at its correct poition

        int partitionIndex = partition(arr, lower, upper);

        // Sorting elements before and after the partition index
        quickSort(arr, lower, partitionIndex - 1);
        quickSort(arr, partitionIndex + 1, upper);
    }
}

int main()
{
    clock_t begin, end;
    double time1 = 0.0, time2 = 0.0;
    int     i;
    double  t1,t2,t3;

    printf("Enter the elements of the array\n");
    int *arr = (int *)malloc(sizeof(int) * N);
    srand(1432427398);
    for (int i = 0; i < N; i++)
    {
        arr[i] = rand() % 10000;
    }

    printf("Original array: ");
    display(arr, N); // Original array : 10 11 9 8 4 7 3 8
    begin = clock();
    quickSort(arr, 0, N - 1);
    end = clock();
    printf("Sorted array: ");
    display(arr, N); // Sorted array : 3 4 7 8 8 9 10 11
    time1 = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Average time taken by Seqential Quicksort: %f seconds\n",time1);
    getchar();
    return 0;
}