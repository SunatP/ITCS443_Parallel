#include <stdio.h>
#define T 256
#define n 1024
// #define n 1029 

__global__ void vecAdd(float *A, float *B, float *C) {
    int i;
    
    i = blockIdx.x*blockDim.x + threadIdx.x;
    C[i] = A[i] + B[i];
    
}

int main (int argc, char *argv[]){
    int i; 
    int size = n * sizeof(float);
    float a[n] , b[n], c[n], *devA , *devB,* devC;
    for(i=0 ; i <n ; i++)
    {
        a[i] = 1 ; b[i] = 2;

    }
    cudaMalloc( (void**)&devA,size);
	cudaMalloc( (void**)&devB,size);
	cudaMalloc( (void**)&devC,size);

	cudaMemcpy( devA, a, size, cudaMemcpyHostToDevice);
	cudaMemcpy( devB, b, size, cudaMemcpyHostToDevice);

    int nblocks = n/T;
    // int nblocks = (n+T - 1) / T; // Efficient way to run

	vecAdd<<<nblocks, T>>>(devA, devB, devC);

	cudaMemcpy(c, devC, size, cudaMemcpyDeviceToHost);
	cudaFree(devA);
	cudaFree(devB);
	cudaFree(devC);

    for(i = 0 ; i< n ; i++)
    {
        printf("%f",c[i]);

    }
    printf("\n");
}



