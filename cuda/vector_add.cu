#include <stdlib.h>
#include <stdio.h>

__global__ void vector_add(const float *a, const float *b, float *c, const size_t n){
unsigned int i = threadIdx.x + blockDim.x*blockIdx.x;

if(i<n)
c[i] = a[i] + b[i];
}

int main(){

const int num_elements = 1<<20;

const int num_bytes = num_elements*sizeof(float);

  float *device_array_a = 0;
  float *device_array_b = 0;
  float *device_array_c = 0;
  float *host_array_a   = 0;
  float *host_array_b   = 0;
  float *host_array_c   = 0;

  // malloc the host arrays
  host_array_a = (float*)malloc(num_bytes);
  host_array_b = (float*)malloc(num_bytes);
host_array_c = (float*)malloc(num_bytes);
if((host_array_c == NULL)||(host_array_b == NULL)||(host_array_a == NULL)){
printf("couldnt allocate that much memory\n");
exit(1);
}

cudaMalloc((void**)&device_array_a, num_bytes);
cudaMalloc((void**)&device_array_b, num_bytes);
cudaMalloc((void**)&device_array_c, num_bytes);



if((device_array_c == NULL)||(device_array_b == NULL)||(device_array_a == NULL)){
printf("couldnt allocate that much memory for the GPU\n");
exit(1);
}

int i;
for(i = 0; i< num_elements; i++){
host_array_a[i]= (float)i;

host_array_b[i] = rand()/((float)RAND_MAX+1.0);
}

cudaMemcpy(device_array_a, host_array_a, num_bytes, cudaMemcpyHostToDevice);
cudaMemcpy(device_array_b, host_array_b, num_bytes, cudaMemcpyHostToDevice);

const size_t block_size = 256;
size_t grid_size = num_elements/block_size;

if(num_elements%block_size) ++grid_size;

vector_add<<<grid_size, block_size>>>(device_array_a, device_array_b, device_array_c, num_elements);

cudaMemcpy(host_array_c, device_array_c, num_bytes, cudaMemcpyDeviceToHost);

for(i = 0; i< 10; i++)
printf("result %d: %1.1f + %7.1f = %7.1f\n",i,host_array_a[i], host_array_b[i], host_array_c[i]);

free(host_array_a);
free(host_array_b);
free(host_array_c);

cudaFree(device_array_a);
cudaFree(device_array_b);
cudaFree(device_array_c);



return 0;
}