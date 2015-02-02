#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

//./dijkstra 16 1

int nv, *notdone, nth, chunk, md, mv, largeint = -1;

unsigned *ohd; //1-hop distance between vertices
               //"ohd[i][j]" = ohd[i*nv +j]
unsigned *mind; //min distance so far

void init (int ac, char **av){
  int i,j;
  nv = atoi(av[1]);
  ohd = malloc(nv*nv*sizeof(int));
  mind = malloc(nv*sizeof(int));
  notdone = malloc(nv*sizeof(int));
  //random graph
  for(i = 0; i< nv; i++){
    for(j = 0; j < nv; j++){
      if(i == j) ohd[i*nv + i] = 0;
      else {
	ohd[nv*i + j] = rand()%20;
	ohd[nv*j + i] = ohd[nv*i+j];
      }
    }
  }
  for(i=1;i<nv;i++){
    notdone[i] = 1;
    mind[i] = ohd[i];
  }
}

//finds closest to 0 among notdone, among s through e
void findmymin(int s, int e, unsigned *d, int *v){
  int i;
  *d = largeint;
  for(i = s; i<= e; i++)
    if(notdone[i] && mind[i]<*d){
      *d = mind[i];
      *v = i;
    }
}

void updatemin(int s, int e){
  int i;
  for(i = s; i <= e; i++)
    if(mind[mv]+ohd[mv*nv+i]<mind[i])
      mind[i] = mind[mv] + ohd[mv*nv + i];
}

void dowork(){

#pragma omp parallel
  {int startv, endv, step, mymv, me = omp_get_thread_num();
    unsigned mymd;
#pragma omp single
    {nth = omp_get_num_threads();
      if(nv%nth != 0){
	printf("nv must be divisible by nth\n");
	exit(1);
      }
      chunk = nv/nth;
    }
    startv = me*chunk;
    endv = startv + chunk -1;
    for(step = 0; step<nv; step++){
#pragma omp single
      {md = largeint; mv =0;}

      findmymin(startv, endv, &mymd, &mymv);

#pragma omp critical
      {if(mymd<md)
	  {md = mymd; mv = mymv;}
      }
#pragma omp barrier
#pragma omp single
      {notdone[mv] = 0;}
      updatemin(startv, endv);
#pragma omp barrier
    }
  }
}

int main(int argc, char **argv){
  srand(time(NULL));
  int i,j, print;
  double startime, endtime;
  init(argc, argv);
  startime = omp_get_wtime();
  dowork();
  endtime = omp_get_wtime();

  printf("elapsed time: %f\n", endtime-startime);
  print = atoi(argv[2]);
  if(print){
    printf("graph weights:\n");
    for(i=0; i<nv; i++){
      for(j=0;j<nv;j++)
	printf("%u ", ohd[nv*i+j]);
      printf("\n");
    }
    printf("minimum distance: \n");
    for(i = 1; i<nv; i++)
      printf("%u\n", mind[i]);
  }
  return 0;
}

