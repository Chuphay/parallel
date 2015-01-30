#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define MAX 2

typedef struct queue{
  char *name;
  int value[MAX];
  int start;
  int length;
} queue;

int push(queue *q, int value){
  if(q->length+1 > MAX){
    printf("queue %s is filled\n", q->name);
    return 0;
  } else {
    printf("pushing %d onto %s\n", value, q->name);
    q->value[(q->start + q->length)%MAX] = value;
    q->length++;
    return 1;
  }
}

int pop(queue *q){

  if ( q->length == 0){
    printf("erg....\nexiting\n");
    exit(1);
  }
  int out = q->value[q->start];
  printf("popping %d from %s\n", out, q->name);
  q->start = (q->start + 1)%MAX;
  q->length--;
  return out;
 
}

queue *create(char *name){
  queue *q = malloc(sizeof(queue));
  if(q == NULL){
    printf("32 bit machine :(\n");
    exit(1);
  }
  q->name = name;
  q->start = 0;
  q->length = 0;
 
  return q;
}

void pass_messages(queue **q, int num_messages, int thread_num, int num_threads){

  printf("greeting from %d of %d\n", thread_num, num_threads);

  //int num_messages = 10;
  int messages_sent[num_threads];
  int messages[num_threads][num_messages];
  int i,j;
  for(i = 0; i< num_threads; i++){
    messages_sent[i] = 0;
    for(j = 0; j < num_messages; j++){
      messages[i][j] = j+1;
    }
  }
  //printf("get here?\n");

  while(1){
    if(j>32){
     break;
    }
    int flag = 0;

    //this block for sending 3 messages to the other 4 cores
    for(i = 0; i< num_threads; i++){

	printf("flag at the top %d\n", flag);
      if(i == thread_num){
	printf("getting pissed\n");
	printf("flag at the top %d\n", flag);
	flag++;
	printf("flag at the top %d\n", flag);

	} else {//we dont want to pass messages to ourselves

	  if(messages_sent[i] == num_messages){//checking if we've sent all the messages
	    flag++;
	  } else {

#pragma omp critical
	    {
	  push(q[i], messages[i][messages_sent[i]]);
	  messages_sent[i]++;
	    }

	  if(q[thread_num]->length>0){
#pragma omp critical
	    {
	      printf("popping %d from thread num: %d\n", pop(q[thread_num]), thread_num);
	    }
	  }
	}
     
      }

    }
    printf("over here %d ?\n", flag);
    //this block for final clean-up to check for any messages

    if(q[thread_num]->length == 0) flag++;
    if(flag == num_threads){
      printf("exiting gracefully?\n");
      break;
    }

    // printf("%d\n", j);
    j++;
  }


  // printf("message: %d\n", messages[3][6]);
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
  
  int i;
  int num_of_queues = 4;
  queue *que[num_of_queues];
  char *FRUIT_STRING[] = {
    "apple", "orange", "grape", "banana",
};
  for(i = 0; i< num_of_queues; i++){
    que[i]  = create(FRUIT_STRING[i%4]);
    printf("created que[%d]: %s\n", i, que[i]->name);
  }
  // printf("que[321]: %s\n", que[3]->name);
  

#pragma omp parallel
  {
    if(num_of_queues != omp_get_num_threads()){
      printf("num_of_queues != omp_get_num_threads()\n");
      exit(1);
    }

    pass_messages(que, 10, omp_get_thread_num(), omp_get_num_threads());

  }



  return 0;
}
 
