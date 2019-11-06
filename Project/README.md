# Project Parallel (Quick sort via MPI)

## Due Date : Tuesday, November 5, 2019 at Midnight (11.55 PM) 

## Demo Date : Wednesday, November 6, 2019 from 1 PM

### โจทย์ต้องการอะไรในการทำ Sorting

1. Generate 1 million (1,000,000) random integer numbers ranging from 0 to 9999
2. To apply a parallel sorting algorithm, the number of compute nodes is varied from 1 to 16, if possible.
3. If the number on each node is below a threshold, say 1000 or 10000, you can call another sorting function to sort those numbers locally.
4. You have to measure the time used to do the sorting algorithm for each number of nodes varied. 
5. There will be a bonus point if your algorithm can handle 10 million numbers.

### สิ่งที่ต้องเตรียม

1. เน็ตแรงๆ (ไม่งั้นตอนต่อ SSH จะมีอาการกระตุก)
2. VPN Mahidol
3. Code Editing สักตัว (VSCODE, Atom ,etc.)
4. Virtual Box , VMWare (เอามารัน Ubuntu)
5. GCC และ MPI ไว้สำหรับคอมไพล์ Code 
6. สวดมนต์ไว้เยอะๆ

## Algorithm of QuickSort

![Algorithm](https://i.stack.imgur.com/Z7EIO.png)
<br>
หรือ
<br>
![2](https://image.slidesharecdn.com/quicksortdiscussionandanalysis-150830134127-lva1-app6891/95/quick-sort-algorithm-discussion-and-analysis-10-638.jpg?cb=1440942157)

อัลกอริทึ่มของ Quick Sort คือ
```c
/* low  --> Starting index,  high  --> Ending index */
quickSort(arr[], low, high)
{
    if (low < high)
    {
        /* pi is partitioning index, arr[pi] is now
           at right place */
        pi = partition(arr, low, high);

        quickSort(arr, low, pi - 1);  // Before pi
        quickSort(arr, pi + 1, high); // After pi
    }
}
```

การใช้ Partition เข้าช่วย
```c
/* This function takes last element as pivot, places
   the pivot element at its correct position in sorted
    array, and places all smaller (smaller than pivot)
   to left of pivot and all greater elements to right
   of pivot */
partition (arr[], low, high)
{
    // pivot (Element to be placed at right position)
    pivot = arr[high];  
 
    i = (low - 1)  // Index of smaller element

    for (j = low; j <= high- 1; j++)
    {
        // If current element is smaller than the pivot
        if (arr[j] < pivot)
        {
            i++;    // increment index of smaller element
            swap arr[i] and arr[j]
        }
    }
    swap arr[i + 1] and arr[high])
    return (i + 1)
}
```
นึกภาพตาม
```c
arr[] = {10, 80, 30, 90, 40, 50, 70}
Indexes:  0   1   2   3   4   5   6 

low = 0, high =  6, pivot = arr[h] = 70
Initialize index of smaller element, i = -1

Traverse elements from j = low to high-1
j = 0 : Since arr[j] <= pivot, do i++ and swap(arr[i], arr[j])
i = 0 
arr[] = {10, 80, 30, 90, 40, 50, 70} // No change as i and j 
                                     // are same

j = 1 : Since arr[j] > pivot, do nothing
// No change in i and arr[]

j = 2 : Since arr[j] <= pivot, do i++ and swap(arr[i], arr[j])
i = 1
arr[] = {10, 30, 80, 90, 40, 50, 70} // We swap 80 and 30 

j = 3 : Since arr[j] > pivot, do nothing
// No change in i and arr[]

j = 4 : Since arr[j] <= pivot, do i++ and swap(arr[i], arr[j])
i = 2
arr[] = {10, 30, 40, 90, 80, 50, 70} // 80 and 40 Swapped
j = 5 : Since arr[j] <= pivot, do i++ and swap arr[i] with arr[j] 
i = 3 
arr[] = {10, 30, 40, 50, 80, 90, 70} // 90 and 50 Swapped 

We come out of loop because j is now equal to high-1.
Finally we place pivot at correct position by swapping
arr[i+1] and arr[high] (or pivot) 
arr[] = {10, 30, 40, 50, 70, 90, 80} // 80 and 70 Swapped 

Now 70 is at its correct place. All elements smaller than
70 are before it and all elements greater than 70 are after
it.
```
![qcksort](https://lh6.googleusercontent.com/fk7AoKlReC13QN15YTWs8VZ-habZByAGbK8PIK3k15PkhsAB6sHGkSziUNQgHQ-7uINis6ecow8iQP7hkruxWaOdF2m1RL-pe_HEgEts7CDH_-LfsgqBNmpAIEl4QoqHDEAUtyg)<br>

![qcksort2](https://www.w3resource.com/w3r_images/quick-sort-part-1.png)



มาดูการเขียนโค้ดแบบ Sequence กันก่อน
```c
#include <stdio.h>
#include <stdlib.h>
#include "math.h"
#include <time.h>
static int N = 1000000;
void swap(int a, int b) 
{ 
    int t = a; 
    a = b; 
    b = t; 
} 
 
int Partitionqck (int a[], int low, int high)
{
	// Pick rightmost element as pivot from the array
	int pivot = a[high];

	// elements less than pivot will be pushed to the left of pIndex
	// elements more than pivot will be pushed to the right of pIndex
	// equal elements can go either way
	int pIndex = low;	
	
	// each time we finds an element less than or equal to pivot, pIndex
	// is incremented and that element would be placed before the pivot. 
	for (int i = low; i < high; i++)
	{
		if (a[i] <= pivot)
		{
			swap(a[i], a[pIndex]);
			pIndex++;
		}
	}
	// swap pIndex with Pivot
	swap (a[pIndex], a[high]);
	
	// return pIndex (index of pivot element)
	return pIndex;
}

void QuickSort(int a[], int low, int high)
{
	// base condition
	if(low >= high)
		return;

	// rearrange the elements across pivot
	int pivot = Partitionqck(a, low, high);

	// recur on sub-array containing elements that are less than pivot
	QuickSort(a, low, pivot - 1);

	// recur on sub-array containing elements that are more than pivot
	QuickSort(a, pivot + 1, high);
}

int main()
{
    // seed for random input
	clock_t begin, end;
	double time1 = 0.0, time2 = 0.0;
    int     i;
    double  t1,t2,t3;
    

    int *arr = (int *)malloc(sizeof(int) * N);
    if(arr==0)
        printf("Malloc memory error!");

    srand(1432427398);
    for (int i = 0; i < N; i++)
    {
        arr[i] = rand() % 10000;
    }
	// Perform non-optimized Quicksort on arr
	begin = clock();
	QuickSort(arr, 0, N-1);
	end = clock();

	// calculate time taken by Non-Optimized QuickSort
	time1 = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Average time taken by Seqential Quicksort: %f seconds\n",time1);
}
```

QuickSort นั้น Sort โดยการใช้ Pivot(Partition) เข้ามาช่วยในการจัดเรียงค่าโดยเลือกค่าที่สูงที่สุดมาทำ Pivot 

```c
void swap(int a, int b) 
{ 
    int t = a; 
    a = b; 
    b = t; 
} 
```

ฟังก์ชัน Swap นั้นทำหน้าที่สลับตำแหน่งของค่า โดยสลับจากค่าเก่าเป็นค่าใหม่

```c
int Partitionqck (int a[], int low, int high)
{
	// Pick rightmost element as pivot from the array
	int pivot = a[high];

	// elements less than pivot will be pushed to the left of pIndex
	// elements more than pivot will be pushed to the right of pIndex
	// equal elements can go either way
	int pIndex = low;
	
	// each time we finds an element less than or equal to pivot, pIndex
	// is incremented and that element would be placed before the pivot. 
	for (int i = low; i < high; i++)
	{
		if (a[i] <= pivot)
		{
			swap(a[i], a[pIndex]);
			pIndex++;
		}
	}
	// swap pIndex with Pivot
	swap (a[pIndex], a[high]);
	
	// return pIndex (index of pivot element)
	return pIndex;
}
```
ฟังก์ชั่น Partition หรือ Pivot นั้นจะเข้ามามีหน้าที่เลือกค่าที่มากที่สุดก่อน จากนั้น เมื่อค่าที่น้อยกว่า pivot จะโดนขยับไปทางซ้ายของค่าที่น้อยที่สุด และค่าที่มากสุดจะไปอยู่ทางขวาของ pivot ที่เราเลือก<br>
มาดูที่ for loop ถ้าค่าน้อยกว่าหรือเท่ากับ Pivot ที่เรากำหนด เราจะทำการสลับค่าจากตำแหน่งปัจจุบันไปไว้ก่อนหน้า Pivot(อยู่ทางซ้าย) ถ้าไม่ใช่ก็สลับค่าปกติ

```c
void QuickSort(int a[], int low, int high)
{
	// base condition
	if(low >= high)
		return;

	// rearrange the elements across pivot
	int pivot = Partitionqck(a, low, high);

	// recur on sub-array containing elements that are less than pivot
	QuickSort(a, low, pivot - 1);

	// recur on sub-array containing elements that are more than pivot
	QuickSort(a, pivot + 1, high);
}
```
ตามหลักปกติของการ **sort** เราจะเริ่ม **sort** โดยการเลือก **pivot** ก่อนแล้วใช้ฟังก์ชั่น Quicksort (แบบ **Recursive**) เข้ามาเรียงค่าใหม่

## Parallel Coding 

ในส่วนโค้ดของ **Parallel** นั้นเราได้ทำการเขียนฟังก์ชันขึ้นมาใหม่แต่ยังใช้อัลกอริทึ่มเดียวกันกับฝั่งชอง **Sequential** พูดง่ายๆคือ เราแบ่งโค้ดออกมาเป็นสองฝั่ง<br> โดยฝั่งของ Sequential ทำงานที่ **Master Process** หรือ **Rank 0** นั่นเอง<br>
ฝั่งของ **Parallel** จะยังทำงานตามปกติ โดยแบ่ง NP หรือ Number Processes ไปทำงานโดยการใช้ **MPI_Bcast** เพื่อช่วยในการกระจายงานให้เท่าๆกันทุก Process<br>
![bcast](https://image.slidesharecdn.com/slides2a-130504060132-phpapp01/95/open-mpi-2-4-638.jpg?cb=1367647332)<br>
![ex](https://computing.llnl.gov/tutorials/mpi/images/collective_comm.gif)<br>

จากตอนแรกที่เราได้ **Define** ค่าไว้ในโค้ด เราได้ทำการเปลี่ยนจากการ **Define** เป็นการใช้ **atoi(argv[])** แทนเพื่อเพิ่มความสามารถรับค่าจากการ **input** โดยใช้ **Terminal** <br>

**MPI** ที่ใช้นั้นแตกต่างจากปกติแต่ไม่ถึงขั้นแตกต่างกันมากและการคอมไพล์ไฟล์และการรันก็จะแตกต่างจากในสไลด์นิดหน่อย เพราะโค้ดที่ใช้ใน **Parallel** นั้น **For-loop Condition** นั้น ไม่ได้มีการใส่**ปีกกา** หรือ **bracket** ไว้ทำให้เราต้องคอมไพล์โค้ดโดยใช้แบบนี้

```bash
mpicc -o main main.c -std=c99 # ซึ่ง c99 คือ Flag Compiler บน Ubuntu OS 
mpicc -o main main.c -std=gnu99
# c99 stand for standard version C99
# old school C -
# int i;
# for (i=0; condition ; operation)
# 
# C99 - for (int i=0; condition ; operation)
```

### วิธีการรันโค้ด 

```bash
mpiexec -np <INT> main <INT> # หรือจะใช้ mpirun ก็ได้(ไม่แนะนำ)
mpiexec -np 8 main 100000 # เป็นต้น 
```

### ผลลัพธ์ที่ได้
```bash
Initial Quick Sort with MPI
---------- Our Group Member ----------
 Nutgamol M. 6088061
 Poonkasem K. 6088071
 Sunat P. 6088130
 Barameerak K. 6088156
---------- Our Group Member ----------
Initial Data with allocate memory 1000000 sizes
Process Used in Parallel : 1 and used time is : 0.376826 second(s)
Process Used in Parallel : 2 and used time is : 0.376490 second(s)
Process Used in Parallel : 3 and used time is : 0.376563 second(s)
Process Used in Parallel : 4 and used time is : 0.376715 second(s)
Process Used in Parallel : 5 and used time is : 0.376686 second(s)
Process Used in Parallel : 6 and used time is : 0.376705 second(s)
Process Used in Parallel : 7 and used time is : 0.376774 second(s)
Average time taken by Parallel Quicksort: 0.374151 seconds
Process Used in Sequential : 0
Average time taken by Sequential Quicksort: 0.710000 second(s)
Difference time is 0.335849 second(s) (In Parallel Quicksort is 1.90 time(s) faster than Sequential)
Quicksort with Sequential and Parallel Finished
Press Enter to Exit...
```


### Result

|Value|Time Used<br> Sequential <br> seconds|Time Used<br> Parrellel (-np 4)<br>seconds|Time Used<br> Parrellel (-np 8)<br> seconds|Time Used<br> Parrellel (-np 16)<br> seconds|
|:---:|:---:|:---:|:---:|:---:|
|10|0.000000|0.000167|0.000374|0.007246|
|100|0.000000|Segmentation fault (11)|0.000636|0.002492|
|1000|0.000000|Segmentation fault (11)|Segmentation fault (11)|0.008483|
|10000|0.000000|0.002655|0.001713|0.008657|
|100000|0.030000|0.023061|0.015439|0.064442|
|1000000|0.810000|0.322219|0.383939|0.524017|
|10000000|55.190000|12.533968|10.925307|10.517562|
|15000000|120.090000|22.963344|22.018736|33.711107|