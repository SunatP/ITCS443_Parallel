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
#include <thrust/sort.h> // ตรงนี้เราจะใช้ Header ของ Nvidia เข้ามาช่วย sort
#define array_size 64 // ขนาดอาเรย์
#define thread 16 // define ไว้สำหรับ thread
__global__ void rank_sort(int *data, int *result)
{
    int i,j,position;
    i = blockIdx.x * blockDim.x + threadIdx.x;
    int self = data[i];
    for(;i < array_size; i++) // เนื่องจากเราassign i ไว้แล้วเลยไม่ต้อง assign ค่าให้มันอีกรอบ
    {
        position = 0;
    for(j = 0; j < array_size; j++)
        if(( self > data[j]) || (self == data[j]) && (j < i))
            position+=1;
    result[position] = self;
    }
} 

int main(int argc, char *argv[]){
  int *arr = (int *) malloc(sizeof(int)*array_size);
  int i;
  int *data = (int *) malloc(sizeof(int)*array_size);
  int *result = (int *) malloc(sizeof(int)*array_size);
  int size =  sizeof(int)*thread;
  srand(123);
  for(i = 0; i < array_size; i++)
  {
     arr[i] = rand() % 50;
  }
  cudaMalloc( (void**) &data, thread*array_size);
  cudaMalloc( (void**) &result, thread*array_size);
  thrust::sort(arr, arr + size); // ตรงนี้คือ function sort ของ Nvidia สามารถใช้ sort ได้แบบปกติเลย
  cudaMemcpy(data,arr,size, cudaMemcpyHostToDevice);
  dim3 dimBlock(thread);
  dim3 dimGrid(array_size/thread - 1);
  rank_sort<<<dimGrid,dimBlock>>>(data,result);
  cudaMemcpy(arr,data,size,cudaMemcpyDeviceToHost);
  printf(" Sorted Data \n");
  for(i = 0 ; i < array_size ; i++)
  {
    printf("%d ",arr[i]);
  }
  printf("\n");
  cudaFree(data);  
  cudaFree(result);
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
