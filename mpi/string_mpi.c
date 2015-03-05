#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"


int main(){
 
  int i, N = 12;
  int *array = malloc(N*sizeof(int));
  for(i = 1; i<N-1; i++)
    array[i] = i;
  array[0] = array[N-1] = 0;

  int comm_sz, my_rank;

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  int chunk_sz = N/comm_sz;
  if(N%comm_sz != 0){
    printf("sorry... must divide correctly\n");
    exit(1);
  }
  
  if(my_rank == 0)  
    printf("chunk_sz: %d\n", chunk_sz);
  
  int *new_array = malloc(chunk_sz*sizeof(int));
  if(my_rank == 0){
    for(i = 1; i<chunk_sz; i++){
      printf("%d %d %d\n",my_rank, i, array[i+my_rank*chunk_sz-1]+array[i+my_rank*chunk_sz]+array[i+my_rank*chunk_sz+1]);
    }
  } else if(my_rank == comm_sz -1){
    for(i = 0; i<chunk_sz -1; i++){
      printf("%d %d %d\n",my_rank, i, array[i+my_rank*chunk_sz-1]+array[i+my_rank*chunk_sz]+array[i+my_rank*chunk_sz+1]);
    }
  } else {
    for(i = 0; i<chunk_sz; i++){
      printf("%d %d %d\n",my_rank, i,  array[i+my_rank*chunk_sz-1]+array[i+my_rank*chunk_sz]+array[i+my_rank*chunk_sz+1]);
    }
  }




  MPI_Finalize();
  free(array);
  return 0;
}
