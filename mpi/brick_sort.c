#include <stdio.h>
#include <stdlib.h>

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
  int i;
  for(i = 0; i< n/2 ; i++){
    if(compare((const void *) &a[2*i+1], (const void *) &a[2*i]) == -1){
      //printf("i = %d\n",i);
      int tmp = a[2*i];
      a[2*i] = a[2*i + 1];
      a[2*i + 1] = tmp;
    }
  }
}
void odd(int *a, int n){
  int i;
  int flag = 0;
  if(n/2 == 0)
    flag = 1;
  for(i = 0; i< n/2 -flag; i++){
    if(compare((const void *) &a[2*i+2], (const void *) &a[2*i+1]) == -1){
      //printf("i = %d\n",i);
      int tmp = a[2*i+1];
      a[2*i+1] = a[2*i+2];
      a[2*i+2] = tmp;
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

  //for(i = 0; i< N; i++)
  //   printf("%d ", b[i]);
  //printf("\n");

  for(i = 0; i<N/2+1; i++){
    even(b,N);
    odd(b,N); 
  }
  
  qsort(a, N, sizeof(int), compare);

  for(i =0; i<N; i++)
    if(a[i] != b[i]){
      printf("%d != %d\n", a[i], b[i]);
      break;
    }


  return 0;
}

