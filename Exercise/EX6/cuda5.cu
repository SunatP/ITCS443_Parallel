#include <stdio.h>
#define T 8 // As Threads
#define N 16


__global__ void vecMatrix(int *A, int *B) {
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y+ threadIdx.y;
    int width = gridDim.x * T;
    for( int j = 0; j<T; j+=N )
    {
        B[x*width + (j+y)] = A[(y+j)*width + x];
    }

}

int main (int argc, char *argv[]){
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
    int newline = 0;

	for (i=0; i < N * N; i++) {
        newline++;
        printf("%3d ",size[i]);
        if(newline == N)
        {
            newline = 0;
            printf("\n");
        }	
	}
	printf("\n");

}


