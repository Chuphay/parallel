#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>

__global__ void use_shared(float *array){

  int i, index = threadIdx.x;
  float average, sum = 0;

  __shared__ float sh_arr[128];

  sh_arr[index]  = array[index];

  __syncthreads();

  for(i = 0; i<=index; i++) sum += sh_arr[i];

  average = sum/(index + 1);

  array[index] = average;

}

int main(){

  float *h_array, *d_array;
  int n = 10;
  int size = n*sizeof(float);
  h_array = (float *) malloc(size);
  cudaMalloc((void **) &d_array, size);

  int i;
  for(i = 0; i< n ;i++) h_array[i] = i+1;
  for(i = 0; i< n ; i++) printf("%f\n",h_array[i]);

  cudaMemcpy(d_array, h_array, size, cudaMemcpyHostToDevice);
//  GpuTimer timer;
 // timer.Start();
  use_shared<<<1,n>>>(d_array);
 // timer.Stop();

  cudaMemcpy(h_array, d_array, size, cudaMemcpyDeviceToHost);

  for(i = 0; i<n ; i++) printf("%f\n",h_array[i]);
 // printf("Elapsed: %f\n", timer.Elapsed());
  return 0;

}

