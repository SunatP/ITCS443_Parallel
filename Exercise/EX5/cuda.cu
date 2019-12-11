#include <stdio.h>

// CUDA to assign a value to each element of the array of integers A[256] using 256 threads. 
// Each A[i] is assigned with the value of 2*i, for  i = 0 to 255.
#define T 256 // As Threads
// #define n 256

__global__ void vecMultiply(int *A) 
{
	int i = threadIdx.x;
	A[i] = A[i] * 2;
}

int main (int argc, char *argv[])
{
	int i;
	int size = T*sizeof(int);
	int a[T], *devA;
	for (i=0; i< T; i++)
	{
		a[i] = i + 1; 	
	}
	
	cudaMalloc( (void**)&devA,size);
	cudaMemcpy( devA, a, size, cudaMemcpyHostToDevice);

	vecMultiply<<<1, T>>>(devA);
	printf("Before\n");
	for (i=0; i< T; i++)
	{
		printf("%d ", a[i]);	
	}	
	printf("\n");

	cudaMemcpy(a, devA, size, cudaMemcpyDeviceToHost);
	cudaFree(devA);
	printf("After\n");
	for (i=0; i < T; i++) {
		printf("%d ",a[i]);
	}
	printf("\n");

}


