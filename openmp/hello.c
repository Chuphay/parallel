//gcc -g -Wall -fopenmp -o omp_hello omp_hello.c


#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void hello(int i){
  int my_rank = omp_get_thread_num();
  int thread_count = omp_get_num_threads();
  printf("Hi from %d and %d of %d\n", i, my_rank, thread_count) ;
}

int main(){

# pragma omp parallel
{

  int i;
#pragma omp for
  for(i = 0; i < 20; i++){   
    hello(i);
  }
 }

  return 0;
}
