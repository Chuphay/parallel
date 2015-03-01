#include <stdio.h>
#include <stdlib.h>
#include "stack.h"


typedef struct vertex{
  int identity;
  int *edges;
  int num_edges;
  int cluster;
} vertex;

typedef struct cluster{
  int identity;
  stack *external_edges;
  int *internal_vertices;
  int num_internal_vertices;
  int area;
  int perimeter;
} cluster;

int find_next_vertex(vertex **graph, cluster *cluster, int K){
  //returns the number of the next cluster to add
  //if there is a good one
  //otherwise returns -1

  int out = -1;
  int keys[2*K];
  int counts[2*K];
  int tot_count = get_stack(keys, counts, 2*K, cluster->external_edges);
  for(int i=0; i<tot_count; i++)
    printf("tot--key: %d count: %d\n", keys[i], counts[i]); 

  return out;
}

void add_vertex(int v, vertex **graph, cluster *cluster){

  for(int i=0; i< graph[v]->num_edges; i++){
    push_stack(graph[v]->edges[i], &(cluster->external_edges));
  }

}





int main(){
  int N = 6;
  int K = 3;

  cluster *cluster_one = malloc(sizeof(cluster));
  cluster_one->external_edges = NULL;

  vertex **graph = malloc(N*sizeof(vertex *));
  for(int i = 0; i< N; i++){
    graph[i] = malloc(sizeof(vertex));
    graph[i]->edges = malloc(2*K*sizeof(int)); //2*K for worst case scenrio
  }
 
  graph[0]->identity = 0;
  graph[0]->edges[0] = 1;
  graph[0]->edges[1] = 2;
  graph[0]->num_edges = 2;
  graph[0]->cluster = 0;

  graph[1]->identity = 1;
  graph[1]->edges[0] = 0;
  graph[1]->edges[1] = 2;
  graph[1]->num_edges = 2;
  graph[1]->cluster = 0;

  graph[2]->identity = 2;
  graph[2]->edges[0] = 1;
  graph[2]->edges[1] = 0;
  graph[2]->edges[2] = 3;
  graph[2]->num_edges = 3;
  graph[2]->cluster = 0;

  graph[3]->identity = 3;
  graph[3]->edges[0] = 2;
  graph[3]->edges[1] = 4;
  graph[3]->edges[2] = 5;
  graph[3]->num_edges = 3;
  graph[3]->cluster = 0;

  graph[4]->identity = 4;
  graph[4]->edges[0] = 3;
  graph[4]->edges[1] = 5;
  graph[4]->num_edges = 2;
  graph[4]->cluster = 0;

  graph[5]->identity = 5;
  graph[5]->edges[0] = 3;
  graph[5]->edges[1] = 4;
  graph[5]->num_edges = 2;
  graph[5]->cluster = 0;

  add_vertex(0, graph, cluster_one);
  find_next_vertex(graph, cluster_one, K);

  for(int i = 0; i< N; i++){
    free(graph[i]->edges);
    free(graph[i]);
  }
  free(graph);
  destroy_stack(cluster_one->external_edges);
  free(cluster_one);





  stack *root = NULL; //necessary to initialize root to 0
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
  int keys[12];
  int counts[12];
  int tot_count = get_stack(keys, counts, 12, root);
  for(int i=0; i<tot_count; i++)
    printf("tot--key: %d count: %d\n", keys[i], counts[i]); 



  destroy_stack(root);
  return 0;
}
