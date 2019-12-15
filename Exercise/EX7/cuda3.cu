#include <stdio.h>
#include <stdlib.h>
#define n 16

__global__ void countNumberInArray(int *originalData, int *arrayCount)
{    
    int index = blockIdx.x * blockDim.x + threadIdx.x, i;
    int sum = 0;
    if(threadIdx.x < n)
    {
    for(i = 0; i < n; i++)
    {
        if(i < n)
        {
            sum += originalData[(index * n) + i];
            // atomicAdd(&arrayCount[index],sum);
        }
        else
        {
            sum += originalData[(index * n) + index];
            // atomicAdd(&arrayCount[index],sum);
        }
        printf("%3d " ,threadIdx.x);
    }
    printf("\n");
    }
    
    else
    {
        for(i = 0; i < n; i++)
        {
            if(i < n)
            {
                sum += originalData[(index * n) + i];
                // atomicAdd(&arrayCount[index],sum);
            }
            else
            {
                sum += originalData[(index * n) + index];
                // atomicAdd(&arrayCount[index],sum);
            }
            printf("%3d " ,threadIdx.x);
        }  

    }
 
    atomicAdd(&arrayCount[index],sum);
}

int main(int argc, char *argv[])
{
    int totalCount = 2 * n;
    int originalData[n][n], count[totalCount];
    int i, j;
    int *deviceOriginalData, *deviceArrayCount;
    int arrayByteSize = (n *n) * sizeof(int);
    int countArrayByteSize = totalCount * sizeof(int);
    printf("\n"); 
    printf("ORIGINAL: \n");
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < n; j++)
        {
            originalData[i][j] = i;
            printf("%3d ", originalData[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");

    cudaMalloc((void**) &deviceOriginalData, arrayByteSize);
    cudaMalloc((void**) &deviceArrayCount, countArrayByteSize);
    cudaMemcpy(deviceOriginalData, originalData, arrayByteSize, cudaMemcpyHostToDevice);
    
    dim3 blockDim(totalCount);
    countNumberInArray<<<1, blockDim>>>(deviceOriginalData, deviceArrayCount);
    
    cudaMemcpy(count, deviceArrayCount, countArrayByteSize, cudaMemcpyDeviceToHost);
    cudaFree(deviceOriginalData);
    cudaFree(deviceArrayCount);

    int rowCounts[n], colCounts[n], rowArrayIterator = 0, colArrayIterator = 0;
    int rowsum = 0;
    int colsum = 0;
    int l = 0;
    for(l = 0; l < totalCount; l++)
    {
        if(l < n)
        {
            rowCounts[rowArrayIterator++] = count[l]; 
            rowsum += count[l];
        } 
    }
    for(l = 0; l < totalCount; l++)
    {
        if(l < n)
        {
            colCounts[colArrayIterator++] = count[l];     
        }
        colsum += count[l];
    }
    printf("\nTOTAL COUNT ROW\n");
    for(l = 0; l < n; l++)
    {
        printf("(%d,%3d)", l, rowCounts[l]);
    }
    printf("\nSum Row: %d\n" ,rowsum);
    printf("\n\nTOTAL COUNT COL\n");
    for(l = 0; l < n; l++)
    {
        printf("(%d,%3d)", l, colCounts[l]);
    }
    printf("\nSum Col: %d\n" ,colsum);
    printf("\n");
    return 0;
}