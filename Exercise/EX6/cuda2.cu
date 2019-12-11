#include <stdio.h>
#define T 64 // As Threads
#define array_size 256

__global__ void vecMultiplyReverse(int *A, int *B) 
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    int Reverse = (T - 1) - i;
	B[Reverse] = A[i];
    
}
int main (int argc, char *argv[])
{
	int i;
	int size = T*sizeof(int);
    int a[T],b[T], *devA,*devB;
	for (i=0; i< T; i++)
	{
		a[i] = i + 1; 	
	}
	
	cudaMalloc( (void**)&devA,size);
	cudaMalloc( (void**)&devB,size);
    cudaMemcpy( devA, a, size, cudaMemcpyHostToDevice);
    cudaMemcpy( devB, b, size, cudaMemcpyHostToDevice);
    dim3 dimBlock(T);
    dim3 dimGrid(array_size/T - 1);
    vecMultiplyReverse<<<dimGrid,dimBlock>>>(devA,devB);
	printf("Before\n");
	for (i=0; i< T; i++)
	{
		printf("%d ", a[i]);	
	}	
	printf("\n");

    cudaMemcpy(a, devA, size, cudaMemcpyDeviceToHost);
    cudaMemcpy(b, devB, size, cudaMemcpyDeviceToHost);
    cudaFree(devA);
    cudaFree(devB);
	printf("After\n");
    for (i=0; i < T; i++) 
    {
		printf("%d ",b[i]);
	}
	printf("\n");
return 0;
}


