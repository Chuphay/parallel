#include <stdio.h>

#define MAX 4



typedef struct node{

  double coordinates[MAX]; 
  //coordinates[0] should provide the dimension of the data 
  int value;
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

void insert(double coordinates[MAX], int coordinate, int value, struct node **leaf){
  if(coordinate>MAX-2){
    printf("dimensions are all wrong\n");
  }

  if(*leaf == NULL){
    *leaf = malloc(sizeof(struct node));
    (*leaf)->coordinates = coordinates;
    (*leaf)->value = value;
    (*leaf)->bigger = NULL;
    (*leaf)->smaller = NULL;
  } else if( (*leaf)->coordinates[coordinate] > coordinates[coordinate]){
    insert(coordinates,coordinate,value, &(*leaf)->smaller

 


}

void insert(double key[MAX], int value,  struct node **leaf){

  //http://www.cprogramming.com/tutorial/c/lesson18.html

  if(*leaf == NULL) {

    *leaf = (struct node*) malloc( sizeof( struct node ) );
    (*leaf)->key_value = key;
    /* initialize the children to null */
    (*leaf)->left = 0;    
    (*leaf)->right = 0;  
  } else if(key < (*leaf)->key_value){

        insert( key, &(*leaf)->left );
    }
    else if(key > (*leaf)->key_value)
    {
        insert( key, &(*leaf)->right );
    }
}





int main(){

  node t;
  t.key[0] = 3;

  printf("key: %f\n",t.key[0]);
  return 0;
}
