#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

// mpirun -np 4 ./zero_zero 10


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
      if(i%2 == 0)
	array[i] = i+1;
      else
	array[i] =0;
      if(my_rank == 0) printf("%d ",array[i]);
    }
    if(my_rank == 0) printf("\n");

  int chunk_sz = N/comm_sz + 1;
  int left_over = N%chunk_sz;
  int non_zero[chunk_sz];

  int sum = 0;
  if(my_rank != comm_sz-1){
    for (i = 0; i< chunk_sz; i++){
      if(array[chunk_sz*my_rank + i] == 0){
	//pass
      } else {
	non_zero[sum] = array[chunk_sz*my_rank + i];
	sum++;
      }
    }
  } else {
    for(i = 0; i<left_over; i++){
      if(array[chunk_sz*my_rank + i] == 0){
	//pass
      } else {
	non_zero[sum] = array[chunk_sz*my_rank + i];
	sum++;
      }
    }
  }


  MPI_Send(&sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
  MPI_Send(non_zero, sum, MPI_INT, 0, 1, MPI_COMM_WORLD);
  if(my_rank == 0){
    for(i=0; i< comm_sz; i++){
  
      int sum;

      MPI_Recv(&sum, 1, MPI_INT, i, 0, MPI_COMM_WORLD, NULL);
      MPI_Recv(&non_zero, chunk_sz , MPI_INT, i, 1, MPI_COMM_WORLD, NULL);
      //      printf("my_rank %d, i %d, sum %d\n", my_rank, i, sum);
      int j;
      for(j=0;j<sum;j++){
	printf("%d ",non_zero[j]);
      }
    }
    printf("\n");
  }

  free(array);
  MPI_Finalize();

  return 0;
}

