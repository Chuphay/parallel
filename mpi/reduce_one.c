#include <stdio.h>
#include <mpi.h>

//couldn't get it to work

int main(){

  int size = 4;
  int array[8] = {1,2,3,4,5,6,7,8};
  int sum = 0;
  int my_rank, comm_sz;

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  printf("on\n");

  int a, b;
  for(size; size > 0; size>>=1){
    printf("size %d\n", size);
    if(my_rank<=size){
  
      if(size == 4){
	printf("inside here\n");
	a = array[my_rank];
	b = array[my_rank + size];
	printf("A: %d, b: %d\n", a, b);
      } else {
	MPI_Recv(&a, 1, MPI_INT, my_rank + 2, 0, 
		 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	b = sum;
      }
      printf("a: %d, b: %d\n", a, b);
      sum = a +b;
      printf("my_rank %d, my_sum %d\n", my_rank, sum);
      if(my_rank> size>>1){

	MPI_Send(&sum, 1, MPI_INT, my_rank/2 - 1, 0 ,  MPI_COMM_WORLD);
      } 

    }
  }


  MPI_Finalize();
  return 0;
}
