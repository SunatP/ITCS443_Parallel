#include <stdio.h>

// CUDA to assign a value to each element of the array of integers A[256] using 256 threads. 
// Each A[i] is assigned with the value of 2*i, for  i = 0 to 255.
#define T 256 // As Threads
#define ArraySize 1314

__global__ void reverseArray(int *A, int *B) {
	int threadID = threadIdx.x;
	int start = (threadID * ArraySize) / 256;
	int end = ( ( (threadID + 1 ) * ArraySize) / 256) - 1;
	while(end > 0)
	{
		B[end] = A[start];
		end--;
		start++;
	}
}

int main (int argc, char *argv[]){
	int i;
	int size = ArraySize*sizeof(int);
	int a[ArraySize],b[ArraySize], *devA,*devB;
	for (i=0; i< ArraySize; i++)
	{
		a[i] = i + 1; 	
	}
	
	cudaMalloc( (void**)&devA,size);
	cudaMalloc( (void**)&devB,size);
	cudaMemcpy( devA, a, size, cudaMemcpyHostToDevice);
	cudaMemcpy( devB, b, size, cudaMemcpyHostToDevice);

	
	reverseArray<<<1, 256>>>(devA,devB); // 1 , 256 mean send each data with total thread 256 threads
	printf("Before\n");
	for (i=0; i< ArraySize; i++)
	{
		printf("%d ", a[i]);	
	}	
	printf("\n");

	cudaMemcpy(a, devA, size, cudaMemcpyDeviceToHost);
	cudaMemcpy(b, devB, size, cudaMemcpyDeviceToHost);
	cudaFree(devA);
	cudaFree(devB);
	printf("After\n");
	for (i=0; i < ArraySize; i++) {
		printf("%d ",b[i]);
	}
	printf("\n");

}


