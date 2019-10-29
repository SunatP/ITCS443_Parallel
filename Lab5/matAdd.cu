#include <stdio.h>

#define N 16

__global__ void addMatrix(float *A, float *B, float *C) {
    int i = threadIdx.y;
    int j = threadIdx.x;
    int index = i*N+j;
    C[indexi] = A[index] + B[index];
}

int main(int argc , char *argv[])
{
    int i,j;
    int size = N * N * sizeof(float);
    float = a[N][N],b[N][N], c[N][N], *devA, *devB,*devC;

    for(i=0; i<N;i++)
    {
        for(j=0;j< N ;j++)
        {
            a[i][j] = 1;b[i][j]=2;
        }
    }
    cudaMalloc( (void**)&devA,size);
	cudaMalloc( (void**)&devB,size);
	cudaMalloc( (void**)&devC,size);

	cudaMemcpy( devA, a, size, cudaMemcpyHostToDevice);
	cudaMemcpy( devB, b, size, cudaMemcpyHostToDevice);

    dim3 dimBlock (N,N);
    dim3 dimGrid (1,1);

    // int nblocks = n/T;
    // int nblocks = (n+T - 1) / T; // Efficient way to run

	addMatrix<<<dimGrid, dimBlock>>>(devA, devB, devC);

	cudaMemcpy(c, devC, size, cudaMemcpyDeviceToHost);
	cudaFree(devA);
	cudaFree(devB);
    cudaFree(devC);
    for(i=0; i<N;i++)
    {
        for(j=0;j< N ;j++)
        {
           printf("%.2f " ,c[i][j]);
        }
        printf("\n");
    }   
}