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

  return 0;
}
