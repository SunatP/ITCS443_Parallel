# Exercise 3 : Collective Communication

สิ่งที่ต้องใช้ 
    1. mpi.h
    2. MPI_Init()
    3. MPI_Comm_size()
    4. MPI_Comm_rank()
    5. MPI_Scatter()
    6. MPI_Reduce()
    7. MPI_Finalize()

### ข้อ 1.

Write a parellel program to find the maximum, minimum, and average values in an array of integers A[100] using 
 processes as follows

  * Process 0 initializes the array A by the following code fragment

```c
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int A[100];
    srand(1234);
    for(i = 0; i < 100 ; i++)
    A[i] = rand() % 1000;
}
```
   
 * Process 0 distributes the Array A to all processes (including itself) by MPI_Scatter ส่งข้อมูลให้กับทุกโปรเซสที่มี
 * All processes find the local maximum, minimum, and average (sum) values, and then perform reduction for the global values ให้ทุกโปรเซสหาค่าที่มากที่สุด ค่าที่น้อยที่สุด และค่าเฉลี่ยโดยใช้ MPI_Reduce()
 * Check the result with the sequential version executed at Process 0 เช็คคำตอบที่โปรเซส 0
 * Hint: Use separate MPI_Reduce for global maximum, minimum and average values

## อะไรคือ MPI_Scatter, MPI_Bcast,MPI_Reduce, MPI_Gather

MPI_Scatter คือฟังก์ชันที่ทำงานโดยการกระจายข้อมูลจากโปรเซส root ไปยังโปรเซสทั้งหมด แต่ไม่เหมือนกับ MPI_Bcast ตรงที่ MPI_Bcast จะส่งข้อมูลทั้งหมดไปให้กับทุกโปรเซส แต่ MPI_Scatter จะแบ่งข้อมูลเป็นส่วนๆ และส่งแต่จะส่วนให้กับแต่ละโปรเซส

![MPI](http://pages.tacc.utexas.edu/~eijkhout/pcse/html/graphics/collective_comm.jpg)


```C
MPI_Scatter(
    const vorank *sendbuf,  // &ตัวที่จะส่ง data
    int sendcount,        // ส่งข้อมูลเป็นจำนวน
    MPI_Datatype sendtype,// ชนิดข้อมูลที่จะส่ง
    vorank *recvbuf,        // &ตัวที่จะรับ data
    int recvcount,        // จำนวนข้อมูลที่รับ
    MPI_Datatype recvtype, // ประเภทข้อมูล
    int root,             // rank ที่ส่งโปรเซส
    MPI_Comm comm)        // ตัว communicator
```

MPI_Bcast คือฟังก์ชันสำหรับกระจายข้อมูลจากโปรเซสหนึ่งไปยังโปรเซสที่เหลือทั้งหมด 


![Bcast](https://mpitutorial.com/tutorials/mpi-scatter-gather-and-allgather/broadcastvsscatter.png)
```C
MPI_Bcast(
    vorank* data,             // &ตัวส่ง data
    int count,              // จำนวนข้อฒูลที่จะส่ง
    MPI_Datatype datatype,  // ชนิดข้อมูล
    int root,               // rank เริ่มต้น
    MPI_Comm communicator)  // ตัว communicator
```

MPI_Reduce คือฟังก์ชันสำหรับรวมข้อมูลของทุกโปรเซสมายังโปรเซส root

```C
MPI_Reduce(
    vorank* send_data,        // &ตัวส่ง data
    vorank* recv_data,        // &ตัวรับ data
    int count,              // จำนวนข้อมูลที่จะส่ง
    MPI_Datatype datatype,  // ชนิดข้อมูล
    MPI_Op op,              // Operator เช่น MPI_SUM, MPI_MAX, MPI_MIN
    int root,               // rank เริ่มต้นที่ส่งโปรเซส
    MPI_Comm communicator)  // ตัว communicator
```

MPI_Gather คือฟังก์ชั่นตรงกันข้ามกับ Scatter โดยฟังก์ชันนี้จะรวมงานแต่ละโปรเซสเข้ามารวมเป็นอันเดียว

![Gather](https://mpitutorial.com/tutorials/mpi-scatter-gather-and-allgather/gather.png)

```C
MPI_Gather(
    void* send_data,  // &ตัวส่ง data
    int send_count,   // จำนวนข้อมูลที่จะส่ง
    MPI_Datatype send_datatype, // ชนิดข้อมูล
    void* recv_data, // &ตัวรับ data
    int recv_count, // รับ data เป็นจำนวนเท่าไหร่
    MPI_Datatype recv_datatype, // รับ data เป็นชนิดไหน
    int root,       // rank เริ่มต้นที่ส่งโปรเซส
    MPI_Comm communicator)  // ตัว communicator

```


### ข้อ 1. จริงๆแล้วนะ.

เริ่มเขียนโค้ด

```C
#include "mpi.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main(int argc,char *argv[]) {
    int i=0,total_size = 100, size = 25,reciver[size];
    int min[size],max[size],sum[size],avg=0;
    int A[total_size];
    int rank, numprocs;
    MPI_Init(&argc,&argv); 
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    if(rank == 0)
    {
        srand(1234);
        for (i=0; i < total_size; i++)
        {
            A[i] = rand()%1000;
        }
    }
    MPI_Scatter(A,size,MPI_INT,&reciver,size,MPI_INT,0,MPI_COMM_WORLD); 
    // กระจาย value ไปให้แต่ละโปรเซส โดยที่ value ที่ให้จะไม่ซ้ำกัน
    MPI_Reduce ( &reciver, &min, size, MPI_INT,MPI_MIN, 0,MPI_COMM_WORLD ); 
    // ใช้ Reduce() หาค่าน้อยที่สุดจาก reciver
    MPI_Reduce ( &reciver, &max, size, MPI_INT,MPI_MAX, 0,MPI_COMM_WORLD ); 
    // ใช้ Reduce() หาค่ามากที่สุดจาก reciver
    MPI_Reduce ( &reciver, &sum, size, MPI_INT,MPI_SUM, 0,MPI_COMM_WORLD ); 
    // ใช้ Reduce() หาผลรวมจาก reciver
    if(rank==0)
    {
        printf("Local maximum: ");
        for (i=0; i < size; i++) printf("%d\n",max[i]);
        printf("\n");
        printf("Local minimum: ");
        for (i=0; i < size; i++) printf("%d\n",min[i]);
        printf("\n");
        printf("Average (sum): ");
        for (i=0; i < size; i++) printf("%d\n",sum[i]/size);
        printf("\n");

        int lastmax=A[0];
        int lastmin=A[0];
        int lastsum=0;
        for(i=0;i<total_size;i++)
        {
            if(A[i]>lastmax)
            {
                lastmax=A[i];
            }
            if(A[i]<lastmin)
            {
                lastmin=A[i];
            }
            lastsum+=A[i];
        }
        printf("Max of A %d\n",lastmax);
        printf("Min of A %d\n",lastmin);
        printf("Average (sum) of A %d\n",lastsum/total_size);
    }
    MPI_Finalize();
}

```

### ข้อ 2. Parallel Search

    - Create an array of random integers in Process 0 
    - สร้างอาเรย์ที่สุ่มค่าได้ในโปรเซสที่ 0
    - Distribute distinct part of the array to all process
    - ส่งค่ออาเรย์ที่ได้ให้กับอาเรย์ในทุกโปรเซส
    - Process 0 receives an input integer i from a user, and broadcasts it to all
    - โปรเซส 0 รับค่าจากผู้ใช้ (ค่า i) และส่งค่าโดยใช้ MPI_Bcast ให้ทุกโปรเซส
    - Count (in parallel) the number of elements in the array that is less than the input integer i. Use MPI_Reduce to complete this step
    - นับค่าทั้งหมดที่เจอ (Sum) ในอาเรย์โดยมีเงื่อนไขว่าค่าที่เอามานับนั้นน้อยกว่าค่า i. ใช้ฟังก์ชั่น MPI_Reduce
    - Display the result
    - แสดงค่าที่ได้

เริ่มเขียนโค้ด

```c
#include "mpi.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main(int argc,char *argv[]) {
    int total_size = 100, size = 25;
    int i=0,A[total_size],reciver[size],input=-1;
    int count=0;
    int abc; // define as global variable in every node
    int id,numprocs;
    int chunk_size;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&id);
    chunk_size = 100/ numprocs; // create chunk size for separate value to each node
    // สร้าง chunk size มาเพื่อแบ่งค่าลงไปในแต่ละ node
    if(id ==0) // rank 0 หรือเรียกว่า node 0
    { 
        srand(100);
        for (i=0; i < total_size; i++)
        {
            A[i] = rand()%1000; // Random values in Process 0 (Rank 0)
        }
        printf("Input 1-1000 ");
        fflush(stdout);
        scanf("%d", &input);
    }
    MPI_Bcast(&input, 1, MPI_INT, 0, MPI_COMM_WORLD); // Process 0 must Bcast to all process
    MPI_Scatter(A,total_size/numprocs,MPI_INT,&reciver,chunk_size,MPI_INT,0,MPI_COMM_WORLD); 
    // Use Scatter for separate data to all process

    for(i=0;i<total_size/numprocs;i++) // use for loop for increment value
    {
        if(A[i]<input) count++; // increment value's count 
    }
    // After calculate complete
    MPI_Reduce(&count,&abc,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD); // use reduce to calculate sum value
        /**
         * send,receive,send as 1 time, int type, using sum, root as 0
         * **/ 
   if (id == 0)
   {
       printf("Total number that less than %d is %d\n",input,abc);
   }
    MPI_Finalize();
}
```

### 3. Sum 2 arrays value into new array

Given arrays of integers A[100] and B[100], write an MPI program to construct C[100],  which each element C[i] = A[i] + B[i], using MPI_Scatter and MPI_Gather on 2 processes.

เราจะต้องสร้างอาเรย์ 3 อาเรย์ โดยให้อาเรย์แรกบวกค่ากับอาเรย์ที่สองและให้ผลลัพธ์ออกมาเป็นอาเรย์ที่ 3 โดยใช้ MPI_Scatter และ MPI_Gather สองโปรเซส

```C
#include <stdio.h> 
#include <stdlib.h> 
#include <mpi.h>

int main(int argc, char *argv[]) 
{ 
    int A[100];
    int local_A[25];
    int B[100];
    int local_B[25];
    int C[100];
    int local_C[25];
    int i;
    int chunk_size;
    int rank, size;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    chunk_size = 100 / size;
    if(rank == 0) // ถ้าโปรเซสตัวนี้เป็นตัวแรก หรือ โปรเซส 0
    { 
        srand(1234);
        for(i=0; i<100; i++)
        {
            A[i] = rand()%1000; // สุ่มค่าให้กับอาเรย์ตัวแรก
            B[i] = rand()%1000; // สุ่มค่าให้กับอาเรย์ตัวที่สอง
        }
    }

    MPI_Scatter(A, chunk_size, MPI_INT, local_A, chunk_size, MPI_INT, 0, MPI_COMM_WORLD); // ใช้ MPI_Scatter ในการกระจายข้อมูลให้กับโปรเซสตัวอื่น
    MPI_Scatter(B, chunk_size, MPI_INT, local_B, chunk_size, MPI_INT, 0, MPI_COMM_WORLD); // ใช้ MPI_Scatter ในการกระจายข้อมูลให้กับโปรเซสตัวอื่น

    printf("Rank %d, local_A & local_B = A & B[%d to %d]\n", rank, rank*chunk_size, (rank+1)*chunk_size);
    for(i=0; i<chunk_size; i++)
    {
        local_C[i] = local_A[i] + local_B[i];
    }

    MPI_Gather(local_C, chunk_size, MPI_INT, C, chunk_size, MPI_INT, 0,MPI_COMM_WORLD); // ใช้ MPI_Scatter ในการรวมงานจาก MPI_Scatter
    if(rank == 0) // ถ้าโปรเซสตัวนี้เป็นตัวที่ 0 หรือ โปรเซสตัวแรก
    {    		
		for(i=0;i<100;i++)
        {
            printf("%d ", C[i]);
        }
    }
    MPI_Finalize(); // จบการทำงานด้วย MPI
}
```

### 4. Calculate random value with array[8][8]

Given a matrix A[8][8] with some random values, write an MPI program to calculate the summation of all elements using only MPI collective communication on 8 processes. (MPI_Send/MPI_Recv are not allowed) 

โจทย์ให้เราสร้าง อาเรย์สองมิติ ที่มีการสุ่มค่าแล้วใช้ฟังก์ชั่นของ MPI ในการคิดค่าทั้งหมด และใช้ MPI_Collective หรือ MPI_Reduce ในการคิดผลรวมของอาเรย์ทั้งหมด(ใช้ 8 โปรเซสในการคำนวณ) โจทย์ไม่ให้เราใช้ MPI_Send และ MPI_Recv

```C
#include <stdio.h> 
#include <stdlib.h> 
#include <mpi.h>

int main(int argc, char *argv[]) { 
    int A[8][8];
    int i, j;
    int chunk_size;
    int rank, size;
    int local_sum = 0;
    int global_sum = 0;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    chunk_size = 8 / size; // กำหนด Chunk size เพื่อนำไปคำนวณใน MPI
    int local[8*chunk_size];

    if(rank == 0){
        srand(1234);
        for(i=0; i<8; i++){
            for(j=0; j<8; j++){
                A[i][j] = rand()%1000; // ใช้ for loop มาสุ่มค่าให้กับอาเรย์ที่เป็นอาเรย์ 2 มิติ
            }
        }
    }
    MPI_Scatter(A, 8*chunk_size, MPI_INT, local, 8*chunk_size, MPI_INT, 0, MPI_COMM_WORLD); // แบ่งข้อมูลไปให้แต่ละโปรเซสโดยที่ข้อมูลจะไม่ซ้ำกัน

    for(i=0; i<8*chunk_size; i++){
        local_sum = local_sum + local[i];
    }
    printf("Rank: %d, sum = %d\n", rank, local_sum);

    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD); // รวม data มาใว้ที่โปรเซสเดียวแล้วทำการคำนวนผลลัพธ์
    if(rank == 0){
		printf("Parallel total sum: %d\n", global_sum);	
	}
    MPI_Finalize();
}
```