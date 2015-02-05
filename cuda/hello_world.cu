#include "util/cuPrintf.cu"
#include <stdio.h>

__global__ void device_greetings(void){
cuPrintf("Hello world from the device!\n");
}

int main(void){
printf("Hello from the host!\n");

cudaPrintfInit();

device_greetings<<<2,3>>>();

cudaPrintfDisplay();

cudaPrintfEnd();

return 0;

}