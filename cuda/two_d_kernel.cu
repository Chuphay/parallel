#include <stdio.h>
#include <stdlib.h>

__global__ void kernel(int *array){

int index_x = blockIdx.x * blockDim.x + threadIdx.x;
int index_y = blockIdx.y * blockDim.y + threadIdx.y;

int grid_width = gridDim.x * blockDim.x;
int index = index_y * grid_width + index_x; 

int result = blockIdx.y * gridDim.x + blockIdx.x;

array[index] = result;
}

int main(){

int num_elements_x = 16;
int num_elements_y = 16;

int num_bytes = num_elements_x * num_elements_y * sizeof(int);

int *device_array = NULL;
int *host_array = 0;

host_array = (int *) malloc(num_bytes);
cudaMalloc((void **) &device_array, num_bytes);

dim3 block_size;

block_size.x = 4;
block_size.y = 4;

dim3 grid_size;
grid_size.x = num_elements_x / block_size.x;
grid_size.y = num_elements_y / block_size.y;

kernel<<<grid_size, block_size>>>(device_array);

cudaMemcpy(host_array, device_array, num_bytes, cudaMemcpyDeviceToHost);

int row, col;
for(row = 0; row <num_elements_y; ++row){
for(col = 0; col<num_elements_x; ++col)
printf("%2d ", host_array[row*num_elements_x + col]);

printf("\n");
}

free(host_array);
cudaFree(device_array);

return 0;
}