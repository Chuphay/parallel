#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

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
