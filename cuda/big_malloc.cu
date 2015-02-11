#include <stdlib.h>
#include <stdio.h>

int main(){
int *ptr = 0;
cudaError_t error = cudaMalloc((void**)&ptr, UINT_MAX);
if(error != cudaSuccess){
printf("CUDA error: %s\n", cudaGetErrorString(error));
exit(-1);
}
return 0;
}