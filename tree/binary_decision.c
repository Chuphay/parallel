#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//gcc -Wall -o binary_decision binary_decision.c -lm


typedef struct data_set {
  int features;
  int length;
  int **data;
} data_set;

void print(data_set *d){
  int i,j;
  for(i = 0; i< d->length; i++){
    for(j=0;j<d->features; j++){
      printf("%d ",d->data[j][i]);
    }
    printf("\n");
  }
}



data_set *init_data_set(int features, int length) {
  data_set *out = malloc(sizeof(data_set));
  int **data = malloc(features*length*sizeof(int*));
  int i;
  for(i=0;i<features;i++){
    data[i] = malloc(length*sizeof(int));
  }
  out->features = features;
  out->length =length;
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


double get_prob_a(int *input, int *target, int length){
  //I'm not sure why I call it this
  //this calculates the entropy gain of a column

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
int sum(int *array, int length){
  int sum = 0;
  int i;
  for(i = 0; i< length; i++)
    sum += array[i];
  return sum;
} 

void make_splits(data_set *d, int *target){
  print(d);
  int i;
  for(i=0; i<d->length;i++) printf("%d",target[i]);
  printf("\n");

  double smallest =  get_prob_a(d->data[0], target, d->length);
  int split= 0;
  for(i=1; i< d->features; i++){
   double temp = get_prob_a(d->data[i], target, d->length);
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

  //first we will need a key of all the positive
  //and negatives of our split data
  //int *key_split = (int*)d->data[split];
  int positives = sum(d->data[split],d->length);
  printf("positives: %d\n",positives);

  data_set *pos = init_data_set(d->features, positives);
  data_set *neg = init_data_set(d->features, d->length - positives);
  int pos_target[positives];
  int neg_target[d->length - positives];
  int j, pos_count = 0, neg_count = 0;
  for(i = 0; i< d->length; i++){
    if(d->data[split][i] == 1){
      for(j = 0; j< d->features; j++){
	pos->data[j][pos_count] = d->data[j][i];
      }
      pos_target[pos_count] = target[i];
      pos_count++;
    } else if(d->data[split][i] == 0){ //error checking
      for(j = 0; j< d->features; j++){
	neg->data[j][neg_count] = d->data[j][i];
      }
      neg_target[neg_count] = target[i];
      neg_count++;
    } else {
      printf("should not be here\n");
    }
  }
  destroy_data_set(d);
  //printf("pos count %d neg count %d\n",neg_count, pos_count);
  //for(i=0; i<neg_count;i++) printf("%d",neg_target[i]);
  //for(i=0; i<pos_count;i++) printf("%d",pos_target[i]);
  // print(neg);
  //print(pos);

  // make_splits(neg, neg_target

  destroy_data_set(neg);
  destroy_data_set(pos);

}



int main(){

  data_set *d = init_data_set(3,8);

  int j;

    for(j=0;j<8; j++){
      d->data[0][j] = d->data[1][j] = d->data[2][j] = 0;

      if(j/4 == 0)
	d->data[0][j] = 1;
      if((j/2)%2 == 0)
	d->data[1][j] = 1;
      if(j%2 == 0)
	d->data[2][j] = 1;
    }
    int target[8] = {1,0,1,1,1,0,1,0};

    //print(d);
    //printf("prob %f\n", get_prob(&data[3][4], 4));


  printf("entropy of 0.5 %f\n", entropy(0.5));
  make_splits(d, target);


  //destroy_data_set(d);


  return 0;
}
