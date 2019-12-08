#include <stdio.h>
#include <stdlib.h>
#include <thrust/sort.h>
#define array_size 64
#define thread 16
__global__ void rank_sort(int *data, int *result)
{
    int i,j,position;
    i = blockIdx.x * blockDim.x + threadIdx.x;
    int self = data[i];
    for(;i < array_size; i++)
    {
        position = 0;
    for(j = 0; j < array_size; j++)
        if(( self > data[j]) || (self == data[j]) && (j < i))
            position+=1;
    result[position] = self;
    }
}   

int main(int argc, char *argv[]){
  int *arr = (int *) malloc(sizeof(int)*array_size);
  int i;
  int *data = (int *) malloc(sizeof(int)*array_size);
  int *result = (int *) malloc(sizeof(int)*array_size);
  int size =  sizeof(int)*thread;
//   (float *)malloc(blocks*threads_per_block*sizeof(float));
  srand(123);
  for(i = 0; i < array_size; i++)
  {
     arr[i] = rand() % 50;
  }
  printf(" Working Ok\n");
  cudaMalloc( (void**) &data, thread*array_size);
  cudaMalloc( (void**) &result, thread*array_size);
  printf(" Mallock Ok\n");
  thrust::sort(arr, arr + size);
  cudaMemcpy(data,arr,size, cudaMemcpyHostToDevice);
  printf(" Copy Ok\n");
  dim3 dimBlock(thread);
  dim3 dimGrid(array_size/thread - 1);
  rank_sort<<<dimGrid,dimBlock>>>(data,result);
  printf(" Function Ok\n");
  cudaMemcpy(arr,data,size,cudaMemcpyDeviceToHost);
  printf(" Copy Back Ok\n");

  printf(" Sorted Data \n");
  for(i = 0 ; i < array_size ; i++)
  {
    printf("%d ",arr[i]);
  }
  printf("\n");
  printf(" Sorted OK \n");
  cudaFree(data);  
  cudaFree(result);
  return 0;
}
