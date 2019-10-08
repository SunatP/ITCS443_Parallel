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

### ข้อ 2.

```c
Write an OpenMP program to find the summation of values in A[] using reduction clause (+operator). Verify your output with the sequential version.
```
