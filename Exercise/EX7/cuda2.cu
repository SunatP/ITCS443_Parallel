#include <stdio.h>
#include <stdlib.h>
#include <thrust/sort.h>
#define thread 512
__global__ void rank_sort(int *data, int *result)
{
    int i,j,position;
    position = 0;
    i = threadIdx.x;
    int self = data[i];

    for(j = 0; j < thread; j++)
    {
        if(( self > data[j]) || (self == data[j]) && (j < i))
        {
          position+=1;
        }      
    }
    result[position] = self;
}   

int main(int argc, char *argv[]){
  int *arr,*data;
  int i;
  int Data[thread],sort[thread];
  int size =  sizeof(int)*thread;
//   (float *)malloc(blocks*threads_per_block*sizeof(float));
  srand(123);
  printf(" Generate Ok\n");
  for(i = 0; i < thread; i++)
  {
     Data[i] = rand() % 100;
     printf("%d ",Data[i]);
  }
  printf(" \n Working Ok\n");
  cudaMalloc( (void**) &arr, size);
  cudaMalloc( (void**) &data, size);
  printf(" Mallock Ok\n");
  // thrust::sort(arr, arr + size);
  cudaMemcpy(arr,Data,size, cudaMemcpyHostToDevice);
  printf(" Copy Ok\n");
  rank_sort<<<1,thread>>>(arr,data);
  printf(" Function Ok\n");
  cudaMemcpy(sort,data,size,cudaMemcpyDeviceToHost);
  printf(" Copy Back Ok\n");

  printf(" Sorted Data \n");
  for(i = 0 ; i < thread ; i++)
  {
    printf("%d ",sort[i]);
  }
  printf("\n");
  printf(" Sorted OK \n");
  cudaFree(data);  
  cudaFree(arr);
  return 0;
}
