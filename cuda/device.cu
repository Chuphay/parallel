#include <stdlib.h>
#include <stdio.h>

__device__ int get_global_index(){
return blockIdx.x * blockDim.x + threadIdx.x;
}
__device__ int get_constant(){
return 7;
}
__global__ void kernel1(int *array){
int index = get_global_index();
array[index] = get_constant();
}
__global__ void kernel2(int *array){
int index = get_global_index();
array[index] = get_global_index();
}

int main(){
int num_elements = 256;
int num_bytes = num_elements*sizeof(int);

int *device_array = 0;
int *host_array = 0;

host_array = (int *) malloc(num_bytes);
cudaMalloc((void**)&device_array, num_bytes);

int block_size = 128;
int grid_size = num_elements/block_size;

kernel1<<<grid_size, block_size>>>(device_array);
cudaMemcpy(host_array, device_array, num_bytes, cudaMemcpyDeviceToHost);

printf("kernel 1 results: \n");
int i;
for(i=0;i<num_elements;i++){
printf("%d ", host_array[i]);
}
printf("\n");

kernel2<<<grid_size, block_size>>>(device_array);
cudaMemcpy(host_array, device_array, num_bytes, cudaMemcpyDeviceToHost);

printf("kernel 2 results: \n");
for(i= 0; i< num_elements; i++){
printf("%d ", host_array[i]);
}
printf("\n");
return 0;
}