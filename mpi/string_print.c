#include <stdio.h>
#include <stdlib.h>


#define NUM_MASSES 1024

int main(){

  float *y, *yold, *v;

  const float Ktension = 0.2;
  const float Kdamping = 0.9999;
  const float duration = 1.0;

  int size = NUM_MASSES*sizeof(float);
  y = (float *)malloc(size);
  yold = (float *)malloc(size);
  v = (float *)malloc(size);


  // initialize displacements (pluck it!) and velocities
  for (int i = 0; i < NUM_MASSES; i++ ) {
    v[i]  = 0.0f;
    yold[i] = y[i] = 0.0f;
    if (i == NUM_MASSES/2 )
      yold[i] = 1.0; // impulse at string center
  }

 
  // run simulation for desired period:
  int numIters = duration *20; // 44100; // 5 seconds


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
  }
}
