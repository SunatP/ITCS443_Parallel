#include <omp.h> 
  
#include <stdio.h> 
#include <stdlib.h> 
  
int main(int argc, char* argv[]) 
{ 
  int a[100],sum;
  srand(1234);
    // Beginning of parallel region 
    #pragma omp parallel for

        for (int i = 0; i < 100; i++)
        {
            a[i] = rand() %1000;
            // a[i] = i;
            /* code */
            sum += a[i]; // Reduction clause instead 
        }
        printf("%d\n",sum);
    return 0;
    // Ending of parallel region 
} 