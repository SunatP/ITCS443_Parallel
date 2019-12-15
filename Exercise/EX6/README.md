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
dim3 dimBlock(T,T); //  ก็คือเธรดในบล็อก หรือ ขนาดเธรดในมิติที่เรากำหนด
dim3 blockDim(T,T); // เหมือนกับ dimBlock นั่นแหละ
dim3 dimGrid(array_size/T - 1); // เป็นขนาดเส้นกริดของมิติที่เรากำหนด
dim3 gridDim(array_size/T - 1); // เหมือนกันกับ dimGrid
```

![explaindim](https://encrypted-tbn0.gstatic.com/images?q=tbn%3AANd9GcR6cr-c7jAYN32hBTJeJ5TVVJyv9nEhmagV06vSxG6zPqUUDbPo)

ในตัวอย่างคือเส้นกริดขนาด **4096** โดยที่แต่ละ **blockDim** นั้นในตัวอย่างจะแบ่งโดยใส่ **Thread** ลงใน **Block** ทั้งหมด 256 ตัว (0 - 255) ซึ่ง blockIdx.x 1 ตัว นั้นจะมีค่าเท่ากับ thread 256 ตัว ส่วน threadIdx นั้นจะอยู่ใน blockDim ซึ่งก็คือ (0 - 255) ไปจนครบ blockIdx.x (0 - 4095) หรือ gridDim(4096)

ตัวอย่างในโจทย์คือ threadIdx.x ตัวที่ 3 ใน blockIdx.x ที่ 2 นั้น โจทย์ถามหาว่าตัว threadIdx.x ตัวนี้อยู่ตรงไหน

```bash
    index =  blockIdx.x * blockDim.x + threadId.x
# ตำแหน่งที่จะหา = ตำแหน่ง block ที่threadโดนไฮไลท์(blockIdx.x) * ตัวเธรดที่อยู่ในบล็อกนั้นทั้งหมด(blockDim.x) + ตำแหน่งเธรดที่โดนไฮไลท์ (threadId.x)
    index = 2 * 256 + 3 = 515
```

**dim3** คือ integer ในรูปแบบเวกเตอร์ (Vector) มาจาก uint3 (unsigned integer3) ไว้สำหรับระบุมิติของอาเรย์ เมื่อเราใช้ dim3 เมื่อไหร่ก็ตาม ค่าที่ยังไม่ได้โดน assign จะถูก assign เป็น 1 เสมอ ส่วนผลลัพธ์ที่ได้นั้นจะเป็นแบบนี้<br>
![1](https://raw.githubusercontent.com/SunatP/ITCS443_Parallel/master/Exercise/EX6/img/1.PNG)


### 2. Write a CUDA program to copy array A into array B in reverse order using multiple thread blocks.  Both array A and B are of an arbitrary size N.

ข้อนี้ให้เราทำการคัดลอกค่าในอาเรย์ A ไปอาเรย์ B โดยที่ค่าในอาเรย์ B นั้นต้องเรียงลำดับจากท้ายสุดมาตัวแรก (Reverse) โดยทึ่ขนาดอาเรย์ของทั้งคู่นั้นสามารถกำหนดได้ตามใจตัวเองด้วยขนาด N

```C++
#include <stdio.h>
#define T 64 // As Threads
#define array_size 256

__global__ void vecMultiplyReverse(int *A, int *B) 
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    int Reverse = (T - 1) - i;
    B[Reverse] = A[i];
}

int main (int argc, char *argv[])
{
    int i;
    int size = T*sizeof(int);
    int a[T],b[T], *devA,*devB;
    for (i=0; i< T; i++)
    {
        a[i] = i + 1;
    }
    cudaMalloc( (void**)&devA,size);
    cudaMalloc( (void**)&devB,size);
    cudaMemcpy( devA, a, size, cudaMemcpyHostToDevice);
    cudaMemcpy( devB, b, size, cudaMemcpyHostToDevice);
    dim3 dimBlock(T);
    dim3 dimGrid(array_size/T - 1);
    vecMultiplyReverse<<<dimGrid,dimBlock>>>(devA,devB);
    printf("Before\n");
    for (i=0; i< T; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\n");

    cudaMemcpy(a, devA, size, cudaMemcpyDeviceToHost);
    cudaMemcpy(b, devB, size, cudaMemcpyDeviceToHost);
    cudaFree(devA);
    cudaFree(devB);
    printf("After\n");
    for (i=0; i < T; i++) 
    {
        printf("%d ",b[i]);
    }
    printf("\n");
}
```

มาดูตรงนี้ 

```C++
__global__ void vecMultiplyReverse(int *A, int *B) 
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    int Reverse = (T - 1) - i;
	B[Reverse] = A[i];
}
```

ตรงนี้คือ **Kernel** ที่เอาไว้ Reverse ค่านั่นแหละ **ตัวอย่าง**

```Bash
A         : 1 2 3 4 5 6 7 8 9 10
A Reverse : 10 9 8 7 6 5 4 3 2 1
```

แบบนี้เป็นต้น ซึ่งเราจะใช้ Multiple Thread หรือตรงบรรทัด **int i = blockIdx.x * blockDim.x + threadIdx.x;** อันนี้แหละ และผลลัพธ์ที่จะได้เป็นแบบนี้
![2](https://raw.githubusercontent.com/SunatP/ITCS443_Parallel/master/Exercise/EX6/img/2.PNG)

### 3. Given two arrays A[N] and B[N], write a CUDA program to create the array C[N] such that

```C++
C[i] = A[i] + B[i];  // if i is even 
C[i] = A[i] - B[i]; // if i is odd 
```
เราจะใช้อาเรย์สามตัวในข้อนี้โดยมีข้อกำหนดว่าถ้าเป็นเลขคู่ให้เอาอาเรย์ A บวกกับอาเรย์ B แต่ถ้าเกิดเป็นเลขคี่ให้เราเอาอาเรย์ A - B แทน

```C++
#include <stdio.h>
#define T 16 // As Threads
#define array_size 64

__global__ void vecMultiplyReverse(int *A, int *B, int *C) 
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if(i%2 == 0)
    {
        C[i] = A[i] + B[i];
    }
    else if(i%2 != 0)
    {
        C[i] = A[i] - B[i];
    }
}

int main (int argc, char *argv[])
{
    int i;
    int size = T*sizeof(int);
    int a[T],b[T],c[T], *devA,*devB,*devC;
    for (i=0; i< T; i++)
    {
        a[i] = i + 2;
        b[i] = i + 1; 
    }

    cudaMalloc( (void**)&devA,size);
    cudaMalloc( (void**)&devB,size);
    cudaMalloc( (void**)&devC,size);
    cudaMemcpy( devA, a, size, cudaMemcpyHostToDevice);
    cudaMemcpy( devB, b, size, cudaMemcpyHostToDevice);
    cudaMemcpy( devC, c, size, cudaMemcpyHostToDevice);
    dim3 dimBlock(T);
    dim3 dimGrid(array_size/T - 1);
    vecMultiplyReverse<<<dimGrid,dimBlock>>>(devA,devB,devC);
    printf("Before A: \n");
    for (i=0; i< T; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\n");

    printf("Before B: \n");
    for (i=0; i< T; i++)
    {
        printf("%d ", b[i]);
    }
    printf("\n");

    cudaMemcpy(a, devA, size, cudaMemcpyDeviceToHost);
    cudaMemcpy(b, devB, size, cudaMemcpyDeviceToHost);
    cudaMemcpy(c, devC, size, cudaMemcpyDeviceToHost);
    cudaFree(devA);
    cudaFree(devB);
    cudaFree(devC);
    printf("After\n");
    for (i=0; i < T; i++)
    {
        printf("%d ",c[i]);
    }
    printf("\n");

}
```

มาดูตรง Kernel ตรงนี้

```C++
__global__ void vecMultiplyReverse(int *A, int *B, int *C) 
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if(i%2 == 0) // ตรงนี้คือถ้าเราหารด้วย2ลงตัวโดยไม่เหลือเศษ(Mod) จะเป็นเลขคู่ 
    {
        C[i] = A[i] + B[i]; 
    }
    else if(i%2 != 0) // ตรงนี้คือถ้าเราหารด้วย2ไม่ลงตัวโดยเหลือเศษ(Mod) จะเป็นเลขคี่
    {
        C[i] = A[i] - B[i];
    }
}
```

```C++
for (i=0; i< T; i++)
{
    a[i] = i + 2; //ตำแหน่งอาเรย์ A
    b[i] = i + 1; //ตำแหน่งอาเรย์ B
}
```

ที่ต้องบวกด้วยเลขที่ต่างกันนั้นเพื่อจะให้เห็นผลลัพธ์นั่นเอง ตัวอย่าง
```bash
Index   : 0 1 2 3 4  5  6 7   8  9
A       : 2 3 4 5 6  7  8 9  10 11
Pos|Neg : + - + - +  -  + -   +  -
B       : 1 2 3 4 5  6  7 8   9 10
Result  : 3 1 7 1 11 1 15 1  19  1 
```
ผลลัพธ์ที่ได้<br>
![3](https://raw.githubusercontent.com/SunatP/ITCS443_Parallel/master/Exercise/EX6/img/3.PNG)


### 4. Create a CUDA program to increase the value of each element in an NxN matrix by one. That is Ai,j = Ai,j + 1, for each i,j = 0 to N-1 using multiple thread blocks.  N could be any arbitrary size.

ข้อนี้ใช้ CUDA นั่นแหละ โดยในโค้ดจะให้บวกค่าเข้าไปอีก 1 ลงในช่องอาเรย์แต่ละตัวโดยเมทริกซ์มีขนาด NxN โดยที่ อาเรย์ A ตำแหน่ง (i,j) จะเท่ากับ A(i,j) + 1 ซึ่งในแต่ละตำแหน่งของ i และ j จะเริ่มตั้งแต่ 0 ถึง N - 1 โดยใช้ Multiple Thread Block และ N มีขนาดเท่าไหร่ก็ได้

```C++
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

}
```
มาดูตรงนี้
```C++
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
```
ตรงนี้คือการกำหนดให้ Matrix มีขนาด NxN และบวกค่าเข้าไปอีก 1

```C++
    dim3 dimBlock(T,T);
    dim3 dimGrid((N+ dimBlock.x - 1)/ dimBlock.x ,(N + dimBlock.y - 1) / dimBlock.y);
```
ตรงนี้จะทำการแบ่งขนาด Block และเส้นกริดให้โดยอัตโนมัติ ผลลัพธ์ที่ได้จะออกมาแบบนี้<br>
![4](https://raw.githubusercontent.com/SunatP/ITCS443_Parallel/master/Exercise/EX6/img/4.PNG)

### 5. Write a CUDA program to create the transpose matrix B from a square matrix A of an arbitrary size N using multiple thread blocks. Recall that if B is the transpose of A, then Bi,j = Aj,i. 

โจทย์ข้อนี้้จะให้เอาข้อเมื่อกี้นี้แหละมาทำทรานโพสเมทริกซ์นั่นแหละ ไม่มีอะไรมาก ไปดูโค้ดเลย

```C++
#include <stdio.h>
#define T 8 // As Threads
#define N 16

__global__ void vecMatrix(int *A, int *B) 
{
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y+ threadIdx.y;
    int width = gridDim.x * T;
    for( int j = 0; j<T; j+=N )
    {
        B[x*width + (j+y)] = A[(y+j)*width + x];
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
    int newline = 0;

    for (i=0; i < N * N; i++) 
    {
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
```

ข้อแตกต่างระหว่างข้อที่ 4 กับ 5 คือ

```C++
// ข้อ 4.
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
```
และ
```C++
// ข้อ 5 
__global__ void vecMatrix(int *A, int *B) 
{
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y+ threadIdx.y;
    int width = gridDim.x * T;
    for( int j = 0; j<T; j+=N )
    {
        B[x*width + (j+y)] = A[(y+j)*width + x];
    }

}
```
จากข้อ 4. โดยการเอา if condition ออกแล้วใส่ for loop เข้าไปแทนเพื่อทำการ Transpose ตัวเมตริกซ์นั่นเอง ผลลัพธ์จะได้แบบนี้ <br>
![5](https://raw.githubusercontent.com/SunatP/ITCS443_Parallel/master/Exercise/EX6/img/5.PNG)
<br>
อะไรคือการ Transpose<br>
![5.1](https://www.studymite.com/wp-content/uploads/2018/12/transpose.jpg)

Transpose คือการเปลี่ยนข้อมูลในเมทริกซ์จาก Row เป็น Column หรือ Column เป็น Row ประมาณนี้<br>

