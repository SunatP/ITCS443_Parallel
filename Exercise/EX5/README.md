# Excercise 5 Introduction to CUDA 

สิ่งที่ต้องใช้ 
    1.  WinSCP 
    2.  Terminal
    3.  Code Editor
    4.  CUDA (nvcc or Nvidia compiler)

### ข้อ 1.

1. Use CUDA to assign a value to each element of the array of integers A[256] using 256 threads. Each A[i] is assigned with the value of 2*i, for  i = 0 to 255. 

โจทย์ข้อนี้ให้เราใช้ CUDA เพื่อกำหนดค่าในอาเรย์ตำแหน่งโดยใช้ อาเรย์ขนาด 256 และใช้เธรดขนาด 256 เธรด โดยอาเรย์ A ที่มีขนาด 256 นั้นในแต่ละตำแหน่งจะถูกกำหนดค่าโดย i * 2 โดยที่ i เริ่มตั้งแต่ 0 จนถึง 255

```C++
#include <stdio.h> 
#define T 256 // As Threads

__global__ void vecMultiply(int *A) {
	int i = threadIdx.x;
	A[i] = A[i] * 2; // ตำแหน่งจะถูกกำหนดค่าโดย i * 2 โดยที่ i เริ่มตั้งแต่ 0 จนถึง 255
}

int main (int argc, char *argv[])
{
	int i;
	int size = T*sizeof(int);
	int a[T], *devA;
	for (i=0; i< T; i++)
	{
		a[i] = i + 1;
	}
	cudaMalloc( (void**)&devA,size);
	cudaMemcpy( devA, a, size, cudaMemcpyHostToDevice);
	
	vecMultiply<<<1, T>>>(devA); // ตรงนี้เรียกว่า Kernel launch 
	printf("Before\n");
	for (i=0; i< T; i++)
	{
		printf("%d ", a[i]);
	}	
	printf("\n");

	cudaMemcpy(a, devA, size, cudaMemcpyDeviceToHost);
	cudaFree(devA);
	printf("After\n");
	for (i=0; i < T; i++) 
    {
		printf("%d ",a[i]);
	}
	printf("\n");

}
```
ผลลัพธ์ที่ออกมาจะประมาณนี้


vecMultiply<<<1, T>>>(devA); ตรงนี้ใน **Nvidia CUDA** จะเรียกว่า **kernel Launch**  ซึ่งค่า **1** ตรงนี้หมายถึง จำนวนกริดของเธรดบล็อก (Grid number of Thread Blocks) และ ค่า **T** หมายถึง เธรดบล็อกที่มีขนาดเป็น T ตัวในการทำงานแบบคู่ขนาน (thread block has T parallel threads)ถ้าเราจะมองให้เห็นภาพกว่านี้อีกนิ๊ดด

```C++
mykernel<<<blocks, threads, shared_mem, stream>>>(args);
vecMultiply<<<1, T>>>(devA); 
```
คือตรง mykernel อ่ะมันเป็นคำสั่งหรือ **kernel launch** เพื่อส่งข้อมูลไปหา Device (CUDA) ประมวลผลลัพธ์มาให้โดยเทียบกับ **vecMultiply** มันก็คือ Kernel launch เช่นกันโดยค่า **1** คือ **blocks**  และ **T** คือ **Threads Blocks** นั่นเอง 

ตัวอย่าง
```C++
__global__ void vecMultiply(int *A) { // ตรงนี้เรียกว่า Kernel 
	int i = threadIdx.x;
	A[i] = A[i] * 2; // ตำแหน่งจะถูกกำหนดค่าโดย i * 2 โดยที่ i เริ่มตั้งแต่ 0 จนถึง 255
}
vecMultiply<<<1, T>>>(devA); // ตรงนี้เรียกว่า Kernel launch
```
Function ที่ CUDA สามารถติดต่อกับ Devices ได้

```c++
__host__ // เรียกใช้ได้เฉพาะบน Host และรันได้แค่ Host เท่านั้น
__global__ // เรียกใช้จาก Host ไปรันบน Device (CUDA)
__device__ // เรียกใช้จาก Device ไปรันบน Device เท่านั้น (CUDA)
```

### วิธีส่งโค้ดไปคำนวณบน CUDA Device 

วิธีมันจะพิสดารกว่าปกติหน่อยเพราะมี **Memory** ที่แยกกันออกมาเลยต้องกำหนดขนาด Memory แล้วค่อยโยนโค้ดไปคิดแล้วโยนผลลัพธ์กลับมาหาเครื่อง วิธีก็ตามนี้
```bash
1.  Allocate Memory บน Device # cudaMalloc( (void**)&devA,size);
2.  Transfer ข้อมูลจาก Host ไปยัง Device # cudaMemcpy( devA, a, size, cudaMemcpyHostToDevice);
3.  Kernel Launch  # vecMultiply<<<1, T>>>(devA);
4.  Transfer ข้อมูลกลับจาก Device ไปยัง Host # cudaMemcpy(a, devA, size, cudaMemcpyDeviceToHost);
5.  Free Memory บน Device # cudaFree(devA);
```

### วิธีคอมไพล์ไฟล์ CUDA (.cu)

ใช้คำสั่งตามนี้
```bash
nvcc hello.cu -o hello # เพื่อคอมไพล์
nvcc -o hello hello.cu # หรือแบบนี้ก็ย่อมได้
```

### วิธีรัน CUDA
วิธีรันคล้ายๆกับการรันภาษาซีบน Unix เลยแค่
```bash
./ชื่อไฟล์ที่จะรัน
```

### ข้อ 2.

Repeat Question 1 with the array A[1314], using only 256 threads.  
ก็คือเอาโค้ดจากข้อ 1 มาแก้เพิ่มโดยกำหนดขนาดอาเรย์ A ให้มีขนาดเป็น 1314 และใช้เธรดแค่ 256 ตัวเท่านั้น

```C++
#include <stdio.h>

#define T 256 // As Threads
#define ArraySize 1314

__global__ void vecMultiply(int *A) 
{
	int i;
	int threadID = threadIdx.x;
	int start = (threadID * ArraySize) / 256;
	int end = ( ( (threadID + 1 ) * ArraySize) / 256) - 1;
	for(i = start ; i < end ; i++)
	{
		A[i] = A[i] * 2;
	}
}
int main (int argc, char *argv[])
{
	int i;
	int size = ArraySize*sizeof(int);
	int a[size], *devA; // ตรงนี้ a[size] จะกำหนดขนาดด้วย 1314 เรียบร้อยแล้ว
	for (i=0; i< ArraySize; i++)
	{
		a[i] = i + 1; 	
	}	
	cudaMalloc( (void**)&devA,size);
	cudaMemcpy( devA, a, size, cudaMemcpyHostToDevice);

	vecMultiply<<<1, 256>>>(devA); // 1 , 256 mean send each data with total thread 256 threads
	printf("Before\n");
	for (i=0; i< ArraySize; i++)
	{
		printf("%d ", a[i]);	
	}	
	printf("\n");

	cudaMemcpy(a, devA, size, cudaMemcpyDeviceToHost);
	cudaFree(devA);
	printf("After\n");
	for (i=0; i < ArraySize; i++)
    {
		printf("%d ",a[i]);
	}
	printf("\n");
}
```