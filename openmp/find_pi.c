#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

//gcc -g -Wall -fopenmp -o find_pi find_pi.c


double serial_pi(int n){
  double factor = 1.0;
  double sum = 0.0;

  int k;
  for(k =0; k<n; k++){
    sum += factor/(2*k +1);
    factor = -factor;
  }

  return sum*4;
}





int main(){

  printf("Serial : %f\n", serial_pi(1000));

  double factor;
  double sum;

  sum = 0.0;
  int i;
# pragma omp parallel for reduction(+: sum) private(factor)
  for(i = 0; i < 1000; i++){
    factor = (i%2 == 0)? 1.0: -1.0;
    //printf("factor %f sum %f\n", factor,sum);
    sum += factor/(2*i + 1);
  }

 
 printf("parallel sum: %f\n", 4*sum);

 return 0;
}
