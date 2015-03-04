#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

//Not working....

int compare(const void *a_p, const void  *b_p){
   int a = *((int*)a_p);
   int b = *((int*)b_p);

   if (a < b)
      return -1;
   else if (a == b)
      return 0;
   else
      return 1;
}

void even(int *a, int n){
  if(n>1){
    int i;
    for(i = 0; i< n/2 ; i++){
      if(compare((const void *) &a[2*i+1], (const void *) &a[2*i]) == -1){
	//printf("i = %d\n",i);
	int tmp = a[2*i];
	a[2*i] = a[2*i+1];
	a[2*i+1] = tmp;
      }
    }
  }
}
void odd(int *a, int n){
  if(n>1){
    int i;
    for(i = 1; i< n/2; i++){
      if(compare((const void *) &a[2*i], (const void *) &a[2*i-1]) == -1){
	//printf("i = %d\n",i);
	int tmp = a[2*i-1];
	a[2*i-1] = a[2*i];
	a[2*i] = tmp;
      }
    }
  }
}

void random_nums(int *a, int n){
  int i;
  for(i = 0; i<n; i++)
    a[i] = rand()%100;
}
 
int main(int argc, char **argv){
  if(argc != 3){
    printf("usage: .\brick_sort n seed\n");
    exit(1);
  }
  srand(atoi(argv[2]));
  int N = atoi(argv[1]);
  int *b = malloc(N*sizeof(int));
  int *a = malloc(N*sizeof(int));
  random_nums(b, N);
  int i;
  for(i =0; i< N; i++)
    a[i] = b[i];


  int comm_sz, my_rank;

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  if(my_rank == 0){
    for(i = 0; i< N; i++)
      printf("%d ", b[i]);
    printf("\n");
  }
  int chunk_sz = N/(comm_sz-1);
  int left_over = N%(comm_sz-1); 
  if(left_over != 0){
    printf("sorry. try something like 30 numbers with 3 + 1 processes\n");
    printf("like this should work: mpirun -np 4 ./brick_sort 30 5\n");
    exit(1);
  }

  //printf("N %d chunk_sz %d left over %d\n",N, chunk_sz, left_over);

  int j;
 
  if(my_rank != comm_sz-1){
    for(i=0; i<1; i++){
      for(j = 0; j<chunk_sz/2+1; j++){
	even(&b[chunk_sz*my_rank], chunk_sz);
	odd(&b[chunk_sz*my_rank], chunk_sz);
      }
      int *my_buffer = malloc(chunk_sz*sizeof(int));
      if(i%2 == 0){
	if(my_rank<2){
	if(my_rank%2 == 0 ){
	  printf("here\n");
	  MPI_Sendrecv(&b[chunk_sz*my_rank], chunk_sz, MPI_INT, my_rank+1, 0, my_buffer, chunk_sz, MPI_INT, my_rank+1, 0, MPI_COMM_WORLD, NULL);
	  int k;
	  for(k=0; k<chunk_sz; k++)
	    b[chunk_sz*my_rank+k] = my_buffer[k];



	} else { 
	  MPI_Sendrecv(&b[chunk_sz*my_rank], chunk_sz, MPI_INT, my_rank-1, 0, my_buffer, chunk_sz, MPI_INT, my_rank-1, 0, MPI_COMM_WORLD, NULL);

	  int k;
	  for(k=0; k<chunk_sz; k++)
	    b[chunk_sz*my_rank+k] = my_buffer[k];

	}
	}
      }
    }
  } else {
    
    for(j = 0; j<chunk_sz/2+1; j++){
      even(&b[chunk_sz*my_rank], left_over);
      odd(&b[chunk_sz*my_rank], left_over); 
      }  
  }


  if(my_rank == 0){
    //printf("my_rank = %d\n", my_rank);
    for(i = 0; i< N; i++)
      printf("%d ", b[i]);
    printf("\n");
    }


  
  qsort(a, N, sizeof(int), compare);
  /*
  for(i =0; i<N; i++)
    if(a[i] != b[i]){
      //printf("%d\n", b[i]);
      printf("%d != %d\n", a[i], b[i]);
      break;
    }
  */
  free(a);
  free(b);
  MPI_Finalize();


  return 0;
}

