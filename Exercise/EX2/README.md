# Exercise 2 : MPI Basics

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

### ข้อ 2.

Write an MPI program with two processes working as follows:

    - Process 0 sends an integer number to process 1
    - Process 1 calculates the square of the number, and sends the result to process 0
    - Process 0 prints out result

โจทย์ต้องการให้โปรเซส 0 ส่งค่าไปให้โปรเซส 1 แล้วให้โปรเซส 1 นั้นไปคิดค่ายกกำลังแล้วส่งคำตอบกลับมาที่โปรเซส 0 และให้โปรเซส 0 จะแสดงผลลัพธ์ออกมา

```C
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

### ข้อ 3.

Write an MPI program with two processes working as follows:<br>

    - Process 0 sends an array of 10 integers to process 1
    - Process 1 multiplies 10 to each element in an the array, and sends the result array back to process
    - Process 0 prints out result

โจทย์ต้องการให้โปรเซส 0 ส่งค่าไปให้โปรเซส 1 แล้วให้โปรเซส 1 นั้นไปคูณค่าในอาเรย์แต่ละตัวด้วย 10 แล้วส่งคำตอบกลับมาที่โปรเซส 0 และให้โปรเซส 0 จะแสดงผลลัพธ์ออกมา

```C
#include <stdio.h>
#include <mpi.h>
#include <math.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int msgtag;
    int x[10], i, mul[10]; // ประกาศขนาดอาเรย์ทั้งสองตัวไว้ว่าสามารถจุค่าได้ 10 ตัว
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        for (i = 0; i < 10; i++) // สร้าง for loop เพื่อมารับค่าลงอาเรย์ทั้งหมด 10 ตัว
        {
            printf("Enter the number %d:  \n", i + 1);
            fflush(stdout);
            scanf("%d", &x[i]);
        }
        MPI_Send(x, 10, MPI_INT, 1, 1234, MPI_COMM_WORLD); // ส่งค่าไป rank 1
        MPI_Recv(mul, 10, MPI_INT, 1, 1234, MPI_COMM_WORLD, &status); // รับค่าจาก rank 1
        for (i = 0; i < 10; i++)
        {
            printf("processor %d got %d\n", rank, mul[i]); // แสดงผลลัพธ์จากการส่งข้อมูลจาก rank 1 ไปยัง rank 0
        }
    }
    if (rank == 1) {
        MPI_Recv(x, 10, MPI_INT, 0, 1234, MPI_COMM_WORLD, &status); // รับค่าจาก rank 0
        for (i = 0; i < 10; i++) 
        {
            mul[i] = x[i] * 10; // เอาค่าจาก rank 0 มาคูณด้วย 10
        }
        MPI_Send(mul, 10, MPI_INT, 0, 1234, MPI_COMM_WORLD); // ส่งค่ากลับไปหา rank 0
    }

    MPI_Finalize(); // จบการทำงานของ MPI

    return 0;
}
```


### ข้อ 4.

Create MPI program with 1 master and 8 slave processes.<br>
The master process initializes an 8x8 matrix A,which eaxh element A[i][j] = i + j.<br> Then, the master sends a distinct row of matrix A to each slave.<br> Each slave calculates the summation of all elements in row it has received from the master.<br> Finally, all slaves send back the results to the master for aggregation into the total summation.<br>

โจทย์ให้เราสร้าง MPI ทั้งหมด 9 โปรเซส ประกอบด้วย ตัวหลัก 1 ตัว และลูกย่อยอีก 8 ตัว. โดยตัว Master จะทำการสร้างเมทริกซ์ หรือ อาเรย์สองมิติ. โดยตัว Master จะทำการส่งค่าแต่ละแถวไปให้ slave. และ slave แต่ละตัวจะทำการบวกค่าทั้งหมดในแถวที่ได้รับจาก Master จากนั้น Slave จะทำการส่งค่ากลับไปให้ Master เพื่อแสดงคำตอบที่ได้ทั้งหมด

```C
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int totalSize = 8; // Declare slave value as 8
    int mpiCommSize;
    int rank; 
    int i, j;

    MPI_Status status;
    int data[totalSize][totalSize];
    int receive[totalSize];

    int COMM_TAG = 1234;

    int localSum, total = 0;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &mpiCommSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) 
    {
        for (i = 0; i < totalSize; i++) // use for loop to increment value from 0 to 8 (7 elements)
        {
            for (j = 0; j < totalSize; j++) 
            {
                data[i][j] = i + j;
            }
        }

        for (i = 1; i <= totalSize; i++) // use for loop to send and receive value to each slave
        {
            MPI_Send(&data[i - 1], totalSize, MPI_INT, i, COMM_TAG, MPI_COMM_WORLD);
            MPI_Recv(&localSum, 1, MPI_INT, i, COMM_TAG, MPI_COMM_WORLD, &status);
            total += localSum;
        }
        printf("%d\n", total);

    } 
    else 
    {
        localSum = 0;
        MPI_Recv(&receive, totalSize, MPI_INT, 0, COMM_TAG, MPI_COMM_WORLD, &status); // Receive value from each slave
        for (i = 0; i < totalSize; i++) // using for loop to calculate total value from each value
        {
            localSum += receive[i];
        }
        MPI_Send(&localSum, 1, MPI_INT, 0, COMM_TAG, MPI_COMM_WORLD); // send value to each slave
    }
    MPI_Finalize(); // End MPI process
}
```


วิธีรันโจทย์ข้อนี้

```bash
mpirun -np 9 ./exercise3 # 9 คือโปรเซสทั้งหมด (Master 1 ตัว และ Slave อีก 8 ตัว)
```