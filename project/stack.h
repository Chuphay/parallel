#ifndef _stack_h
#define _stack_h

typedef struct s_node{
  int key;
  int count;
  struct s_node *next;
} s_node;

void destroy_stack(s_node *root);

void push_stack(int key, s_node **root);

void peek_stack(int *key, int *count, s_node **root);

void pop_stack(int *key, int *count, s_node **root);

int search_stack(int key, int *count, s_node *root);

void print_stack(s_node *root);

#endif
