#include <stdio.h>
#include <string.h>
#include <mpi.h>

//compile:
//mpicc -o hello hello.c -std=gnu99

//run:
//mpirun -np 5 ./hello

const int MAX_STRING = 100;

#include <time.h>
float timediff(struct timespec t1, struct timespec t2){

  if(t1.tv_nsec > t2.tv_nsec){
    t2.tv_sec -= 1;
    t2.tv_nsec += 1000000000;
  }
  return t2.tv_sec - t1.tv_sec + 0.000000001 *(t2.tv_nsec - t1.tv_nsec);
}



int main(){
  char greeting[MAX_STRING];
  int comm_sz, my_rank;

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);


  struct timespec bgn, end;
  clock_gettime(CLOCK_REALTIME, &bgn);
  if(my_rank != 0){
    sprintf(greeting, "Hi from process %d of %d!", my_rank, comm_sz);
    MPI_Send(greeting, strlen(greeting) + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
  } else {
    printf("Yo! from process %d\n", my_rank);
    
    for(int i = 1; i< comm_sz; i++){
      MPI_Recv(greeting, MAX_STRING, MPI_CHAR, MPI_ANY_SOURCE, 0, 
	       MPI_COMM_WORLD, MPI_STATUS_IGNORE);  
      printf("%s\n", greeting);
    }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("time: %f\n", timediff(bgn, end));

  }

  MPI_Finalize();


  return 0;
}
