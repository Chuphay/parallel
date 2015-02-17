#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>

#define NUM_MASSES 1024

__device__ void make_move(float *y, float *yold, float *v){

  int i = blockIdx.x*blockDim.x + threadIdx.x;
  float Ktension = 0.2;
  float Kdamping = 0.9999;

  if ( i > 0 && i < NUM_MASSES-1 ) {
    float accel = Ktension * (yold[i+1] + yold[i-1] - 2*yold[i]);
    //      add accel to velocity
    v[i] += accel;
    v[i] *= Kdamping;
    //      add velocity to position
    y[i] = yold[i] + v[i];
  }
}


__global__ void do_it(float *y, float *yold,float  *v,float *out, int numIters){

  for(int t = 0; t < numIters; t++){
    
    make_move(y,yold, v);
    __syncthreads();
    
    float *tmp = y;
    y = yold;
    yold = tmp;
    out[t] = y[NUM_MASSES/2];
  }
}

int main (){

  float *y, *device_y, *yold, *device_yold;
  float *v, *device_v; 
  float *cuda_out;
  
  const float Ktension = 0.2;
  const float Kdamping = 0.9999;
  const float duration = 1.0;

  int size = NUM_MASSES*sizeof(float);
  y = (float *)malloc(size);
  yold = (float *)malloc(size);
  v = (float *)malloc(size);
  cudaMalloc((void **) &device_y, size);
  cudaMalloc((void **) &device_yold, size);
  cudaMalloc((void **) &device_v, size);


  // initialize displacements (pluck it!) and velocities
  for (int i = 0; i < NUM_MASSES; i++ ) {
    v[i]  = 0.0f;
    yold[i] = y[i] = 0.0f;
    if (i == NUM_MASSES/2 )
      yold[i] = 1.0; // impulse at string center
  }

  cudaMemcpy(device_y, y, size, cudaMemcpyHostToDevice);
  cudaMemcpy(device_yold, yold, size, cudaMemcpyHostToDevice);
  cudaMemcpy(device_v, v, size, cudaMemcpyHostToDevice);

  // run simulation for desired period:
  int numIters = duration *20; // 44100; // 5 seconds
  cudaMalloc((void **) &cuda_out, numIters*sizeof(float));

  int num_blocks = 8;
 
  do_it<<<num_blocks, NUM_MASSES>>>(device_y, device_yold, device_v, cuda_out, numIters);
  cudaThreadSynchronize();

  float *host_out = (float *)malloc(numIters*sizeof(float));
    cudaMemcpy(host_out, cuda_out, numIters*sizeof(float), cudaMemcpyDeviceToHost);

  for ( int t = 0; t < numIters; t++ ) {
    // for each mass element
    float sum = 0;
    for ( int i = 0; i < NUM_MASSES; i++ ) {
      //   if boundary element
      //      handle boundary element
      if ( i == 0 || i == NUM_MASSES-1 ) {
      } else {
	//   else
	//      compute acceleration as scaled sum of differences with neighbors
	float accel = Ktension * (yold[i+1] + yold[i-1] - 2*yold[i]);
	//      add accel to velocity
	v[i] += accel;
	v[i] *= Kdamping;
	//      add velocity to position
	y[i] = yold[i] + v[i];
	sum += y[i];
      }
    }
    //   swap displacement buffers
    float *tmp = y;
    y = yold;
    yold = tmp;
    printf("y[NUM_MASSES/2] %f  %f\n",y[NUM_MASSES/2], host_out[t]);

    /*
    #if 1
    //   output a sample as sum of displacements
    fwrite ( &sum, sizeof(float), 1, f );
    #else
    // output a sample at pickup
    fwrite ( &y[PICKUP_POS], sizeof(float), 1, f );
    #endif
    */
  }
  // close output file
  //fclose ( f );


}
