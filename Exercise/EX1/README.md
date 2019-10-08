# Exercise 1 : OpenMP

```C
#include <stdio.h>
#include <stdlib.h>
int main()
{
    int A[100];
    srand(1234);
    for(i=0;i<100;i++)
    {
        A[100] = rand()%1000;
    }
}
```
## สิ่งที่ต้องใช้ 
```C
// Ubuntu ที่ติดตั้ง GCC และ omp เรียบร้อยแล้ว
#include <omp.h> // ไลบรารี่ OpenMP
#pragma omp parallel // สำหรับการแบ่งงานให้แต่ละ Thread
```

นี่คือโจทย์ที่เราจะต้องแก้ให้ตรงเงื่อนไขของอาจารย์

### ข้อ 1.
```c
Given an integer array A[100], write an OpenMP program that multiples 10 to each 
element of the array. Verify your output with the sequential version.
```
เราจะต้องใช้ OpenMP เข้ามาเพื่อคูณค่าแต่ละตัวลงอาเรย์

```c
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
int main()
{
    int A[100];
    srand(1234);
    #pragma omp parallel for // เติม for สำหรับการรัน for loop แบ่งงานให้แต่ละ Thread
    for(i=0;i<100;i++)
    {
        A[i] = rand()%1000;
        printf("%d",A[i]);
    }
    return 0;
}
```

### วิธีการคอมไพล์และรันไฟล์

วิธีการคอมไพล์โค้ดที่เขียนด้วย OpenMP มีวิธีดังนี้ (วิธีนี้ใช้บน Ubuntu)

    1. เปิด Terminal ในที่ที่เราเก็บโค้ดไว้
    2. พิมพ์คำสั่งนี้ลงไปแล้วกด Enter

```bash
gcc -o ชื่อไฟล์ที่จะเอาไว้รัน -fopenmp ชื่อไฟล์ที่เราสร้าง.c
# ตัวอย่าง
gcc -o openmp5 -fopenmp openmp5.c
```
    3. เมื่อ Enter แล้วถ้าไม่มี Error ให้พิมพ์คำสั่งต่อไปนี้เพื่อรัน

```bash
./openmp5 # เป็นต้น
```




### ข้อ 2.

```c
Write an OpenMP program to find the summation of values in A[] using reduction clause (+operator). Verify your output with the sequential version.
```
เราจะต้องใช้ reduction clause เพื่อลดรูปเครื่องหมายสมการ

```c
#include <omp.h> 
  
#include <stdio.h> 
#include <stdlib.h> 
  
int main(int argc, char* argv[]) 
{ 
  int a[100],sum;
  srand(1234);
    // Beginning of parallel region 
    #pragma omp parallel for // pragma สำหรับ for สำหรับการรัน for loop 
        for (int i = 0; i < 100; i++)
        {
            a[i] = rand() %1000;
            // a[i] = i;
            /* code */
            sum += a[i]; // Reduction clause instead 
        }
        printf("%d\n",sum);
    return 0;
    // Ending of parallel region 
} 
```

ตรง sum += a[i] คือการลดรูปจาก sum = sum + a[i] หรือที่เรียกว่า Reduction Clause 