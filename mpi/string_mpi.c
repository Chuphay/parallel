#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int comm_sz, my_rank, chunk_sz;

void get_next(int *array){

  int i;
  int *my_array = malloc((chunk_sz+2)*sizeof(int));
  int *temp_array = malloc((chunk_sz+2)*sizeof(int));
  int buffer_low, buffer_high;
  
  if(my_rank == 0){
    for(i = 0; i<chunk_sz+1; i++)
      my_array[i] = array[i+my_rank*chunk_sz];

    for(i = 1; i<chunk_sz; i++){
      temp_array[i] =  my_array[i-1]+my_array[i]+my_array[i+1];
    }
    temp_array[0] = 0;
    //printf("last = %d\n", temp_array[chunk_sz-1]);
    MPI_Sendrecv(&temp_array[chunk_sz-1], 1, MPI_INT, my_rank+1, 0, &buffer_high, 1, MPI_INT, my_rank+1, 0, MPI_COMM_WORLD, NULL);
    //printf("low, I recieved %d\n", buffer_high);


    //my_array = temp_array;
    for(i = 0; i < chunk_sz; i++)
      my_array[i] = temp_array[i];
    my_array[chunk_sz] = buffer_high; 

    for(i = 0; i < chunk_sz+1; i++)
      printf("my_process %d my_array[%d] = %d\n",my_rank, i, my_array[i]);

  } else if(my_rank == comm_sz -1){

    for(i = 0; i<chunk_sz+1; i++)
      my_array[i] = array[i+my_rank*chunk_sz-1];

    for(i = 0; i<chunk_sz -1; i++){
      temp_array[i] =  my_array[i]+my_array[i+1]+my_array[i+2];
    }

    temp_array[chunk_sz-1] = 0;
    //my_array = temp_array;
    //printf("my_first = %d\n", temp_array[0]);
    MPI_Sendrecv(&temp_array[0], 1, MPI_INT, my_rank-1, 0, &buffer_low, 1, MPI_INT, my_rank-1, 0, MPI_COMM_WORLD, NULL);
    //printf("high, I recieved %d\n", buffer_low);

    for(i = 1; i < chunk_sz; i++)
      my_array[i] = temp_array[i-1];
    my_array[0] = buffer_low; 

    for(i = 0; i < chunk_sz+1; i++)
      printf("my_process %d my_array[%d] = %d\n",my_rank, i, my_array[i]);

  } else {

    for(i = 0; i<chunk_sz+2; i++)
      my_array[i] = array[i+my_rank*chunk_sz-1];

    for(i = 0; i<chunk_sz; i++){
      temp_array[i] =  my_array[i]+my_array[i+1]+my_array[i+2];    
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


    for(i = 0; i < chunk_sz+2; i++)
      printf("my_process %d my_array[%d] = %d\n",my_rank, i, my_array[i]); 
  }
  

  free(my_array);
  free(temp_array);

}


int main(){
 
  int i, N = 12;
  int *array = malloc(N*sizeof(int));
  for(i = 1; i<N-1; i++)
    array[i] = i;
  array[0] = array[N-1] = 0;



  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  chunk_sz = N/comm_sz;
  if(N%comm_sz != 0){
    printf("sorry... must divide correctly\n");
    exit(1);
  }
  
  if(my_rank == 0)  
    printf("chunk_sz: %d\n", chunk_sz);

  get_next(array);
  


  MPI_Finalize();
  free(array);
  return 0;
}
