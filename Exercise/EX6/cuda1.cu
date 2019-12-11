#include <stdio.h>
#define T 16 // As Threads
#define array_size 64

__global__ void vecMultiply(int *A) 
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	A[i] = A[i] * 2;
}

int main (int argc, char *argv[])
{
	int i;
	int size = T*sizeof(int);
    int a[array_size], *devA;
	for (i=0; i< array_size; i++)
	{
		a[i] = i + 1; 	
	}
	cudaMalloc( (void**)&devA,size);

	cudaMemcpy( devA, a, size, cudaMemcpyHostToDevice);
    dim3 dimBlock(T,T);
    dim3 dimGrid(array_size/T - 1);
    vecMultiply<<<dimGrid,dimBlock>>>(devA);
	printf("Before\n");
	for (i=0; i< array_size; i++)
	{
		printf("%d ", a[i]);	
	}	
	printf("\n");

	cudaMemcpy(a, devA, size, cudaMemcpyDeviceToHost);
	cudaFree(devA);
	printf("After\n");
    for (i=0; i < array_size; i++)
    {
		printf("%d ",a[i]);
	}
	printf("\n");

    return 0;
}


