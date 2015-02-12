#include <stdio.h>
#include <stdlib.h>


//uses shared memory, in a single block
//limited to n<= 4000 if only 16K shared memory

__device__ void initsp( int *sprimes, int n, int nth, int me){

int chunk, startsetsp, endsetsp, val, i;

sprimes[2] = 1;

chunk = (n-1)/ nth;
startsetsp = 2 +me*chunk;

if(me<nth -1) endsetsp = startsetsp + chunk-1;
else endsetsp = n;

val = startsetsp%2;
for(i = startsetsp; i<=endsetsp; i++){
sprimes[i] = val;
val = 1 - val;
}

__syncthreads();
}

__device__ void cpytoglb(int *dprimes, int *sprimes, int n, int nth, int me){
int startcpy, endcpy, chunk, i;
chunk = (n-1)/nth;
startcpy = 2 + me*chunk;
if(me<nth-1) endcpy = startcpy + chunk -1;
else endcpy = n;

for(i = startcpy; i<= endcpy; i++) dprimes[i] = sprimes[i];

__syncthreads();
}

__global__ void sieve(int *dprimes, int n, int nth){

extern __shared__ int sprimes[];
int me = threadIdx.x;
int nthl = nth -1;

initsp(sprimes, n ,nth, me);

int maxmult, m, startmult, endmult, chunk, i;
for(m = 3; m*m <= n; m++){
if(sprimes != 0){
maxmult = n/m;
chunk = (maxmult -1)/nth;
startmult = 2 + me*chunk;
if(me<nthl) endmult = startmult + chunk -1;
else endmult = maxmult;
}
for(i = startmult; i <= endmult; i++) sprimes[i*m] = 0;
}
__syncthreads();
cpytoglb(dprimes,sprimes,n,nth,me);
}

int main(){
printf("start\n");
int n = 10, nth = 16;

int *hprimes, *dprimes;
int psize = (n+1)*sizeof(int);

hprimes = (int *) malloc(psize);
printf("before cuda Malloc\n");

cudaMalloc((void **) &dprimes, psize);
printf("after cudamalloc\n");

dim3 dimGrid(1,1);
dim3 dimBlock(nth, 1, 1);

sieve<<<dimGrid, dimBlock, psize>>>(dprimes, n ,nth);

cudaError_t err = cudaGetLastError();
if(err != cudaSuccess) printf("fail\n");
printf("apparently enough memory\n");

cudaThreadSynchronize();

cudaMemcpy(hprimes, dprimes, psize, cudaMemcpyDeviceToHost);


int i;
for(i = 2; i<=n; i++){
      if(hprimes[i] == 1){ 
      		    printf("%d\n", i);
		    }
}

return 0;
}