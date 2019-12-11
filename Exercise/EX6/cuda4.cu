#include <stdio.h>
#define T 8 // As Threads
#define N 16


__global__ void vecMatrix(int *A, int *B) 
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    int j = blockIdx.y * blockDim.y + threadIdx.y;
    int id = (i * N) + j;
    if(i < N && j < N)
    {
        B[id] = A[id] + 1;
    }
}

int main (int argc, char *argv[])
{
	int i,j;
    int size[N*N];
    int A[N][N];

    int sizearr = N*N *sizeof(int);

    int *Adefault,*B;

	for (i=0; i< N; i++)
	{
        for(j = 0 ; j<N ; j++ )
        {
            A[i][j] = ((i*i) +1) * (j+1);
            printf("%5d ", A[i][j]);
        }
    }
    printf("\n");
	cudaMalloc( (void**)&Adefault,sizearr);
    cudaMalloc( (void**)&B,sizearr);
    cudaMemcpy( Adefault, A, sizearr, cudaMemcpyHostToDevice);

    dim3 dimBlock(T,T);
    dim3 dimGrid((N+ dimBlock.x - 1)/ dimBlock.x ,(N + dimBlock.y - 1) / dimBlock.y);
    vecMatrix<<<dimGrid,dimBlock>>>(Adefault,B);
    cudaMemcpy(size, B, sizearr, cudaMemcpyDeviceToHost);
    cudaFree(Adefault);
    cudaFree(B);
	printf("Result\n");
    for (i=0; i < N * N; i++)
    {
		printf("%5d ",size[i]);
	}
	printf("\n");
return 0;
}


