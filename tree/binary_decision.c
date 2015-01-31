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
    return -(q * log(q)/log(2) + (1-q)*log(1-q)/log(2));
}

double get_prob(int *target, int length){
  int i;
  double out = 0.0;
  for(i = 0; i <length; i++){
    out += target[i];
  }
  return out/length;
}

double get_prob_a(int *input, int *target, int length){

  int i;
  int pos_pos = 0;
  int pos_neg = 0;
  int neg_pos = 0;
  int neg_neg = 0;
  for(i = 0; i<length; i++){
    if((input[i] == 1) && (target[i] == 1))
      pos_pos++;
    if((input[i] == 1) && (target[i] == 0))
      pos_neg++;
    if((input[i] == 0) && (target[i] == 1))
      neg_pos++;
    if((input[i] == 0) && (target[i] == 0))
      neg_neg++;
  }
  printf("pos_pos: %d pos_neg: %d \n", pos_pos, pos_neg);
  printf("neg_pos: %d neg_neg: %d\n", neg_pos, neg_neg);


  //apparently the most important equation is this:

  // \sum_{k=1}^2 \frac {p_k+n_k} {p+n} entropy(\frac {p_k} {p_k+n_k}).

  return 0.0;



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

    get_prob_a(data[0],data[3],8);
    get_prob_a(data[1],data[3],8);
    get_prob_a(data[2], data[3],8);


  printf("entropy of 0.5 %f\n", entropy(0.5));

  //apparently the most important equation is this:

  // \sum_{k=1}^2 \frac {p_k+n_k} {p+n} entropy(\frac {p_k} {p_k+n_k}).


  return 0;
}
