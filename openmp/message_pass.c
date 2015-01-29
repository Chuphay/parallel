#include <stdio.h>
#include <stdlib.h>

#define MAX 2

typedef struct queue{
  char *name;
  int value[MAX];
  int start;
  int length;
} queue;

void push(queue *q, int value){
  if(q->length+1 > MAX){
    printf("queue %s is filled. Value %d will not fit\nexiting\n", q->name,value);
    exit(1);
  }

  q->value[(q->start + q->length)%MAX] = value;
  q->length++;
}

int pop(queue *q){
  int out = q->value[q->start];
  q->start = (q->start + 1)%MAX;
  q->length--;
  return out;
}

queue *create(char *name){
  queue *q = malloc(sizeof(queue));
  q->name = name;
  q->start = 0;
  q->length = 0;
 
  return q;
}


int main(){
  queue *q = create("one");

  push(q,4);
  push(q,5);
  int a = pop(q);
  printf("popped: %d\n", a);
  push(q,6);
  a = pop(q);
  printf("popped: %d\n", a);
  push(q,7);
  a = pop(q);
  printf("popped: %d\n", a);
  push(q,8);
  a = pop(q);
  printf("popped: %d\n", a);
  a = pop(q);
  printf("popped: %d\n", a);
  push(q,9);
  push(q,10);
  push(q,11);

  free(q);

  return 0;
}
 
