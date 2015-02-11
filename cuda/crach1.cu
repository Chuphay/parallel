#include <stdio.h>
#include <stdlib.h>

__global__ void foo(int *ptr){
*ptr = 7;
}

int main(){
foo<<<1,1>>>(0);

cudaThreadSynchronize();

cudaError_t error = cudaGetLastError();
if(error != cudaSuccess){
printf("Cuda error: %s\n", cudaGetErrorString(error));
exit(-1);
}

return 0;
}