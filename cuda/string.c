//#include <iostream>
//#include <vector>
#include <stdio.h>
#include <stdlib.h>

#define NUM_MASSES 1024

__global__ void do_it(float *y, float *yold,float  *v,float *out, int numIters){

  int i = threadIdx.x;


  for ( int t = 0; t < numIters; t++ ) {
    // for each mass element
    //float sum = 0;
    out[t] = y[NUM_MASSES/2];
      if ( i > 0 && i < NUM_MASSES-1 ) {
s
	float accel = Ktension * (yold[i+1] + yold[i-1] - 2*yold[i]);
	//      add accel to velocity
	v[i] += accel;
	v[i] *= Kdamping;
	//      add velocity to position
	y[i] = yold[i] + v[i];
	//sum += y[i];
      }
    }

}

int main ( int argc, char **argv )
{

  float *y, *device_y, *yold, *device_yold;
  float *v, device_v; // *vold, device_vold;
  float *cuda_out;
  //const int NUM_MASSES = 1000;
    const float Ktension = 0.2;
    const float Kdamping = 0.9999;
    const float duration = 1.0;

    //const int PICKUP_POS = NUM_MASSES / 7;    // can place pickup anywhere

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

  do_it<<<1, NUM_MASSES>>>(device_y, device_yold, device_v, cuda_out, numIters);


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
    printf("y[NUM_MASSES/2] %f\n",y[NUM_MASSES/2]);

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
