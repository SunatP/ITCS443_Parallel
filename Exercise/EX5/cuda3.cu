#include <stdio.h>

// CUDA to assign a value to each element of the array of integers A[256] using 256 threads. 
// Each A[i] is assigned with the value of 2*i, for  i = 0 to 255.
#define T 256 // As Threads

__global__ void reverseArray(int *A, int *B) {
	int threadID = threadIdx.x;
	int Reverse = (T - 1) - threadID;
	B[Reverse] = A[threadID];
}

int main (int argc, char *argv[]){
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

	
	reverseArray<<<1, T>>>(devA,devB); // 1 , 256 mean send each data with total thread 256 threads
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
	for (i=0; i < T; i++) {
		printf("%d ",b[i]);
	}
	printf("\n");

}


