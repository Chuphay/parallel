#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define NUM_MASSES 1024
void serial(float *y, float *yold, float *v, int numIters, float *out){
  float Ktension = 0.2;
  float Kdamping = 0.9999;


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
    out[t] = y[NUM_MASSES/2];
    printf("t = %d out = %f\n",t,y[NUM_MASSES/2]);
  }
}

int comm_sz, my_rank, chunk_sz;

void get_next(float *array, int num_iters){
  //printf("my_rank*chunk_sz = %d\n", my_rank*chunk_sz);
  float *v = malloc(NUM_MASSES*sizeof(float));
  for (int i = 0; i < NUM_MASSES; i++ ) {
    v[i]  = 0.0f;
  }
  int i;
  float *my_array = malloc((chunk_sz+2)*sizeof(float));
 if(my_rank == 0){
    for(i = 0; i<chunk_sz+1; i++){

      my_array[i] = array[i+my_rank*chunk_sz];

    }
 } else if(my_rank == comm_sz -1){

    for(i = 0; i<chunk_sz+1; i++){
      my_array[i] = array[i+my_rank*chunk_sz-1];

    }
 }else {

    for(i = 0; i<chunk_sz+2; i++){
      my_array[i] = array[i+my_rank*chunk_sz-1];

    }
 }
  float Ktension = 0.2;
  float Kdamping = 0.9999;
  //int i;

  float *temp_array = malloc((chunk_sz+2)*sizeof(float));
  float buffer_low, buffer_high;
  for(int t = 0; t<num_iters; t++){
  if(my_rank == 0){
    for(i = 0; i<chunk_sz+1; i++){

      //     my_array[i] = array[i+my_rank*chunk_sz];
     
      if(i+my_rank*chunk_sz == 512){
	printf("my_rank %d %f\n", my_rank, my_array[i] );
      }

    }
    for(i = 1; i<chunk_sz; i++){
      float accel = Ktension * (my_array[i+1] + my_array[i-1] - 2*my_array[i]);
	v[i] += accel;
	v[i] *= Kdamping;
	//      add velocity to position
	temp_array[i] = my_array[i] + v[i];

	//temp_array[i] =  my_array[i-1]+my_array[i]+my_array[i+1];
    }
    temp_array[0] = 0;
    //printf("last = %d\n", temp_array[chunk_sz-1]);
    MPI_Sendrecv(&temp_array[chunk_sz-1], 1, MPI_INT, my_rank+1, 0, &buffer_high, 1, MPI_INT, my_rank+1, 0, MPI_COMM_WORLD, NULL);
    //printf("low, I recieved %d\n", buffer_high);


    //my_array = temp_array;
    for(i = 0; i < chunk_sz; i++){
      my_array[i] = temp_array[i];
    }
    my_array[chunk_sz] = buffer_high; 

    //for(i = 0; i < chunk_sz+1; i++)
    //  printf("my_process %d my_array[%d] = %d\n",my_rank, i, my_array[i]);

  } else if(my_rank == comm_sz -1){

    for(i = 0; i<chunk_sz+1; i++){
      // my_array[i] = array[i+my_rank*chunk_sz-1];
    if(i+my_rank*chunk_sz-1 == 512){
	printf("my_rank %d %f\n", my_rank, my_array[i] );
      }
    }

    for(i = 0; i<chunk_sz -1; i++){
      //temp_array[i] =  my_array[i]+my_array[i+1]+my_array[i+2];
      float accel = Ktension * (my_array[i+2] + my_array[i] - 2*my_array[i+1]);
	v[i] += accel;
	v[i] *= Kdamping;
	//      add velocity to position
	temp_array[i] = my_array[i+1] + v[i];
    }

    temp_array[chunk_sz-1] = 0;
    //my_array = temp_array;
    //printf("my_first = %d\n", temp_array[0]);
    MPI_Sendrecv(&temp_array[0], 1, MPI_INT, my_rank-1, 0, &buffer_low, 1, MPI_INT, my_rank-1, 0, MPI_COMM_WORLD, NULL);
    //printf("high, I recieved %d\n", buffer_low);

    for(i = 1; i < chunk_sz; i++)
      my_array[i] = temp_array[i-1];
    my_array[0] = buffer_low; 

    //for(i = 0; i < chunk_sz+1; i++)
    // printf("my_process %d my_array[%d] = %d\n",my_rank, i, my_array[i]);

  } else {

    for(i = 0; i<chunk_sz+2; i++){
      // my_array[i] = array[i+my_rank*chunk_sz-1];
    if(i+my_rank*chunk_sz-1 == 512){
	printf("my_rank %d %f\n", my_rank, my_array[i] );
      }
    }

    for(i = 0; i<chunk_sz; i++){
      //temp_array[i] =  my_array[i]+my_array[i+1]+my_array[i+2]; 
      float accel = Ktension * (my_array[i] + my_array[i+2] - 2*my_array[i+1]); 
	v[i] += accel;
	v[i] *= Kdamping;
	//      add velocity to position
	temp_array[i] = my_array[i+1] + v[i];  
    }


    //printf("I'm in the middle, my low is %d, and my high is %d\n", temp_array[0], temp_array[chunk_sz-1]);

    MPI_Sendrecv(&temp_array[0], 1, MPI_INT, my_rank-1, 0, &buffer_low, 1, MPI_INT, my_rank-1, 0, MPI_COMM_WORLD, NULL);
    //printf("low, I recieved %d\n", buffer_low);
    MPI_Sendrecv(&temp_array[chunk_sz-1], 1, MPI_INT, my_rank+1, 0, &buffer_high, 1, MPI_INT, my_rank+1, 0, MPI_COMM_WORLD, NULL);
    //printf("high, I recieved %d\n", buffer_high);
    //my_array = temp_array;
    for(i = 1; i < chunk_sz+1; i++)
      my_array[i] = temp_array[i-1];
    my_array[0] = buffer_low;
    my_array[chunk_sz+1] =buffer_high;


    //for(i = 0; i < chunk_sz+2; i++)
      //printf("my_process %d my_array[%d] = %d\n",my_rank, i, my_array[i]); 
  }
  }

  free(my_array);
  free(temp_array);

}


int main(){
 
  int i, N = 1024;
  int *array = malloc(N*sizeof(int));
  for(i = 1; i<N-1; i++)
    array[i] = i;
  array[0] = array[N-1] = 0;

  int size = NUM_MASSES*sizeof(float);
  float *y = (float *)malloc(size);
  float *yold = (float *)malloc(size);
  float *v = (float *)malloc(size);

  for (int i = 0; i < NUM_MASSES; i++ ) {
    v[i]  = 0.0f;
    yold[i] = y[i] = 0.0f;
    if (i == NUM_MASSES/2 )
      yold[i] = 1.0; // impulse at string center
  }
  int duration = 1;
  int numIters = duration *20; //44100; // 5 seconds
  float *out = (float *)malloc(numIters*sizeof(float)); 
  //serial(y, yold, v,numIters, out);



  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  chunk_sz = NUM_MASSES/comm_sz;
  if(N%comm_sz != 0){
    printf("sorry... must divide correctly\n");
    exit(1);
  }
  
  if(my_rank == 0){  
    printf("chunk_sz: %d\n", chunk_sz);
    //serial(y, yold, v,numIters, out);
  } 
  get_next(yold,20);
  


  MPI_Finalize();
  free(array);
  return 0;
}
