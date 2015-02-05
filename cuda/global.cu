#include <stdlib.h>
#include <stdio.h>

__global__ void kernel(int *array){

int index = blockIdx.x*blockDim.x + threadIdx.x;

array[index] = index;
}

int main(){
int num_elements = 256;
int num_bytes = num_elements*sizeof(int);

int *device_array = 0;
int *host_array = 0;

host_array = (int *)malloc(num_bytes);

cudaMalloc((void **) &device_array, num_bytes);

int block_size = 128;
int grid_size = num_elements/block_size;

kernel<<<grid_size, block_size>>>(device_array);

cudaMemcpy(host_array, device_array, num_bytes, cudaMemcpyDeviceToHost);

int i;
for(i=0;i<num_elements; ++i)
printf("%d\n", host_array[i]);

free(host_array);
cudaFree(device_array);

return 0;
} 