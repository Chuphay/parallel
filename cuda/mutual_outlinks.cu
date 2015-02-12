#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>

//nvcc -o mutual_outlinks mutual_outlinks.cu -arch sm_20



//find mean number of mutual outlinks
//among all pairs of websites
//checking all (i,j) pairs
//thread k will handle all i such that
//i%totth = k, where totth is the number of threads

__global__ void procpairs(int *m, int *tot, int n){
int totth = gridDim.x * blockDim.x;
int me = blockIdx.x*blockDim.x + threadIdx.x;
int i,j,k,sum = 0;

for(i = me; i<n; i+= totth){
for(j = i+i; j<n; j++){
for(k= 0; k<n ; k++)
       sum += m[n*i+k]*m[n*j+k];
}
}
atomicAdd(tot, sum);
}

int main(){
int n = 10, nblk = 4;
int *hm, *dm, htot, *dtot;

int msize = n*n*sizeof(int);

hm = (int *) malloc(msize);

int i,j;
for( i = 0; i < n; i++){
hm[n*i + i] = 0;
for(j=0;j<n; j++)
	     if(j != i) hm[i*n +j] = rand()%2;
}

cudaMalloc((void **) &dm, msize);
cudaMemcpy(dm, hm, msize, cudaMemcpyHostToDevice);
htot = 0;
cudaMalloc((void **) &dtot, sizeof(int));
cudaMemcpy(dtot, &htot, sizeof(int), cudaMemcpyHostToDevice);

dim3 dimGrid(nblk, 1);
dim3 dimBlock(192, 1, 1);

procpairs<<<dimGrid, dimBlock>>>(dm, dtot, n);

cudaThreadSynchronize();

cudaMemcpy(&htot, dtot, sizeof(int), cudaMemcpyDeviceToHost);

return 0;
}
