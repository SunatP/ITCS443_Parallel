# Exercise 6 CUDA Threads 

สิ่งที่ต้องใช้ 
    1.  WinSCP 
    2.  Terminal
    3.  Code Editor
    4.  CUDA (nvcc or Nvidia compiler)

### 1. Create a CUDA program to assign a value to each element of the array of integers X[N] using multiple thread blocks.  Each X[i] = 2*i and N can be any positive integer. 

ข้อนี้เราจะต้องเขียนโค้ดด้วย CUDA (C++) โดยให้ค่าแต่ละตัวนั้นจะอยู่ในรูปของ X[N] โดยใช้เธรดแบบหลายตัว ใช้พวก DimGrid , DimBlock นั่นแหละ โดยแต่ละตำแหน่ง ของ X[i] จะเท่ากับ 2*i และ N จะต้องเป็นค่าบวก

```C++
#include <stdio.h>
#define T 16 // As Threads
#define array_size 64

__global__ void vecMultiply(int *A) 
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	A[i] = A[i] * 2;
}

int main (int argc, char *argv[])
{
	int i;
	int size = T*sizeof(int);int a[array_size], *devA;
	for (i=0; i< array_size; i++)
	{
		a[i] = i + 1; 	
	}
	
	cudaMalloc( (void**)&devA,size);
	cudaMemcpy( devA, a, size, cudaMemcpyHostToDevice);
    dim3 dimBlock(T,T);
    dim3 dimGrid(array_size/T - 1);
    vecMultiply<<<dimGrid,dimBlock>>>(devA);
	printf("Before\n");
	for (i=0; i< array_size; i++)
	{
		printf("%d ", a[i]);	
	}	
	printf("\n");

	cudaMemcpy(a, devA, size, cudaMemcpyDeviceToHost);
	cudaFree(devA);
	printf("After\n");
	for (i=0; i < array_size; i++) 
    {
		printf("%d ",a[i]);
	}
	printf("\n");
    return 0;
}
```

ตรงนี้ 

```C++
__global__ void vecMultiply(int *A) 
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	A[i] = A[i] * 2;
}
```
**int i = blockIdx.x * blockDim.x + threadIdx.x;**  ตรงนี้จะเป็นการคิดเลขแบบ multiple thread โดยที่ blockIdx คือ **block Index** จะทำหน้าที่เป็นดัชนีเพื่อเก็บค่าที่ต่างกันในอาเรย์ ส่วน **blockDim** คือ **Block Dimension** ทำหน้าที่เป็นตัวบอกตำแหน่งของเธรดในบล็อกนั้นๆ ส่วน **threadIdx** คือ **Thread Index** ทำหน้าที่เก็บค่าดัชนีสำหรับเธรดในแต่ละบล็อก ส่วนตรงนี้

```C++
dim3 dimBlock(T,T);
dim3 dimGrid(array_size/T - 1);
```

**dim3** คือ integer ในรูปแบบเวกเตอร์ (Vector) มาจาก uint3 (unsigned integer3) ไว้สำหรับระบุมิติของอาเรย์ เมื่อเราใช้ dim3 เมื่อไหร่ก็ตาม ค่าที่ยังไม่ได้โดน assign จะถูก assign เป็น 1 เสมอ 