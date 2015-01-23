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
  printf("succes up to %ld\n", amount);
  return 0;
}
