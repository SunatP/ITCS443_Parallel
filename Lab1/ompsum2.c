#include <stdio.h> 
#include <omp.h> 
int main() 
{    
    int i;    
    int sum = 0;    
    #pragma omp parallel for reduction(+:sum)    
    for (i=0; i <= 1000; i+=2)       
        sum = sum + i;    
    printf("%d\n",sum);
} 
