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

  int c_identity = cluster->identity;
  graph[v]->cluster = c_identity;
  cluster->internal_vertices[cluster->num_internal_vertices] = v;
  cluster->num_internal_vertices++;

 

  int new_node;
  for(int i=0; i< graph[v]->num_edges; i++){
    //looking at the nodes that the vertex points to
    new_node = graph[v]->edges[i]; 
    if(graph[new_node]->cluster == 0){
      //new_node really is a new node
      push_stack(new_node, &(cluster->external_edges));
      cluster->perimeter++;
    } else if(graph[new_node]->cluster == c_identity){
      //this means that this edge already points to the cluster
      //so, we should delete this edge
      //oor just pass?

    } else {
      //I believe this means it points to some other cluster
      //ignore these
      printf("in this sort of weird place...\n");
      printf("adding node %d, but it apparently belongs somewhere else\n", new_node);
    }
  }
  //how many edges point to this vertex?
  if(cluster->num_internal_vertices != 1){
    //above if to check this is not the first node

    int n_count = del_key_from_stack(v, &(cluster->external_edges));

    //take that number away from the perimeter
    cluster->perimeter -= n_count;

    //and add that number to the area
    cluster->area += n_count;


    if(n_count == -1){
      printf("woh... count of edges is negative?\n");
      exit(1);
    }
  }
}





int main(){
  int N = 6;
  int K = 3;

  cluster *cluster_one = malloc(sizeof(cluster));
  cluster_one->identity = 1;
  cluster_one->external_edges = NULL;
  cluster_one->internal_vertices = malloc(N*sizeof(int));
  cluster_one->num_internal_vertices = 0;
  cluster_one->area = 0;
  cluster_one->perimeter = 0;

  vertex **graph = malloc(N*sizeof(vertex *));
  for(int i = 0; i< N; i++){
    graph[i] = malloc(sizeof(vertex));
    graph[i]->edges = malloc(2*K*sizeof(int)); //2*K for worst case scenario
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
  add_vertex(1, graph, cluster_one);
  add_vertex(2, graph, cluster_one);
  find_next_vertex(graph, cluster_one, K);

  printf("\nidentity: %d\n",cluster_one->identity);
  printf("external edges\n");
  print_stack(cluster_one->external_edges);
  printf("\ninternal edges\n");
  for(int i = 0; i< cluster_one->num_internal_vertices; i++){
    printf("%d\n",  cluster_one->internal_vertices[i]);
  }
  printf("area: %d\n", cluster_one->area);
  printf("perimeter: %d\n", cluster_one->perimeter);

  for(int i = 0; i< N; i++){
    free(graph[i]->edges);
    free(graph[i]);
  }
  free(graph);
  destroy_stack(cluster_one->external_edges);
  free(cluster_one->internal_vertices);
  free(cluster_one);



   
  /*
  stack *root = NULL; //necessary to initialize root to 0
  print_stack(root);
  //push_stack(3, &root);
  
  push_stack(76, &root);
  //push_stack(4, &root);
  //push_stack(3, &root);
  //push_stack(17, &root);
  //push_stack(76, &root);
  //push_stack(76, &root);
  int key, count;
  
  peek_stack(&key, &count, &root); 
  printf("Peeking: %d, %d\n", key, count);
  print_stack(root);
  // pop_stack(&key, &count, &root);
  // printf("popped %d, %d\n", key, count);
  //pop_stack(&key, &count, &root);
  //printf("popped %d, %d\n", key, count);
  print_stack(root);
  int count2;
  int success = search_stack(78, &count2, root);
  printf("success in finding 78: %d, and count %d\n", success, count2);
  int keys[12];
  int counts[12];
  //int tot_count = get_stack(keys, counts, 12, root);

  int n_count = del_key_from_stack(76, &root);
  printf("deleted and the count was %d\n", n_count);

  int tot_count = get_stack(keys, counts, 12, root);
  for(int i=0; i<tot_count; i++)
    printf("tot--key: %d count: %d\n", keys[i], counts[i]); 

  

  destroy_stack(root);
  */

  return 0;
}
