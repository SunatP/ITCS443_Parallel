#include <omp.h> 
  
#include <stdio.h> 
#include <stdlib.h> 
  
int main(int argc, char* argv[]) 
{ 
  int a[100],max;
  srand(1234);
    // Beginning of parallel region 
    #pragma omp parallel for
        for (int i = 0; i < 100; i++)
        {
            a[i] = rand() %1000;
            if(a[i]>=500)
            {
                printf("%d \n",a[i]);
            }
            /* code */
        }
    return 0;
    // Ending of parallel region 
} 