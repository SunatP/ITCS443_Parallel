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
  printf(" Malloc Ok\n");
  cudaMemcpy(arr,Data,size, cudaMemcpyHostToDevice);
  printf(" Copy Ok\n");
  rank_sort<<<1,thread>>>(arr,data); // ส่งไปทีละค่า และค่า thread ทั้งหมด
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

ผลลัพธ์ที่ได้

![1](https://raw.githubusercontent.com/SunatP/ITCS443_Parallel/master/Exercise/EX7/img/1.PNG)


### ข้อ 2. Write a CUDA program. 

  - Create an array of random integers in CPU 
  - Receive an integer to search from a user
  - Use CUDA to count the number of occurrences of the integer in the array (Use atomic operation. Note that this exercise ignores synchronization overhead) 
  - Display the number of occurrences  of the input integer found in the array 
  
1.  อธิบายข้อแรกเราจะต้องสุ่มค่าให้กับอาเรย์จากฝั่ง host(CPU)
2.  รับค่าที่ต้องการจะหาได้จากผู้ที่เขียนโค้ด/ผู้รันโค้ด
3.  ใช้ CUDA ในการนับเลขซ้ำในอาเรย์โดยใช้ Atomic operation (การลดรูปฟังก์ชั่น หรือการใช้ลูป โดยใช้เมธอด)
4.  แสดงผลลัพธ์ตัวเลขที่มีค่าซ้ำออกมาจากอาเรย์

```C++
#include <stdio.h>
#include <stdlib.h>
#define num_thread 64
#define thread 16
__global__ void count(int *data,int input, int *result)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if(data[i] == input)
    {
        int a = 1;
        atomicAdd(result,a); // atomicAdd หรือ การเพิ่มค่า โดยเพิ่มค่าจาก a ไปที่ result
        // result += a;
    }
}

int main(int argc, char *argv[]){
  int Data[num_thread], *arr,input,*result;
  int i;
  int resultarr[1];
  int size = sizeof(int)*num_thread;
  srand(123);
  cudaSetDevice(0);
  for(i = 0; i < num_thread; i++)
  {
     Data[i] = rand() % 50;
     printf("%d ",Data[i]);
  }
  printf("Input value to find: ");
  scanf("%d",&input);
  printf("\n");
  cudaMalloc( (void**) &arr, size);
  cudaMalloc( (void**) &result, sizeof(int));

  cudaMemcpy(arr,Data,size, cudaMemcpyHostToDevice);
  
  count<<<num_thread/thread,thread>>>(arr,input,result);

  cudaMemcpy(resultarr,result,sizeof(int),cudaMemcpyDeviceToHost);
  
  cudaFree(result);  
  cudaFree(arr);

  printf(" Value %d to search occurrences Data found: %d",input,resultarr[0]);

  printf("\n");

  return 0;
}
```

มาดูตรง Kernel
```C++
__global__ void count(int *data,int input, int *result)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if(data[i] == input)
    {
        int a = 1;
        atomicAdd(result,a); // atomicAdd หรือ การเพิ่มค่า โดยเพิ่มค่าจาก a ไปที่ result
        // result += a;
    }
}
```

ตรง **Kernel** นี้จะสังเกตว่าเราใช้ **atomicAdd** (Atomic Operation) ในการหาผลลัพธ์ทั้งหมดจากการบวกค่าทั้งหมด 

แล้วอะไรคือ Atomic Operation กันล่ะ?<br> Atomic Operation คือการทำงานด้วยเธรดเดียว (Single Thread) และใช้หน่วยความจำเพียงนิดเดียวเท่านั้น

Atomic Operation มีกี่แบบ?** มี 5 แบบ**
  1.  Addition/subtraction: atomicAdd, atomicSub
  2.  Minimum/maximum:  atomicMin, atomicMax
  3.  Conditional increment/decrement: atomicInc, atomicDec
  4.  Exchange/compare-and-swap:  atomicExch, atomicCAS
  5.  More types in Fermi:  atomicAnd, atomicOr, atomicXor

Atomic Operation นั้นเป็นการทำงานแบบที่ต้องใช้**แค่เธรดเดียวเท่านั้น **เธรดอื่นไม่สามารถเข้ามาแก้ไขหรือดูค่าได้** ซึ่ง Atomic Operation นั้นสามารถกัน **Race Condition** หรือที่เรียกว่า**การดึงค่าไปใช้ซ้ำกัน**หรือเรียกการเรียก**พารามิเตอร์ซ้ำซ้อน**<br> **Atomic Operation** สามารถ **อ่าน/แก้ไข** และ **เขียนค่า**ลงไปในหน่วยความจำได้เลยโดยไม่มีการรบกวนจากสิ่งต่างๆที่เกิดขึ้น เช่น **การเรียกใช้ค่านั้นๆ**<br> **Atomic Operation** นั้นในหน่วยความจำแบบใช้ร่วมกัน(**Shared memory**)<br> ส่วนการแบ่งหน่วยความจำให้กับทุกฟังก์ชั่น(**Global memory**) นั้นจะใช้ป้องกันการใช้พารามิเตอร์ระหว่างสองเธรดที่ต่างกัน<br> หลังจากที่ **Kernel call** เรียบร้อยแล้ว ผลลัพธ์จะเท่ากับค่า **input**

ผลลัพธ์ที่ได้
![2](https://raw.githubusercontent.com/SunatP/ITCS443_Parallel/master/Exercise/EX7/img/2.PNG)


### ข้อ 3. Given a matrix A[16][16], write a CUDA program to find the summation of each row and each column using atomic operation. 

เราจะต้องสร้างเมทริกซ์ที่มีขนาด **16 * 16** **(A[16][16])** โดยใช้ **CUDA** หาผลลัพธ์ของแต่ละแถวและแต่ละหลักโดยใช้ **Atomic Operation**

```C++
#include <stdio.h>

#define n 16

__global__ void countNumberInArray(int *originalData, int *arrayCount)
{    
    int index = threadIdx.x, i;
    int sum = 0;
    if(threadIdx.x < n)
    {
        for(i = 0; i < n; i++)
        {
            sum += originalData[(index * n) + i];
            printf("%3d " ,threadIdx.x);
        }
    }
    else
    {
        for(i = 0; i < n; i++)
        {
            sum += originalData[(i * n) + index];
            printf("%3d " ,threadIdx.x);
        }   
    }
    atomicAdd(&arrayCount[index],sum);        
}

int main(int argc, char *argv[])
{
    int totalCount = 2 * n;
    int originalData[n][n], count[totalCount];
    int i = 0;
    int j = 0;

    int *deviceOriginalData, *deviceArrayCount;

    int arrayByteSize = (n * n) * sizeof(int);
    int countArrayByteSize = totalCount * sizeof(int);

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
        else
        {
            colCounts[colArrayIterator++] = count[l];
            colsum += count[l];
        }
    }
    printf("TOTAL COUNT ROW\n");
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
```

มาดูตรง Kernel ก่อน**สำคัญมาก**

```C++
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
```
การที่เราใช้ **if-else condition** นั้นเพื่อทำการแบ่งให้เธรด(thread) ทำงานคนละครึ่งซึ่งจะแบ่งแบบนี้

![3.1](https://raw.githubusercontent.com/SunatP/ITCS443_Parallel/master/Exercise/EX7/img/3.1.PNG)<br> โดยตัวเลข 0 - 15 (รวมทั้งหมด 16 threads) นั้นคือการทำส่วน แถว (Row) ก่อน ส่วน 16 - 31 (รวมทั้งหมด 16 threads) จะทำส่วนหลัก (Column) ซึ่งค่าแต่ละหลักรวมกันจะเป็นแบบนี้

```bash
Row[1] = 0 บวกกัน 16 ครั้ง จะได้ 0
Row[2] = 1 บวกกัน 16 ครั้ง จะได้ 16
  .
  .
  .
Row[16] = 15 บวกกัน 16 ครั้ง จะได้ 240
--------------------
Column[1] = บวกตั้งแต่ 0 ไปจนถึง 15 จะได้ 120 # จะได้ 120 ทุกแถว (0+1+2+...+16)
column[2] = บวกตั้งแต่ 0 ไปจนถึง 15 จะได้ 120 # จะได้ 120 ทุกแถว (0+1+2+...+16)
  .
  .
  .
column[16] = บวกตั้งแต่ 0 ไปจนถึง 15 จะได้ 120 # จะได้ 120 ทุกแถว (0+1+2+...+16) 
```

ผลลัพธ์ที่ได้
![3](https://raw.githubusercontent.com/SunatP/ITCS443_Parallel/master/Exercise/EX7/img/3.PNG)

### ข้อ 4. Write a CUDA program to find min, max and average values from an array of student scores using reduction operation. Assume that the number of students is a power of 2. 

ข้อนี้เราก็ต้องใช้ CUDA ในการหาค่าที่น้อยสุด ค่าที่มากสุด และค่าเฉลี่ย จากอาเรย์คะแนนของนักเรียนโดยใช้ **reduction operation** (การมัดรวม/หรือยุบข้อมูลโดยใช้ฟังก์ชั่นเดียวเท่านั้น) โดยนักเรียนมีค่ายกกำลัง 2 

```C++
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
```

ดูที่ **Kernel** ตัวแรก 

```C++
__global__ void reduceToSummation(int *originalData, int stride)
{
    int threadId = (blockIdx.x * blockDim.x) + threadIdx.x;
    int idx = 2 * stride * threadId;
    if(idx < n)
    {
        originalData[idx] = originalData[idx] + originalData[idx + stride];
    }
}
```

**kernel** ตัวแรกเราจะให้ **Host** (**CPU**) นั้นสร้างค่าขึ้นมาแล้วส่งต่อให้ **Device** (**GPU**) ซึ่งจะเรียก **Kernel** **reduceToSummation** ไปหาค่ารวมทั้งหมดก่อนที่จะส่งกลับมาที่ Host เพื่อหาค่าเฉลี่ย

**Kernel** ตัวที่สองก็เหมือนตัวแรกแต่จะทำการหาค่าน้อยที่สุดในอาเรย์ออกมาให้

```C++
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
```

**Kernel** ตัวที่ 3 เหมือนกับตัวที่ 1 และ 2 แต่ตัวที่ 3 จะเป็นการหาค่าที่มากที่สุดในอาเรย์

```C++
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
```

ซึ่ง **Kernel** ทั้ง 3 ตัวนี้คือ **Reduction Operation** หรือที่เรียกว่าการมัดรวมหรือยุบข้อมูลให้เอามาคิดในฟังก์ชั่นเดียว

ผลลัพธ์ที่ได้จะเป็นแบบนี้ 

![4](https://raw.githubusercontent.com/SunatP/ITCS443_Parallel/master/Exercise/EX7/img/4.PNG)