#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

/*
typedef struct s_node{
  int key;
  int count;
  struct s_node *next;
} s_node;
*/

void destroy_stack(s_node *root){
  if(root != NULL){
    destroy_stack(root->next);
    free(root);
  }
}

void push_stack(int key, s_node **root){
  if(*root == NULL){
    //initialize the stack
    *root = (s_node *)malloc(sizeof(s_node));
    if(root == NULL){
      printf("couldn't initialize the stack. exiting\n");
      exit(1);
    }

    (*root)->key = key;
    (*root)->count = 1;
    (*root)->next = NULL;     
  } else {
    //stack has already been initialized
    //now we need to check if the key is already in the stack
    s_node **leaf = root;
    int flag = 0;
    while(*leaf != NULL){
      if((*leaf)->key == key){
	(*leaf)->count++;
	flag = 1;
	break;
      }
      leaf = &(*leaf)->next;
    } 
    if(flag == 0){
      //key is not in the stack
      //therefore we add it to the stack
      //we need to switch the root with the new key value
      //so that it is on top of the stack

      s_node *new_root = malloc(sizeof(s_node));
      if(new_root == NULL){
	printf("couldn't get enough memory. exiting\n");
	exit(1);
      }
      new_root->key = (*root)->key;
      new_root->count = (*root)->count;
      new_root->next = (*root)->next;
      (*root)->key = key;
      (*root)->count = 1;
      (*root)->next = new_root;
    }
  }
}

void peek_stack(int *key, int *count, s_node **root){
  *key = (*root)->key;
  *count = (*root)->count;
}

void pop_stack(int *key, int *count, s_node **root){
  *key = (*root)->key;
  *count = (*root)->count;

  s_node *leaf = (*root)->next;
  (*root)->key = leaf->key;
  (*root)->count = leaf->count;
  (*root)->next = leaf->next;

  free(leaf);
}

int search_stack(int key, int *count, s_node *root){
  //this one is kind of confusing
  //search for a key
  //I'll place the count in the count variable
  //returns 1 for success
  //and 0 for fail
  int flag = 0;
  while(root != NULL){
    if(root->key == key){
      *count = root->count;
      flag = 1;
      break;
    }
    root = root->next;
  }
  return flag;
}





void print_stack(s_node *root){
  if(root != NULL){
    printf("key: %d count: %d\n", root->key, root->count);
    print_stack(root->next);
  }
}

/*
int main(){
  s_node *root = NULL; //necessary to initialize root to 0
  print_stack(root);
  push_stack(3, &root);
  push_stack(76, &root);
  push_stack(4, &root);
  push_stack(3, &root);
  push_stack(17, &root);
  push_stack(76, &root);
  push_stack(76, &root);
  int key, count;
  peek_stack(&key, &count, &root); 
  printf("Peeking: %d, %d\n", key, count);
  print_stack(root);
  pop_stack(&key, &count, &root);
  printf("popped %d, %d\n", key, count);
  print_stack(root);
  int count2;
  int success = search_stack(78, &count2, root);
  printf("success in finding 78: %d, and count %d\n", success, count2);
  destroy_stack(root);
  return 0;
}
*/
