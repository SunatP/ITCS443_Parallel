# Lab 1 OpenMP

## สิ่งที่ต้องใช้

```c
// Ubuntu ที่ติดตั้ง GCC และ omp เรียบร้อยแล้ว
#include <omp.h> // ไลบรารี่ OpenMP
#pragma omp parallel // สำหรับการแบ่งงานให้แต่ละ Thread
```

### ข้อ 1.

ลองสร้างโค้ดที่รันด้วย OpenMP

```C
#include <omp.h> 
#include <stdio.h> 
int main() 
{    
    #pragma omp parallel num_threads(4)    
    {       
        int tid = omp_get_thread_num(); // สร้าง omp เพื่อรับ thread number       
        int nthreads = omp_get_num_threads(); // สร้างมาจำนวน thread      
        printf("Hello World from thread %d of %d\n", tid, nthreads);    
    } 
} 
```
## Pragma คืออะไร

```C
#pragma omp parallel num_threads(4)  
// num_threads(4) คือ number of threads ไว้สำหรับการรันแบบ Parallel
```

Pragma เป็นชุดคำสั่งชนิดหนึ่งสามารถเปิดปิดฟังก์ชั่นบางอย่างได้ เช่น pragma omp การใช้ pragma omp คือการอนุญาตให้โค้ดนั้นสามารถคอมไพล์ (Complie) ลงถึงระดับ OS (Operating System) ในรููปแบบขนาน(Parallel)

### ข้อ 2.

การลดรูป หรือ clause reduction

```c
#pragma omp parallel for reduction(+:sum) // แบบนี้
```

โค้ด

```C
#include <stdio.h> 
#include <omp.h> 
int main() 
{    
    int i;    
    int sum = 0;    
    #pragma omp parallel for reduction(+:sum) // ใช้ reduction เพื่อลดรูปเครื่องหมายสมการ
    for (i=1; i <= 1000; i++)       
    sum = sum + i;    
    printf("%d\n",sum);
} 
```

### ข้อ 3.

การใช้ omp เพื่อเข้า critical section (หรือ Critical region ใน Mutex ของวิชา OS)

```C
#include <omp.h> 
#include <stdio.h> 
int main() 
{    
    int x = 0;    
    int i;    
    #pragma omp parallel private(i) shared(x) num_threads(4)   
    {       
        for (i=0; i < 10000; i++) 
            #pragma omp critical          
            x = x + 1;    
    }  /* end of parallel region */    
    printf("%d\n", x);  // The correct output is 40000 
}
```

```c
#pragma omp critical
```

### omp critical คืออะไร

omp critical หรือ Critical Region คือ การโปรเซสชุดคำสั่งหรือ code ใน thread(เธรด)เดียว โดยถ้าเจอ omp critcal จะโปรเซสโค้ดตรงนั้นก่อน โดยไม่สนใจว่าบรรทัดไหนมาก่อนหรือหลัง<br> เช่น

```c
#pragma omp critical
x = x + 1;
!$OMP CRITICAL
X = X + 1
!$OMP END CRITICAL(Very inefficiant) 

example using critical instead of reduction:
    #pragma omp parallel for
    for (int i=0; i < n; i++)
    {
    double product = a[i] * b[i];
    #pragma omp critical
    result=result + product;
    }
```


```C
#pragma omp parallel private(i) shared(x) num_threads(4)
// ประกาศค่า i ใช้ได้แค่ใน omp parallel
// ประกาศค่า x ให้ใช้ได้แบบ global
// ประกาศค่า num_thread เป็น 4 เพื่อใช้การโปรเซสแบบ parallel
```

### ข้อ 4. 

แก้โจทย์จาก ข้อ 2. ให้บวกค่าทีละ 2

```C
#include <stdio.h> 
#include <omp.h> 
int main() 
{    
    int i;    
    int sum = 0;    
    #pragma omp parallel for reduction(+:sum)    
    for (i=0; i <= 1000; i+=2) // แก้จาก i++ เป็น i+=2       
        sum = sum + i;    
    printf("%d\n",sum);
} 
```