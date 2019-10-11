# Lab 2 MPI

MPI ย่อมาจาก Message Passing Interface เป็นไลบราลีมาตรฐานสำหรับเขียนโปรแกรมแบบ message passing บนแพลตฟอร์มประสิทธิภาพสูง(HPC) เช่น เครื่อง ซุปเปอร์คอมพิวเตอร์  ระบบคลัสเตอร์ หรือกริด เป็นต้น

วิธีคอมไพล์ MPI

```bash
mpicc -o ชื่อoutput ชื่อไฟล์ที่ต้องการคอมไพล์.c
```

วิธีรันไฟล์
```bash
mpirun -np จำนวนโปรเซส ./ชื่อไฟล์ที่รัน # np คือ number process
```
หรือ
```bash
mpiexec -np จำนวนโปรเซส ./ชื่อไฟล์ที่รัน
```



## สิ่งที่ต้องใช้ 
```C
// Ubuntu ที่ติดตั้ง GCC และ mpi เรียบร้อยแล้ว
#include <mpi.h> // ไลบรารี่ MPI
MPI_Init (&argc, &argv);
MPI_Comm_size(MPI_COMM_WORLD, &size);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Send(&ตัวที่จะส่ง, ขนาด, ชนิดข้อมูล, ส่งให้ใคร, tag,MPI_COMM_WORLD); 
MPI_Recv(&ตัวที่จะรับ, ขนาด, ชนิดข้อมูล, รับจากใคร, tag,MPI_COMM_WORLD,&status);
MPI_Finalize();
```

อธิบายทีละตัว

**MPI_Init (&argc, &argv);** คือ ฟังก์ชั่นเริ่มต้นของ MPI  เพื่อกำหนดสภาวะแวดล้อมของระบบให้สามารถรัน MPI โปรแกรมได้ ฟังชั่นนี้ กำหนดให้เรียกใช้ทุกครั้งสำหรับโปรแกรม MPI และต้องเรียกใช้ก่อนทุกๆฟังก์ชั่นของ MPI ด้วย  พารามิเตอร์ที่กำหนดให้ฟังก์ชั่นนี้คือ command line argument ของฟังก์ชั่น main ในภาษา C 

**MPI_Comm_size(MPI_COMM_WORLD, &size);** คือ ฟังก์ชั่นสำหรับเรียกดูจำนวนของ **process** ที่อยู่ในกลุ่มของ **communicator** ตัวเดียวกัน พารามิเตอร์สำหรับฟังก์ชั่นนี้คือ **communicator** (โดยทั่วไปเราจะใช้ MPI_COMM_WORLD ) และ **address** ของตัวแปรชนิด **integer**

**MPI_Comm_rank(MPI_COMM_WORLD, &rank);** คือ ฟังก์ชั่นสำหรับ เรียกดูหมายเลข **process** ใดๆ  ซึ่งเลขที่ได้จะมีค่าเริ่มต้นที่ **0** จนถึงจำนวน **process** ทั้งหมด **-1**
พารามิเตอร์ได้แก่ **communicator** และ **address** ของตัวแปรชนิด **integer**

**MPI_Send(&ตัวที่จะส่ง, ขนาดข้อมูล, ชนิดข้อมูล, ส่งให้ใคร, tag,MPI_COMM_WORLD);** คือ ฟังก์ชั่นส่ง **message** จาก **buffer**  ของ **process** ต้นทาง ให้กับ **process** ปลายทาง 

**MPI_Recv(&ตัวที่จะรับ, ขนาดข้อมูล, ชนิดข้อมูล, รับจากใคร, tag,MPI_COMM_WORLD,&status);** คือ ฟังก์ชั่นรับ **message** จาก **process** อื่น

**MPI_Finalize();** ฟังก์ชั่นสิ้นสุดการทำงานของ โปรแกรม MPI เป็นฟังก์ชั่นที่เรียกตอนสุดท้าย และต้องเรียกทุกครั้งเช่นเดียวกับ **MPI_Init()**

### ข้อ 1. 

สร้างแลปด้วย MPI

```C
#include "mpi.h" 
#include <stdio.h> 
int main(int argc,char *argv[]) 
{    
    int rank, size; // สร้างตัวแปรขึ้นมาเก็บตัวโปรเซส กับ ขนาดโปรเซส
    char hostname[MPI_MAX_PROCESSOR_NAME]; // สร้าง char เพื่อมารับค่า host-name PC(เครื่องที่รัน)  
    int resultlen; //   
    MPI_Init( &argc, &argv ); 
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );    
    MPI_Comm_size( MPI_COMM_WORLD, &size );  ฟังก์ชั่นสำหรับเรียกดูจำนวนของ process  
    MPI_Get_processor_name(hostname,&resultlen); // รับชื่อของ โปรเซสเซอร์นั้นๆ   
    printf( "Hello! I'm %d of %d running on %s\n",rank,size,hostname);   
    MPI_Finalize();   
    return 0; 
} 
```

### ข้อ 2.

```C
#define LEFT 1 
#define RIGHT 1000 
#include <stdio.h> 
#include <mpi.h> 
int main(int argc, char *argv[]) 
{    
    int rank, size;    
    MPI_Status status;    
    int interval;    
    int number, start, end, sum, GrandTotal;    
    int proc;    
    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );    
    MPI_Comm_size( MPI_COMM_WORLD, &size );    
    if (rank == 0) // ถ้า rank หรือ process โปรเซสที่ 0
    { 
        GrandTotal = 0;       
    for (proc=1; proc<size; proc++) 
    {                    
        MPI_Recv(&sum,1,MPI_INT,proc,123,MPI_COMM_WORLD,&status);  
        GrandTotal=GrandTotal+sum;       
    }       
        printf("Grand total = %d \n", GrandTotal);    
    }    
    else  // ถ้า rank หรือ process โปรเซสที่ไม่ใช่ 0 คือ slave
    {       
        interval=(RIGHT-LEFT+1)/(size-1);        
        start=(rank-1)*interval+LEFT;       
        end=start+interval-1;       
        if (rank == (size-1)) 
        {/* for last block */          
        end = RIGHT;        
        }       
        sum=0; /*Sum locally on each proc*/       
        for (number=start; number<=end; number++)          
        sum = sum+number; 
        /*send local sum to Master process*/        
        MPI_Send(&sum,1,MPI_INT,0,123,MPI_COMM_WORLD);    
    }    
    MPI_Finalize(); 
} 
```

### ข้อ 3.

Modify the integersum.c program such that it prints out the starting number, ending number, and local summation computed by each slave process. The following is one possible output.

```C
#define LEFT 1 
#define RIGHT 1000 
#include <stdio.h> 
#include <mpi.h> 
int main(int argc, char *argv[]) 
{    
    int rank, size;    
    MPI_Status status;    
    int interval;    
    int number, start, end, sum, GrandTotal;    
    int proc;    
    MPI_Init( &argc, &argv ); 
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );    
    MPI_Comm_size( MPI_COMM_WORLD, &size );    
    if (rank == 0) // ถ้า rank หรือ process โปรเซสที่ 0
    { 
        GrandTotal = 0;       
    for (proc=1; proc<size; proc++) 
    {                    
        MPI_Recv(&sum,1,MPI_INT,proc,123,MPI_COMM_WORLD,&status);  
        GrandTotal=GrandTotal+sum;       
    }       
        printf("Grand total = %d \n", GrandTotal);    
    }    
    else  // ถ้า rank หรือ process โปรเซสที่ไม่ใช่ 0 คือ slave
    {       
        interval=(RIGHT-LEFT+1)/(size-1);        
        start=(rank-1)*interval+LEFT;       
        end=start+interval-1;       
        if (rank == (size-1)) 
        {/* for last block */          
        end = RIGHT;        
        }       
        sum=0; /*Sum locally on each proc*/       
        for (number=start; number<=end; number++)          
        sum = sum+number;       /*send local sum to Master process*/      
        printf("RANK %d: ,start %d, end %d, local sum %d\n",rank,start,end,sum);  
        MPI_Send(&sum,1,MPI_INT,0,123,MPI_COMM_WORLD);    
    }    
    MPI_Finalize(); 
} 
```