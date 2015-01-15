#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mpi.h>
#include <unistd.h>

const int MAX_STRING = 100;

int main(void){
  char greeting[MAX_STRING];
  int comm_sz; //number of processes
  int my_rank;
  char *cpu_name = malloc(80*sizeof(char));
  gethostname(cpu_name, 80);

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  if(my_rank != 0){
    sprintf(greeting, "Greeting from process %d on machine %s of %d!", my_rank, cpu_name,  comm_sz);
    MPI_Send(greeting, strlen(greeting)+1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
  } else {
    printf("Greeting from process 0 on machine %s\n",cpu_name);
    int q;
    for(q = 1; q < comm_sz; q++){
      MPI_Recv(greeting, MAX_STRING, MPI_CHAR, q, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf("%s\n", greeting);
    }
  }
  MPI_Finalize();

  return 0;
}
