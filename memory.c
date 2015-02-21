#include <stdio.h>
#include <stdlib.h>

int main(){
  int i;
  long amount = 1;
  for(i=0;i<100;i++){
    int *p = malloc(amount*sizeof(int));
    if(p == NULL){
      printf("fail on %ld\n", amount);
      break;
    }
    free(p);
    amount = 10*amount;
  }
  double *p = malloc(2*65536*sizeof(double));
  if(p == NULL) printf("fail\n");
  printf("succes up to %ld\n", amount);
  printf("sizeof(double) %d\n", sizeof(double));
  printf("sizeod(int) %d\n", sizeof(int));
  return 0;
}
