#include <stdlib.h>
#include <stdio.h>

void square_this(int *array, int n){
  int i;
  for(i = 0 ; i<n ; i++)
    array[i] *= array[i];
}

__global__ void cuda_square_this(int *deviceArray){
  int me =threadIdx.x;
  
  deviceArray[me] *= 2;
}


int main(){

  int *hostArray, *deviceArray;

  int n = 10;
  int arraySize = n*sizeof(int);
  hostArray = (int *) malloc(arraySize);
  cudaMalloc((void **) &deviceArray, arraySize);

  int i;
  for(i = 0; i<n ; i++)
    hostArray[i] = i+1;

  cudaMemcpy(deviceArray, hostArray, arraySize, cudaMemcpyHostToDevice);
  cuda_square_this<<<1,n>>>(deviceArray);


  square_this(hostArray, n);

  for(i = 0; i<n; i++)
    printf("%d\n",hostArray[i]);

  cudaMemcpy(hostArray, deviceArray, arraySize, cudaMemcpyDeviceToHost);

for(i=0;i<n;i++)
printf("%d\n", hostArray[i]);

  return 0;
}
