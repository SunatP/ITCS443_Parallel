#include <stdio.h>
#include <mpi.h>
#include <time.h>
#include <stdlib.h>

void merge(int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    /* create temp arrays */
    int L[n1], R[n2];

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    /* Copy the remaining elements of L[], if there
    are any */
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there
    are any */
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l + (r - l) / 2;

        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        merge(arr, l, m, r);
    }
}

void RandomNum(int *arr, int lower, int upper, int count)
{
    int i;
    for (i = 0; i < count; i++) 
    {
        int num = (rand() % (upper - lower + 1)) + lower;
        arr[i] = num;
    }
}

int main(int argc, char *argv[])
{
    int rank, size;
    int interval;
    int number, start, end;
    int proc;
    // Implement lower, upper, count ตามที่ต้องการ
    int lower = 0, upper = 9999, count = 20;
    int data[count];
    int counter = 0;
    int i;
    int *local_bucket;
    int *proc_count;
    int *displs;
    int sorted_num[count];
    MPI_Status status;
    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );

    // สุ่มตัวเลขตาม function ที่ไปก็อปมา
    if (rank == 0) {
        srand(time(0));
        RandomNum(data, lower, upper, count);
        printf("--> Data: ");
        for (i = 0; i < count; i++) 
        {
            printf("%d ", data[i]);
        }
        printf("\n");
    }
    // Broadcast ให้เหล่าทาสในเรือนเบี้ยรวมถึงตัวเอง (rank 0)
    MPI_Bcast(data, count, MPI_INT, 0, MPI_COMM_WORLD);

    // คำนวณ range ของแต่ละ interval (ก็อปใน lab มา)
    interval = (upper - lower + 1)/(size);
    start = (rank)*interval + lower;
    end = start + interval - 1;
    if (rank == (size - 1)) {
        end = upper;
    }

    // เอาไว้เช็ค interval
    printf("-->Rank %d: start %d, end %d\n", rank, start, end);
    
    // เอา data ตัวเลขที่สุ่มมาเช็คตาม interval แล้วคำนวณ size (counter) ของแต่ละ interval
    for (i = 0; i < count; i++) 
    {
        if(data[i] >= start && data[i] <= end) {
            counter += 1;
        }
    }

    // สร้างถังเก็บขึ้นมา แล้วให้มีพื้นที่เก็บตาม size ที่คำนวณไว้
    local_bucket = (int *)malloc(counter*sizeof(int));

    // ให้ counter = 0 อีกรอบ แล้วยัด data ลงไปตามช่วง + คำนวณ counter ใหม่
    counter = 0;
    for (i = 0; i < count; i++) 
    {
        if(data[i] >= start && data[i] <= end) {
            local_bucket[counter] = data[i];
            counter += 1;
        }
    }
    
    // เอาไว้เช็ค
    for (i = 0; i < counter; i++) 
    {
        printf("%d ", local_bucket[i]);
    }
    printf("\n");

    // เอาเลขในถังมา sort ด้วย function mergeSort (ที่ก็อปมาอะเกน)
    mergeSort(local_bucket, 0, counter - 1);

    // กำหนดขนาดของ pointer สำหรับใช้ในการ gather กลับไปให้ rank 0 (MPI_Gatherv)
    // MPI_Gatherv ใช้สำหรับ gather array ที่มี size ต่างกันในแต่ละ rank
    if (rank == 0) {
        proc_count = (int *)malloc(size*sizeof(int));
        displs = (int *)malloc(size*sizeof(int));
    }

    // Gatherv ต้องใช้ array ของ size ในแต่ละ rank 
    // ก็เลยใช้ MPI_Gather ส่ง size ในแต่ละ rank ให้เก็บไว้ใน proc_count แบบเรียงตาม rank 
    MPI_Gather(&counter, 1, MPI_INT, proc_count, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // คำนวณ displs (อันนี้ไม่รู้ว่าคืออะไร แต่รู้ว่ามันต้องใช้ใน Gatherv)
    if (rank == 0) {
        displs[0] = 0;
        for (i = 0; i < size-1; i++) 
        {
           displs[i+1] = displs[i] + proc_count[i];
        }
    }

    // Variable ครบก็โยนโลด
    // data ที่ sort แล้วก็จะอยู่ใน sorted_num ที่เป็น array
    MPI_Gatherv(local_bucket, counter, MPI_INT, sorted_num, proc_count, displs, MPI_INT, 0, MPI_COMM_WORLD);

    // printf ออกมาเช็คคำตอบ
    if (rank == 0) {
        printf("--> Sorted: ");
        for (i = 0; i < count; i++) 
        {
            printf("%d ", sorted_num[i]);
        }
        printf("\n");
    }
    MPI_Finalize();
}
