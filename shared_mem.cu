#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>

__global__ void switch(int *deviceArray){

//extern __shared__ int sharedArray[];
//sharedArray[0] = deviceArray[1];
//deviceArray[2] = deviceArray[1];
//deviceArray[1] = sharedArray[0];
}

int main(){

int *hostArray, *deviceArray, n = 20;
int size = n*sizeof(int);

hostArray = (int *) malloc(size);
cudaMalloc((void **) &deviceArray, size);
int i;
for(i = 0; i< n; i++)
      hostArray[i] = i;
cudaMemcpy(deviceArray, hostArray, size, cudaMemcpyHostToDevice);

dim3 dimGrid(1,1);
dim3 dimBlock(n,1,1);

switch <<<dimGrid, dimBlock, size>>> (deviceArray);

cudaMemcpy(hostArray, deviceArray, size, cudaMemcpyDeviceToHost);

for(i = 0; i<n; i++) printf("%d \n");

return 0;
}
