#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv){

  if(argc != 2){
    printf("Got to pass an argument for the size\n");
    exit(1);
  }

  int comm_sz, my_rank;

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  int N = atoi(argv[1]);
  int *array = malloc(N*sizeof(int));





  int i;
  for(i = 0; i<N; i++){
    array[i] = i+1;
  }


  int chunk_sz = N/comm_sz + 1;
  int left_over = N%chunk_sz;

  int sum = 0;
  if(my_rank != comm_sz-1){
    for (i = 0; i< chunk_sz; i++){
      sum += array[chunk_sz*my_rank + i];
    }
  } else {
    for(i = 0; i<left_over; i++){
      sum += array[chunk_sz*my_rank + i];
    }
  }


  MPI_Send(&sum, 1, MPI_INT, my_rank/2, 0, MPI_COMM_WORLD);

  if(my_rank < comm_sz/2){
    for(i=0; i<2; i++){
  
      int temp;
      MPI_Recv(&temp, 1, MPI_INT, 2*my_rank + i, 0, MPI_COMM_WORLD, NULL);
      printf("my_rank %d, i %d, temp %d\n", my_rank, i, temp);
    }
  }


  /*
  if(my_rank != 0)

    MPI_Send(&sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
  
  else {
    for(i = 1; i< comm_sz; i++){
      int temp;
      MPI_Recv(&temp, 1, MPI_INT, i, 0, MPI_COMM_WORLD, NULL);
      //printf("%d\n", temp);
      sum += temp;
      
    }
    printf("final sum = %d\n", sum);
  }
  */


  free(array);
  MPI_Finalize();

  return 0;
}


