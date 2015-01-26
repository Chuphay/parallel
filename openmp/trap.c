#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

//gcc -g -Wall -fopenmp -o trap trap.c -lm


double f(double x){
  return exp(x);
}



void trap(){
  int i;
  for (i = 0; i < 2; i++){

    printf("%d \n", omp_get_thread_num());
  }
}

void Trap(double a, double b, int n, double * global_result_p){
  double h, x, my_result;
  double local_a, local_b;
  int i, local_n;
  int my_rank = omp_get_thread_num();
  int thread_count = omp_get_num_threads();

  printf("Big T thread count: %d\n", thread_count);

  h = (b-a)/n;
  local_n = n/thread_count;
  local_a = a + my_rank*local_n*h;
  local_b = local_a + local_n*h;

  my_result = (f(local_a) + f(local_b))/2.0;
  for(i = 0; i< local_n ; i++){
    x = local_a + i*h;
    my_result += f(x);
  }
  my_result *= h;

#pragma omp critical
  *global_result_p += my_result;
}
  



double serial_trap(int a,int b, int n){
  double h = (b-a)/((double) n);
  double approx = (f(a) + f(b))/2;
  int i;

  for (i = 1; i < n; i++){
   
    approx += f(a + i*h);
  }
  return h*approx;
}

double my_approx;

double my_trap(int a,int b, int n){
  //doesn't work
  double h = (b-a)/((double) n);
  my_approx = (f(a) + f(b))/2;
  int i;
#pragma omp for 
  for (i = 1; i < n; i++){
    //printf("%d \n", omp_get_thread_num()); 
#pragma omp critical
    my_approx += f(a + i*h);
  }
  return h*my_approx;
}



int main(){

#pragma omp parallel
  trap();

  
#pragma omp parallel
{
  printf("answer: %f\n", serial_trap(0,1,10)+1);

 }

 int i;
 int accuracy = 10;
 for (i=0;i<2;i++){
 printf("my answer: %f\n", my_trap(0,1,accuracy)+1);
 accuracy *= 100;
 }


 double global_result = 0.0;
 //double a,b ;
 int n = 10000;

#pragma omp parallel
 Trap(0, 1, n, &global_result);

 printf("Trap's answer: %f\n", global_result+1);




  return 0;
}
