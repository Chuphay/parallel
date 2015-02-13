#include <stdlib.h>
#include <stdio.h>

void square_this(int *array, int n){
  int i;
  for(i = 0 ; i<n ; i++)
    array[i] *= array[i];
}

__global__ cuda_square_this(int *deviceArray, int n){
  int i;
  for(i = 0; i<n; i++)
    deviceArray[i] *= deviceArray[i];
}


int main(){

  int *hostArray, *deviceArray;

  int n = 64;
  int arraySize = n*sizeof(int);
  hostArray = (int *) malloc(arraySize);
  cudaMalloc((void **) &deviceArray, arraySize);

  int i;
  for(i = 0; i<n ; i++)
    hostArray[i] = i+1;

  square_this(hostArray, n);

  for(i = 0; i<n; i++)
    printf("%d\n",hostArray[i]);
  return 0;
}
