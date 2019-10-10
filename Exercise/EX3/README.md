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

## อะไรคือ MPI_Scatter, MPI_Reduce MPI_Bcast

MPI_Scatter คือฟังก์ชันที่ทำงานโดยการกระจายข้อมูลจากโปรเซส root ไปยังโปรเซสทั้งหมด แต่ไม่เหมือนกับ MPI_Bcast ตรงที่ MPI_Bcast จะส่งข้อมูลทั้งหมดไปให้กับทุกโปรเซส แต่ MPI_Scatter จะแบ่งข้อมูลเป็นส่วนๆ และส่งแต่จะส่วนให้กับแต่ละโปรเซส

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

MPI_Bcast คือฟังก์ชันสำหรับกระจายข้อมูลจากโปรเซสหนึ่งไปยังโปรเซสที่เหลือทั้งหมด

```C
MPI_Bcast(
    vorank* data,             // &ตัวส่ง data
    int count,              // จำนวนข้อฒูลที่จะส่ง
    MPI_Datatype datatype,  // ชนิดข้อมูล
    int root,               // rank เริ่มต้น
    MPI_Comm communicator)  // ตัว communicator
```

### ข้อ 1. จริงๆแล้วนะ.

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
    MPI_Scatter(A,size,MPI_INT,&reciver,size,MPI_INT,0,MPI_COMM_WORLD); // กระจาย value ไปให้แต่ละฟังก์ชัน
    MPI_Reduce ( &reciver, &min, size, MPI_INT,MPI_MIN, 0,MPI_COMM_WORLD ); // ใช้ Reduce() หาค่าน้อยที่สุดจาก reciver
    MPI_Reduce ( &reciver, &max, size, MPI_INT,MPI_MAX, 0,MPI_COMM_WORLD ); // ใช้ Reduce() หาค่ามากที่สุดจาก reciver
    MPI_Reduce ( &reciver, &sum, size, MPI_INT,MPI_SUM, 0,MPI_COMM_WORLD ); // ใช้ Reduce() หาผลรวมจาก reciver
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