#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "stack.h"

int N,K;

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

  double ratio = (cluster->area)/((double)cluster->perimeter + 0.00001); //avoid divide by zero
  double new_ratio;

  int out = -1;
  int keys[2*K]; //because the perimeter can be bigger than K
  int counts[2*K];
  int tot_count = get_stack(keys, counts, 2*K, cluster->external_edges);
  //tot_count is the number of edges in the cluster perimeter
  //keys store the vertices in the perimeter 
  //counts store the number of edges pointing to that vertex

  for(int i=0; i<tot_count; i++){
    //obviously we will go over all the vertices in the perimeter
    vertex *v = graph[keys[i]];

    //we will need to count how many edges the perimeter and the new vertex share in common
    int num_of_edges_in_perimeter = 0;

    for(int j=0; j< v->num_edges; j++){  //here we go over all the edges of the vertex

      int not_using_this; //not using this, but my stack API requires it for some reason
      int in_perimeter = search_stack(v->edges[j], &not_using_this, cluster->external_edges);
      num_of_edges_in_perimeter += in_perimeter;
    }

    //now we have the egdes that are shared
    //but we still need to account for the edges pointing to the cluster
    //however, this is obviously the same as the number of edges pointing to the new vertex
    //and we already have this in counts[i] 
    double new_perimeter = v->num_edges - num_of_edges_in_perimeter - counts[i];
    double new_area = cluster->area + counts[i];
    new_ratio = new_area/new_perimeter;

    //do our stopping condition
    if(new_ratio >= ratio){
      ratio = new_ratio;
      out = keys[i];
    }
  }

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
      //or just pass?

    } else {
      //I believe this means it points to some other cluster
      //ignore these for now
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

cluster *make_cluster(vertex **graph, int N, int K, int identity, int seed){

  cluster *c = malloc(sizeof(cluster));
  if(c == NULL){
    printf("couldn't allocate for the cluster\n");
    exit(1);
  }
  c->identity = identity;
  c->external_edges = NULL;
  c->internal_vertices = malloc(N*sizeof(int));
  if(c->internal_vertices == NULL){
    printf("couldn't allocate for the internal_vertices\n");
    exit(1);
  }
  c->num_internal_vertices = 0;
  c->area = 0;
  c->perimeter = 0;

  add_vertex(seed, graph, c);
  int next_vertex = find_next_vertex(graph, c, K);
  while(next_vertex != -1){
    add_vertex(next_vertex, graph, c);
    next_vertex = find_next_vertex(graph, c, K);
  }
  return c;
}

void get_data(int *N, int *K, char *file){
  FILE * fp = fopen(file, "r");
  if(file == NULL){
    printf("couldn't open the file\n");
    exit(1);
  }

  fclose(fp);
}



int main(){

  vertex **graph = make_graph(&N, &K,"simple.data");
  printf("N = %d, K = %d\n",N,K);
  //print_graph(N, graph);

  cluster *cluster_one = make_cluster(graph, N, K, 1, 3);

  printf("identity: %d\n",cluster_one->identity);
  printf("external edges\n");
  print_stack(cluster_one->external_edges);
  printf("\ninternal edges\n");
  for(int i = 0; i< cluster_one->num_internal_vertices; i++){
    printf("%d\n",  cluster_one->internal_vertices[i]);
  }
  printf("area: %d\n", cluster_one->area);
  printf("perimeter: %d\n", cluster_one->perimeter);

  print_clusters(N, graph);

  destroy_graph(N, graph);
  destroy_stack(cluster_one->external_edges);

  free(cluster_one->internal_vertices);
  free(cluster_one);

  return 0;
}
