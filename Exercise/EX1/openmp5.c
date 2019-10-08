#include <omp.h> 
  
#include <stdio.h> 
#include <stdlib.h> 
  
int main(int argc, char* argv[]) 
{ 
  int a[100],i,max=0,sum=0,psum=0;
  srand(1234);
    // Beginning of parallel region 
    #pragma omp parallel default(none) private(psum,i) shared(sum,a) num_threads(3)
    {
        for (int i = 0; i < 100; i++)
        {
            a[i] = rand() %1000;
            psum += a[i];
            // a[i] = i;
            /* code */
        }
        // Go to the Critical sections like a critical region (Mutex)
        #pragma omp critical
        sum += psum;
    }
    printf("%d\n",sum);
    // Ending of parallel region 
} 