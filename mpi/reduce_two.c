#include <stdio.h>
#include <mpi.h>

//mpirun -np 5 ./reduce_two
//must be five processes, otherwise will not work properly

int main(){

  int comm_sz, my_rank;

  int array[10] = {1,2,3,4,5,6,7,8,9,10};
  int in[2], out;

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  in[0] = array[2*my_rank];
  in[1] = array[2*my_rank + 1];

  MPI_Reduce(&in, &out, 2, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  if(my_rank == 0){
    printf("final sum? = %d\n", out+array[0]+array[1]);
  }


  MPI_Finalize();
  return 0;
}
