# Exercise 2 : MPI Basics

MPI ย่อมาจาก Message Passing Interface เป็นไลบราลีมาตรฐานสำหรับเขียนโปรแกรมแบบ message passing บนแพลตฟอร์มประสิทธิภาพสูง(HPC)  เช่น เครื่อง ซุปเปอร์คอมพิวเตอร์  ระบบคลัสเตอร์ หรือกริด เป็นต้น

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

**MPI_Send(&ตัวที่จะส่ง, ขนาด, ชนิดข้อมูล, ส่งให้ใคร, tag,MPI_COMM_WORLD);** คือ ฟังก์ชั่นส่ง **message** จาก **buffer**  ของ **process** ต้นทาง ให้กับ **process** ปลายทาง 

**MPI_Recv(&ตัวที่จะรับ, ขนาด, ชนิดข้อมูล, รับจากใคร, tag,MPI_COMM_WORLD,&status);** คือ ฟังก์ชั่นรับ **message** จาก **process** อื่น

**MPI_Finalize();** ฟังก์ชั่นสิ้นสุดการทำงานของ โปรแกรม MPI เป็นฟังก์ชั่นที่เรียกตอนสุดท้าย และต้องเรียกทุกครั้งเช่นเดียวกับ **MPI_Init()**

### ข้อ 2.

Write an MPI program with two processes working as follows:

    - Process 0 sends an integer number to process 1
    - Process 1 calculates the square of the number, and sends the result to process 0
    - Process 0 prints out result

โจทย์ต้องการให้โปรเซส 0 ส่งค่าไปให้โปรเซส 1 แล้วให้โปรเซส 1 นั้นไปคิดค่ายกกำลังแล้วส่งคำตอบกลับมาที่โปรเซส 0 และโปรเซส 0 จะแสดงผลลัพธ์ออกมา

```C++
#include <stdio.h>
#include <mpi.h>
#include <math.h>

int main(int argc, char *argv[])
{
    int rank,size,x,square;
    MPI_Status status;
    MPI_Init (&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if(rank == 0){ // rank 0 คือโปรเซสที่ 0
        // printf("Enter the number: ");
        // fflush(stdout);
        scanf("%d", &x); // receive data
        MPI_Send(&x, 1, MPI_INT, 1, 1234, MPI_COMM_WORLD); // Send parameter 
        printf("processor %d sent %d\n", rank, x);
        MPI_Recv(&square, 1, MPI_INT, 1, 1234, MPI_COMM_WORLD, &status); // Like math pow
        printf("processor %d got %d\n", rank, square);
    }
    if(rank == 1){ // rank 1 คือโปรเซสที่ 1
        MPI_Recv(&x, 1, MPI_INT, 0, 1234, MPI_COMM_WORLD, &status); // Receive parameter value
        printf("processor %d got %d\n", rank, x);
        square = x*x;
        MPI_Send(&square, 1, MPI_INT, 0, 1234, MPI_COMM_WORLD); // Send Parameter back
        printf("processor %d sent %d\n", rank, square);
    }

    MPI_Finalize(); // End MPI Process

    return 0;
}
```
