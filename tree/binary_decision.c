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

typedef struct data_set {

  int features;
  int length;
  double **data;

} data_set;

data_set *init_data_set(int features, int length) {
  data_set *out = malloc(sizeof(data_set));
  double **data = malloc(features*length*sizeof(double*));
  int i;
  for(i=0;i<features;i++){
    data[i] = malloc(length*sizeof(double));
  }
  out->features = features;
  out->length =length;
  printf("here\n");
  data[0][0] = 3;
  printf("here\n");
  out->data = data;
  return out;
}

void destroy_data_set(data_set *d){
  int i;
  for(i = 0; i < d->features; i++)
    free(d->data[i]);
  free(d->data);
  free(d);
}



double entropy(double q){

  if((q<0)||(q>1)){
    printf("entropy of %f?\n",q);
    exit(1);
  }
  if((q < 0.01) || (q > 0.99))
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

  double first, second;
  if(pos_pos + pos_neg == 0)
    first = 0;
  else
    first = entropy(pos_pos/((double)(pos_pos + pos_neg)))*(pos_pos + pos_neg)/length;
  if(neg_pos + neg_neg == 0)
    second = 0;
  else
    second = entropy(neg_pos/((double)(neg_pos + neg_neg)))*(neg_pos + neg_neg)/length;
  printf("first %f and second %f\n", first, second);
  return first + second;



}

void make_splits(int (*data)[MAX], int *target, int data_length, int feat_length){

  int i;
  double smallest =  get_prob_a(data[0], target, data_length);
  int split= 0;
  for(i=1; i< feat_length; i++){
   double temp = get_prob_a(data[i], target, data_length);
   if(temp<smallest){
     smallest = temp;
     split = i;
   }
  }
  printf("split: %d\n",split);

  //now, amazingly we are going to split on the split
  //but that means we will need to make two new data sets...
  //well... maybe we don't actually need new data sets

  //yeah... we really will need to make a new data set
}



int main(){

  data_set *d = init_data_set(4,8);

  int data[4][8] = {};
  int j;

    for(j=0;j<8; j++){
      //(d->data)[0][j] = 0;//d->data[1][j] = d->data[2][j] = d->data[3][j] = 0;

      if(j/4 == 0)
	data[0][j] = 1;
      if((j/2)%2 == 0)
	data[1][j] = 1;
      if(j%2 == 0)
	data[2][j] = 1;
    }
    data[3][0] = data[3][4] = data[3][7] = data[3][2] = data[3][6] =1;
    //data[3][2] should be data[3][7], but changed it a little
    //to make the algorithm a little cooler  

    print(data);


    printf("prob %f\n", get_prob(&data[3][4], 4));

    //get_prob_a(data[0],data[3],8);
    //get_prob_a(data[1],data[3],8);
    //get_prob_a(data[2], data[3],8);


  printf("entropy of 0.5 %f\n", entropy(0.5));
  make_splits(data, data[3], 8, 3);


  destroy_data_set(d);


  return 0;
}
