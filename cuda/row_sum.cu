#include <stdio.h>
#include <stdlib.h>

__global__ void findleft(int *deviceMatrix, int *rowSum, int n){
int rownum = blockIdx.x;
int sum = 0;
int k;
for(k=0; k<n; k++)
	 sum += deviceMatrix[rownum*n+k];
rowSum[rownum] = sum;
}

int main(){
int n = 20;
int *hostMatrix, *deviceMatrix, *hostRowSum, *deviceRowSum;
int msize = n*n*sizeof(int);

hostMatrix = (int *) malloc(msize);
hostRowSum = (int *) malloc(n*sizeof(int));

cudaMalloc((void **) &deviceMatrix, msize);
cudaMalloc((void **) &deviceRowSum, msize/n);

int t = 0, i, j;
for(i = 0; i<n; i++){
for(j=0; j<n; j++)
	 hostMatrix[i*n+j] = t++;
}

cudaMemcpy(deviceMatrix, hostMatrix, msize, cudaMemcpyHostToDevice);

dim3 dimGrid(n,1);
dim3 dimBlock(1,1,1);

findleft<<<dimGrid, dimBlock>>>(deviceMatrix, deviceRowSum, n);

cudaThreadSynchronize();

cudaMemcpy(hostRowSum, deviceRowSum, msize/n, cudaMemcpyDeviceToHost);

for(i = 0; i<n; i++) printf("%d\n", hostRowSum[i]);
return 0;
}