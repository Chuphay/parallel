#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

//./qsort 10

void swap(int *x, int *y){
  int temp = *x;
  *x = *y;
  *y = temp;
}

int separate(int *x, int low, int high){
  int i, pivot, last;
  pivot = x[low]; 
  swap(x+low, x+high);
  last = low;
  for(i = low; i<high; i++){
    if(x[i] <= pivot){
      swap(x+last, x+i);
      last += 1;
    }
  }
  swap(x+last, x+high);
  return last;
}

void qs(int *z, int zstart, int zend, int firstcall){
#pragma omp parallel
  {int part;
    if(firstcall == 1) {
#pragma omp single nowait
      qs(z,0,zend,0);
    } else {
      if(zstart<zend) {
	part = separate(z, zstart,zend);
#pragma omp task
	qs(z, zstart, part-1,0);
#pragma omp task
	qs(z,part+1,zend,0);
      }
    }
  }
}

int main(int argc, char **argv){
  int i,n,*w;
  n = atoi(argv[1]);
  w = malloc(n*sizeof(int));
  for(i = 0; i<n; i++) w[i] = rand()%100;
  qs(w,0,n-1,1);
  if(n<25)
    for(i=0;i<n;i++) printf("%d\n",w[i]);
  return 0;
} 
