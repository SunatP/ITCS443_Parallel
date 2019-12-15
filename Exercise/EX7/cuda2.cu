#include <stdio.h>
#include <stdlib.h>
#include <thrust/sort.h>
#define num_thread 64
#define thread 16
__global__ void count(int *data,int input, int *result)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if(data[i] == input)
    {
        int a = 1;
        atomicAdd(result,a);
    }
}   

int main(int argc, char *argv[]){
  int Data[num_thread], *arr,input,*result;
  int i;
  int resultarr[1];
  int size = sizeof(int)*num_thread;
  srand(123456846);
  printf(" Generate Ok\n");
  cudaSetDevice(0);
  for(i = 0; i < num_thread; i++)
  {
     Data[i] = rand() % 50;
     printf("%d ",Data[i]);
  }
  printf(" \n Working Ok\n");
  printf("Input value to find: ");
  scanf("%d",&input);
  cudaMalloc( (void**) &arr, size);
  cudaMalloc( (void**) &result, sizeof(int));
  printf(" Malloc Ok\n");

  cudaMemcpy(arr,Data,size, cudaMemcpyHostToDevice);
  
  printf(" Copy Ok\n");
  
  count<<<num_thread/thread,thread>>>(arr,input,result);
  
  printf(" Function Ok\n");
  
  cudaMemcpy(resultarr,result,sizeof(int),cudaMemcpyDeviceToHost);
  printf(" Copy Back Ok\n");
  
  cudaFree(result);  
  cudaFree(arr);
  printf(" Value %d to search occurrences Data found: %d",input,resultarr[0]);

  printf("\n");

  return 0;
}
