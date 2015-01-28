#include <stdio.h>
#include <math.h>
#include <omp.h>


//serial program has:
//real	0m10.197s
//user	0m10.200s

//naive parallel has:
//real	0m5.524s
//user	0m12.320s

//parallel with scheduling:
//real	0m3.539s
//user	0m14.024s






double f(int i){

  int j, start = i*(i+1)/2, finish = start + i;
  double output = 0.0;

  for(j = start; j<=finish; j++){
    output += sin(j);
  }

  return output;
}

int main(){
  int i, n = 20000;
  double sum = 0.0;
#pragma omp parallel for reduction(+: sum) schedule(static, 1)
  for( i= 0; i< n; i++)
    sum += f(i);

  return 0;
}
