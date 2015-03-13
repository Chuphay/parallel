#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define NUM_MASSES 1024

int comm_sz, my_rank, chunk_sz;

void get_next(float *array, int num_iters){

  float *v = malloc(NUM_MASSES*sizeof(float));
  for (int i = 0; i < NUM_MASSES; i++ ) {
    v[i]  = 0.0f;
  }
  int i;

  //initialize my_array, which is the array for each process
  float *my_array = malloc((chunk_sz+2)*sizeof(float));
  if(my_rank == 0){
    for(i = 0; i<chunk_sz+1; i++)
      my_array[i] = array[i+my_rank*chunk_sz];
    
  } else if(my_rank == comm_sz -1){ 
    for(i = 0; i<chunk_sz+1; i++)
      my_array[i] = array[i+my_rank*chunk_sz-1];

  } else {
    for(i = 0; i<chunk_sz+2; i++)
      my_array[i] = array[i+my_rank*chunk_sz-1];
  }
  
  float Ktension = 0.2;
  float Kdamping = 0.9999;
  float *temp_array = malloc((chunk_sz+2)*sizeof(float));
  float buffer_low, buffer_high;

  //now we loop over the iterations
  for(int t = 0; t<num_iters; t++){

    //first rank 0
    if(my_rank == 0){
      for(i = 0; i<chunk_sz+1; i++){
	if(i+my_rank*chunk_sz == 512)
	  printf("my_rank %d %f\n", my_rank, my_array[i] );
    }
    for(i = 1; i<chunk_sz; i++){
      float accel = Ktension * (my_array[i+1] + my_array[i-1] - 2*my_array[i]);
      v[i] += accel;
      v[i] *= Kdamping;
      temp_array[i] = my_array[i] + v[i];
    }
    temp_array[0] = 0;
 
    MPI_Sendrecv(&temp_array[chunk_sz-1], 1, MPI_INT, my_rank+1, 0, &buffer_high, 1, MPI_INT, my_rank+1, 0, MPI_COMM_WORLD, NULL);

    //copy from temp_array back to my_array
    for(i = 0; i < chunk_sz; i++){
      my_array[i] = temp_array[i];
    }
    my_array[chunk_sz] = buffer_high; 

    //now we deal with the last process
    } else if(my_rank == comm_sz -1){

      for(i = 0; i<chunk_sz+1; i++){
	if(i+my_rank*chunk_sz-1 == 512)
	  printf("my_rank %d %f\n", my_rank, my_array[i] );	
      }

      for(i = 0; i<chunk_sz -1; i++){

      float accel = Ktension * (my_array[i+2] + my_array[i] - 2*my_array[i+1]);
      v[i] += accel;
      v[i] *= Kdamping;
      temp_array[i] = my_array[i+1] + v[i];
      }

      temp_array[chunk_sz-1] = 0;

      MPI_Sendrecv(&temp_array[0], 1, MPI_INT, my_rank-1, 0, &buffer_low, 1, MPI_INT, my_rank-1, 0, MPI_COMM_WORLD, NULL);

      for(i = 1; i < chunk_sz; i++)
	my_array[i] = temp_array[i-1];
      my_array[0] = buffer_low; 

      //finally we deal with all the processes in the middle
    } else {

      for(i = 0; i<chunk_sz+2; i++){
	if(i+my_rank*chunk_sz-1 == 512)
	  printf("my_rank %d %f\n", my_rank, my_array[i] );
      }

      for(i = 0; i<chunk_sz; i++){
	
	float accel = Ktension * (my_array[i] + my_array[i+2] - 2*my_array[i+1]); 
	v[i] += accel;
	v[i] *= Kdamping;

	temp_array[i] = my_array[i+1] + v[i];  
      }

      MPI_Sendrecv(&temp_array[0], 1, MPI_INT, my_rank-1, 0, &buffer_low, 1, MPI_INT, my_rank-1, 0, MPI_COMM_WORLD, NULL);

      MPI_Sendrecv(&temp_array[chunk_sz-1], 1, MPI_INT, my_rank+1, 0, &buffer_high, 1, MPI_INT, my_rank+1, 0, MPI_COMM_WORLD, NULL);

      for(i = 1; i < chunk_sz+1; i++)
	my_array[i] = temp_array[i-1];
      my_array[0] = buffer_low;
      my_array[chunk_sz+1] =buffer_high;

    }
  }
}


int main(){
 
  int i, N = NUM_MASSES;

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
 
  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  chunk_sz = NUM_MASSES/comm_sz;
  if(N%comm_sz != 0){
    printf("sorry... must divide correctly\n");
    exit(1);
  }

  get_next(yold,20);
  


  MPI_Finalize();

  return 0;
}
