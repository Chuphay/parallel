#include <stdio.h>

#define MAX 3

typedef struct node{

  double key[MAX];
  struct node *bigger;
  struct node *smaller;

} node;

void destroy(node *leaf){

  if(leaf != NULL){
    destroy(leaf->bigger);
    destroy(leaf->smaller);
    free(leaf);
  }
}

void insert(double key[MAX], struct node **leaf){

  //http://www.cprogramming.com/tutorial/c/lesson18.html
}





int main(){

  node t;
  t.key[0] = 3;

  printf("key: %f\n",t.key[0]);
  return 0;
}
