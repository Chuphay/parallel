#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//gcc -Wall -o binary_decision binary_decision.c -lm


#define FEATURES 4
#define MAX 8

void print(int (*data)[MAX]){

  int i,j;
  for(i = 0; i< MAX; i++){
    for(j=0;j<FEATURES; j++){
      printf("%d ",data[j][i]);
    }
    printf("\n");
  }
}


double entropy(double q){

  if((q<0)||(q>1)){
    printf("entropy of %f?\n",q);
    exit(1);
  }

  if((q == 0) || (q == 1))
    return 0;
  else 
    return -(q * log(q) + (1-q)*log(1-q));
}

double get_prob(int *target, int length){
  int i;
  double out = 0.0;
  for(i = 0; i <length; i++){
    out += target[i];
  }
  return out/length;
}




int main(){
  printf("on\n");

  int data[4][8] = {};
  int j;

    for(j=0;j<8; j++){
      if(j/4 == 0)
	data[0][j] = 1;
      if((j/2)%2 == 0)
	data[1][j] = 1;
      if(j%2 == 0)
	data[2][j] = 1;
    }
    data[3][0] = data[3][3] = data[3][5] = data[3][2] = 1;
    //data[3][2] should be data[3][7], but changed it a little
    //to make the algorithm a little cooler  

    print(data);


    printf("prob %f\n", get_prob(&data[3][4], 4));

  return 0;
}
