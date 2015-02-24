#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

#define MY_MIN_MSG 0
#define OVERALL_MSG 1
#define COLLECT_MSG 2

//Couldn't get it to work :(


int N, *not_done, comm_sz, my_rank, chunk, start_v, end_v;

unsigned largeint, my_min[2], other_min[2], overall_min[2], *one_hop_distance, *min_distance;

void init(int argc, char **argv){

  int i,j; unsigned u;
  N = atoi(argv[1]);
  // printf("N : %d\n", N);
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  chunk = N/comm_sz;
  start_v = my_rank*chunk;
  end_v = start_v + chunk-1;
  u = -1;
  largeint = u >> 1;
  one_hop_distance = malloc(N*N*sizeof(int));
  min_distance = malloc(N*sizeof(int));
  not_done = malloc(N*sizeof(int));

  srand(999);
  for(i = 0; i< N; i++){
    for(j = 0; j< N; j++){
      if(j == 1) one_hop_distance[i*N+j] = 0;
      else {
	one_hop_distance[i*N+j] = rand()%20;
	one_hop_distance[j*N+i] = one_hop_distance[i*N+j];
      }
    }
  }

  for(i = 0; i< N; i++){
    not_done[i] = 1;
    min_distance[i] = largeint;
  }
  min_distance[0] = 0;
}

void findmymin(){
  int i;
  my_min[0] = largeint;
  for(i = start_v; i <= end_v; i++){
    if(not_done[i] && min_distance[i] < my_min[0]){
      my_min[0] = min_distance[i];
      my_min[1]  = i;
    }
  }
}

void findoverallmin(){

  int i; 
  MPI_Status status;
  if(my_rank>0){
    MPI_Send(my_min, 2, MPI_INT, 0, MY_MIN_MSG, MPI_COMM_WORLD);
  } else {
    overall_min[0] = my_min[0];
    overall_min[1] = my_min[1];

    for(i = 1; i< comm_sz; i++){
      MPI_Recv(other_min, 2, MPI_INT, i, MY_MIN_MSG, MPI_COMM_WORLD, &status);
      if(other_min[0] < overall_min[0]){
	overall_min[0] = other_min[0];
	overall_min[1] = other_min[1];
      }
    }
  }
} 

void updatemymin_distance(){
  int i , min_v = overall_min[1];
  unsigned min_d = overall_min[0];

  for(i = start_v; i <= end_v; i++)
    if(min_d +one_hop_distance[min_v*N +i] <min_distance[i])
      min_distance[i] = min_d + one_hop_distance[min_v*N+i];
}

void disseminateoverallmin(){
  int i;
  if(my_rank == 0)
    for(i = 1; i< comm_sz; i++)
      MPI_Send(overall_min, 2, MPI_INT, i, OVERALL_MSG, MPI_COMM_WORLD);
    else
      MPI_Recv(overall_min,2,MPI_INT,0,OVERALL_MSG, MPI_COMM_WORLD, NULL);
  
}
void updateallmin_distance(){
  int i;
  if(my_rank>0)
    MPI_Send(min_distance+start_v, chunk, MPI_INT, 0, COLLECT_MSG, MPI_COMM_WORLD);
  else
    for(i = 0; i < comm_sz; i++)
      MPI_Recv(min_distance + i*chunk, chunk, MPI_INT, i, COLLECT_MSG, MPI_COMM_WORLD, NULL);
}

void printmd(){
  int i;
  printf("minimum distance:\n");
  for(i = 1; i< N; i++)
    printf("%u\n", min_distance[i]);
}

void dowork(){
  int step;

  for(step = 0; step<N; step++){
    findmymin();
    findoverallmin();
    disseminateoverallmin();
    not_done[overall_min[1]]=0;
    updatemymin_distance(start_v, end_v);
  }
  updateallmin_distance();
}





int main(int argc, char **argv){
  int i, j;
  init(argc, argv);
  dowork();
  if(my_rank == 0){
    printf("graph weights:\n");
    for(i = 0; i< N; i++){
      for(j = 0; j< N; j++){
	printf("%u ", one_hop_distance[i*N+j]);
      }
    }
    printmd();
  }
  MPI_Finalize();
  return 0;
} 
