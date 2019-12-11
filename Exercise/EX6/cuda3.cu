#include <stdio.h>
#define T 16 // As Threads
#define array_size 64

__global__ void vecMultiplyReverse(int *A, int *B, int *C) 
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if(i%2 == 0)
    {
        C[i] = A[i] + B[i];
    }
    else if(i%2 != 0)
    {
        C[i] = A[i] - B[i];
    }
}

int main (int argc, char *argv[])
{
	int i;
	int size = T*sizeof(int);
    int a[T],b[T],c[T], *devA,*devB,*devC;
	for (i=0; i< T; i++)
	{
        a[i] = i + 2;
        b[i] = i + 1; 
	}
	
	cudaMalloc( (void**)&devA,size);
    cudaMalloc( (void**)&devB,size);
    cudaMalloc( (void**)&devC,size);
    cudaMemcpy( devA, a, size, cudaMemcpyHostToDevice);
    cudaMemcpy( devB, b, size, cudaMemcpyHostToDevice);
    cudaMemcpy( devC, c, size, cudaMemcpyHostToDevice);
    dim3 dimBlock(T);
    dim3 dimGrid(array_size/T - 1);
    vecMultiplyReverse<<<dimGrid,dimBlock>>>(devA,devB,devC);
	printf("Before A: \n");
	for (i=0; i< T; i++)
	{
		printf("%d ", a[i]);	
	}	
	printf("\n");

    printf("Before B: \n");
	for (i=0; i< T; i++)
	{
		printf("%d ", b[i]);	
	}	
	printf("\n");

    cudaMemcpy(a, devA, size, cudaMemcpyDeviceToHost);
    cudaMemcpy(b, devB, size, cudaMemcpyDeviceToHost);
    cudaMemcpy(c, devC, size, cudaMemcpyDeviceToHost);
    cudaFree(devA);
    cudaFree(devB);
    cudaFree(devC);
	printf("After\n");
    for (i=0; i < T; i++) 
    {
		printf("%d ",c[i]);
	}
	printf("\n");
return 0;
}


