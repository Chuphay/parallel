#include <stdio.h>
#include <stdlib.h>
#include "graph.h"


#define MAX_CLUSTERS 10

int N, K;
int *cluster_id;
int *num_edges;


typedef struct cluster{
  int identity;
  // int *internal_vertices;
  int num_internal_vertices;
  int area;
  int perimeter;
} cluster;

void add_vertex(int v, vertex **graph, cluster *cluster);
int find_next_vertex(int seed, vertex **graph, cluster *cluster);


cluster *make_cluster(vertex **graph, int N, int K, int identity, int seed){
  printf("making a new cluster, with seed %d and identity %d\n", seed, identity);
  cluster *c = malloc(sizeof(cluster));
  if(c == NULL){
    printf("couldn't allocate for the cluster\n");
    exit(1);
  }
  c->identity = identity;
  //c->internal_vertices = malloc(N*sizeof(int));
  //if(c->internal_vertices == NULL){
  //  printf("couldn't allocate for the internal_vertices\n");
  //  exit(1);
  // }
  c->num_internal_vertices = 0;
  c->area = 0;
  c->perimeter = 0;

  add_vertex(seed, graph, c);
  int next_vertex = find_next_vertex(seed, graph, c);
  while(next_vertex != -1){
    add_vertex(next_vertex, graph, c);
    next_vertex = find_next_vertex(next_vertex, graph, c);
  }
  return c;
}


void destroy_clusters(cluster **clusters, int num_clusters){

  for(int i = 0; i< num_clusters; i++){
    //destroy_stack(clusters[i]->external_edges);

    // free(clusters[i]->internal_vertices);
    free(clusters[i]);
  }
  free(clusters);
}


int find_next_vertex(int seed, vertex **graph, cluster *cluster){
  //returns the number of the next cluster to add
  //if there is a good one
  //otherwise returns -1
  //We will only look at edges that the seed node contains

  double old_ratio = (cluster->area)/((double)cluster->perimeter + 0.00001); //avoid divide by zero
  double new_ratio;

  int out = -1;

  int new_node;
  for(int i=0; i< graph[seed]->num_edges; i++){

    new_node = graph[seed]->edges[i];
    if(cluster_id[new_node] == 0){
 
      int num_new_edges = 0;
      for(int j=0; j<graph[new_node]->num_edges; j++){
	if(num_edges[graph[new_node]->edges[j]] == 0)
	  num_new_edges++;
      }

      double new_perimeter = cluster->perimeter + num_new_edges - num_edges[new_node];
      double new_area = cluster->area + num_edges[new_node];


      printf("node %d perimeter %f area %f\n",new_node, new_perimeter, new_area);

      new_ratio = new_area/new_perimeter;
      //do our stopping condition
      if(new_ratio >= old_ratio){
	old_ratio = new_ratio;
	out = new_node;
      }
    }
  }

  return out;
}

void add_vertex(int v, vertex **graph, cluster *cluster){
  printf("adding %d\n", v);
  int c_identity = cluster->identity;
  graph[v]->cluster = c_identity;
  //cluster->internal_vertices[cluster->num_internal_vertices] = v;
  cluster->num_internal_vertices++;
  cluster_id[v] = c_identity;

  int num_edges_pointing_to_cluster = 0;
  int new_node;
  for(int i=0; i< graph[v]->num_edges; i++){
    //looking at the nodes that the vertex points to
    new_node = graph[v]->edges[i]; 
    if(graph[new_node]->cluster == 0){
      //new_node really is a new node
      //push_stack(new_node, &(cluster->external_edges));
      num_edges[new_node]++;
      cluster->perimeter++;
    } else if(graph[new_node]->cluster == c_identity){
      //this means that this edge already points to the cluster
      //and so we just pass
      num_edges_pointing_to_cluster++;

    } else {
      //this means it points to some other cluster
      //ignore these for now
      num_edges_pointing_to_cluster++;
      printf("%d is already in a cluster...\n", graph[new_node]->identity);

    }
  }
  //how many edges point to this vertex?
  if(cluster->num_internal_vertices != 1){
    //above if to check this is not the first node

    int n_count = num_edges[v]; 
    if(num_edges_pointing_to_cluster != n_count){

      printf("yikes.... aborting\n");
      printf("seed: %d num_edges: %d n_count: %d\n",v, num_edges_pointing_to_cluster, n_count);
      exit(1);
    }
    //take that number away from the perimeter
    cluster->perimeter -= n_count;

    //and add that number to the area
    cluster->area += n_count;

    num_edges[v] = 0;

    if(n_count == -1){
      printf("woh... count of edges is negative?\n");
      exit(1);
    }
  }
}



int main(){
 

  vertex **graph = make_graph(&N, &K,"simple.data");
  printf("N = %d, K = %d\n",N,K);
  cluster_id = malloc(N*sizeof(int));
  num_edges = malloc(N*sizeof(int));
  for(int i = 0; i<N; i++){
    num_edges[i] = 0;
    cluster_id[i] = 0;
  }



  cluster **clusters = malloc(MAX_CLUSTERS*sizeof(cluster *));
  int num_clusters = 0;
  for(int i = 0; i<N; i++){
    if(graph[i]->cluster == 0){
      clusters[num_clusters] = make_cluster(graph, N, K, num_clusters+1, i);
      num_clusters++;
      if(num_clusters>=MAX_CLUSTERS-1){
	//minus to be on the safe side 
	printf("ukk... too many clusters\n");
	exit(1);
      }
    }
  }



  printf("num_of_clusters %d\n", num_clusters);
  print_clusters(N, graph);

  destroy_graph(N, graph);
  destroy_clusters(clusters, num_clusters);

  return 0;
}
