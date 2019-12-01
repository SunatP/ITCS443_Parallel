#include <stdio.h>
#include <stdlib.h>
#define thread 256
#define arraySize 64
/**
 From the following sequential Rank sort algorithm that allows duplicate numbers, 
 implement a parallel version in CUDA. 
**/

__global__ void RankSort(int *data, int *result)
{
   int i,j,position;
   
   i = threadIdx.x;
   int self = data[i];

   for(j = 0 ; j < arraySize ; j++)
   {
       position = 0;
       if((self > data[i]) || (self == data[j]) && (j < i))
       {
           position++;
       }
       result[position] = self;
   }
}
int main(int argc,char *argv[])
{
    int arr[arraySize];
    int i;
    int size = thread*sizeof(int);
    int *data, *result;
    srand(time(0));
    for(i = 0 ; i < arraySize ; i++)
    {
         arr[i] = rand() % 50;
    }
    cudaMalloc( (void**)&data,size);
    cudaMalloc( (void**)&result,size);
	cudaMemcpy( data, arr, size, cudaMemcpyHostToDevice);

    RankSort<<<thread/arraySize,thread>>>(data,result);

    cudaMemcpy( arr, result,size,cudaMemcpyDeviceToHost);
    cudaFree(data);
    cudaFree(result);

    printf("\n sorted data");
    for(i = 0 ; i< arraySize ;i++)
    {
        printf("%d", data[i]);
    }
    printf("\n");
    return 0;
}