#include <stdlib.h>
#include <stdio.h>

int main(void){
int num_elements = 16;
int num_bytes = num_elements*sizeof(int);

int *device_array = 0;
int *host_array = 0;

host_array = (int *)malloc(num_bytes);

cudaMalloc((void**)&device_array, num_bytes);

cudaMemset(device_array, 0, num_bytes);

cudaMemcpy(host_array, device_array, num_bytes, cudaMemcpyDeviceToHost);

int i;
for(i  = 0; i<num_elements; i++)
printf("%d\n", host_array[i]);

free(host_array);

cudaFree(device_array);

return 0;
}