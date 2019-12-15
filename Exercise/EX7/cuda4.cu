#include <stdio.h>
#include <stdlib.h>

#define T 256
#define n 1024

__global__ void reduceToSummation(int *originalData, int stride)
{
    int threadId = (blockIdx.x * blockDim.x) + threadIdx.x;
    int idx = 2 * stride * threadId;
    if(idx < n)
    {
        originalData[idx] = originalData[idx] + originalData[idx + stride];
    }
}

__global__ void reduceToMinimum(int *originalData, int stride)
{
    int threadId = (blockIdx.x * blockDim.x) + threadIdx.x;
    int idx = 2 * stride * threadId;
    if(idx < n)
    {
        int min = originalData[idx];
        if(originalData[idx + stride] < min)
        {
            min = originalData[idx + stride];
        }
        originalData[idx] = min;
    }
}

__global__ void reduceToMaximum(int *originalData, int stride)
{
    int threadId = (blockIdx.x * blockDim.x) + threadIdx.x;
    int idx = 2 * stride * threadId;
    if(idx < n)
    {
        int max = originalData[idx];
        if(originalData[idx + stride] > max)
        {
            max = originalData[idx + stride];
        }
        originalData[idx] = max;
    }
}

int main(int argc, char *argv[])
{
    int originalData[n];
    int sum, min, max;
    int i;
    int *deviceOriginalData;
    int arrayByteSize = n * sizeof(int);
    printf("ORIGINAL: \n");
    for(i = 0; i < n; i++)
    {
        originalData[i] = i;
        printf("%3d ", originalData[i]);
    }
    printf("\n\n"); 
    // Allocates Once for all kernels
    cudaMalloc((void**) &deviceOriginalData, arrayByteSize);
   
    // KERNEL 1: Find Average by Finding Summation
    cudaMemcpy(deviceOriginalData, originalData, arrayByteSize, cudaMemcpyHostToDevice);
    for(int s = 1; s < n; s *= 2)
    {
        reduceToSummation<<<(n + T - 1) / T, T>>>(deviceOriginalData, s);
    }
    cudaMemcpy(&sum, deviceOriginalData, sizeof(int), cudaMemcpyDeviceToHost);
    double realAverage = sum / (double) n;
    
    // KERNEL 2: Find Minimum
    cudaMemcpy(deviceOriginalData, originalData, arrayByteSize, cudaMemcpyHostToDevice);
    for(int s = 1; s < n; s *= 2)
    {
        reduceToMinimum<<<(n + T - 1) / T, T>>>(deviceOriginalData, s);
    }
    cudaMemcpy(&min, deviceOriginalData, sizeof(int), cudaMemcpyDeviceToHost);

    // KERNEL 3: Find Maximum
    cudaMemcpy(deviceOriginalData, originalData, arrayByteSize, cudaMemcpyHostToDevice);
    for(int s = 1; s < n; s *= 2)
    {
        reduceToMaximum<<<(n + T - 1) / T, T>>>(deviceOriginalData, s);
    }
    cudaMemcpy(&max, deviceOriginalData, sizeof(int), cudaMemcpyDeviceToHost);

    // Free the memory
    cudaFree(deviceOriginalData);

    // Print the results
    printf("\nAverage is %.2f", realAverage);
    printf("\nThe Minimum Number is %d\n", min);
    printf("The Maximum Number is %d\n", max);
    return 0;
}