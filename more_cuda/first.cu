#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>

__global__ void device_function() {

}

int main(){
  int num_blocks = 1;
  int num_threads_per_block = 1;
  device_function<<<num_blocks,num_threads_per_block>>>();
  printf("called a function on the device\n");

  return 0;
}
