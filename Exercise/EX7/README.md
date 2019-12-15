# Exercise 7 Advanced Topics in CUDA 

1.  From the following sequential Rank sort algorithm that allows duplicate numbers, implement a parallel version in CUDA. เราจะต้องอิงอัลกอริทึ่มตาม Ranksort โดยที่สามารถ sort เลขที่ซ้ำกันได้

```C++
for (i = 0; i < n; i++) /* for each number */ 
{ 
    x = 0; for (j = 0; j < n; j++) /* count number less than it */ 
    if ((a[i] > a[j]) || (a[i] == a[j]) && (j <i))
    x++; 
    b[x] = a[i];  /* copy number into correct place */ 
}
```

เราจะ implement ตัว rank sort นี้เข้ามาใช้ในโค้ดของเรา

```C++
#include <stdio.h>
#include <stdlib.h>
#include <thrust/sort.h>
#define thread 512
__global__ void rank_sort(int *data, int *result)
{
    int i,j,position;
    position = 0;
    i = threadIdx.x;
    int self = data[i];

    for(j = 0; j < thread; j++)
    {
        if(( self > data[j]) || (self == data[j]) && (j < i))
        {
          position+=1;
        }      
    }
    result[position] = self;
}   

int main(int argc, char *argv[]){
  int *arr,*data;
  int i;
  int Data[thread],sort[thread];
  int size =  sizeof(int)*thread;
//   (float *)malloc(blocks*threads_per_block*sizeof(float));
  srand(123);
  printf(" Generate Ok\n");
  for(i = 0; i < thread; i++)
  {
     Data[i] = rand() % 100;
     printf("%d ",Data[i]);
  }
  printf(" \n Working Ok\n");
  cudaMalloc( (void**) &arr, size);
  cudaMalloc( (void**) &data, size);
  printf(" Mallock Ok\n");
  // thrust::sort(arr, arr + size);
  cudaMemcpy(arr,Data,size, cudaMemcpyHostToDevice);
  printf(" Copy Ok\n");
  rank_sort<<<1,thread>>>(arr,data);
  printf(" Function Ok\n");
  cudaMemcpy(sort,data,size,cudaMemcpyDeviceToHost);
  printf(" Copy Back Ok\n");

  printf(" Sorted Data \n");
  for(i = 0 ; i < thread ; i++)
  {
    printf("%d ",sort[i]);
  }
  printf("\n");
  printf(" Sorted OK \n");
  cudaFree(data);  
  cudaFree(arr);
  return 0;
}
```

### ข้อ 2. Write a CUDA program. 

  - Create an array of random integers in CPU 
  - Receive an integer to search from a user
  - Use CUDA to count the number of occurrences of the integer in the array (Use atomic operation. Note that this exercise ignores synchronization overhead) 
  - Display the number of occurrences  of the input integer found in the array 

### ข้อ 3. Given a matrix A[16][16], write a CUDA program to find the summation of each row and each column using atomic operation. 



### ข้อ 4. Write a CUDA program to find min, max and average values from an array of student scores using reduction operation. Assume that the number of students is a power of 2. 
