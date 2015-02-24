#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

#define MY_MIN_MSG 0
#define OVERALL_MSG 1
#define COLLECT_MSG 2

int N, *not_done, comm_sz, my_rank, chunk, start_v, end_v;

unsigned largeint, my_min[2], other_min[2], overall_min[2], *one_hop_distance, *min_distance;

void init(int argc, char **argv){

  int i,j,tmp; unsigned u;
  N = atoi(argv[1]);
  printf("N : %d\n", N);
  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD,


}

int main(int argc, char **argv){
  init(argc, argv);
  return 0;
} 
