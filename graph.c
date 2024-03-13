/*
 * Author: Tammy VanDeGrift, Tanya Crenshaw, Martin Cenek
 * graph.c
 * Fall 2023 HW5 starter code
 * Graph function implementations, based on adjacency list
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h" 

/* 
 * makeAdjListNode
 * makes new adjacency list node using destination node value and cost to the 
 * node
 */
AdjListNode * makeAdjListNode(int dest, int cost) {
  // costs cannot be negative
  if (cost < 0) return NULL;
  // dest cannot be negative
  if (dest < 0) return NULL;
  
  AdjListNode * newNode = (AdjListNode*) malloc(sizeof(AdjListNode));
  
  // malloc may have failed.
  if (newNode == NULL) return NULL;
  
  // Initialize all the fields with the values coming in
  // via function parameters.
  newNode->dest = dest;
  newNode->cost = cost;  
  return newNode;
}

/*
  resets the graph's adjacency edge pred, color, d_value so
  dijkstra can run on the graph multiple times
*/
void reset(Graph* graph){
  for (int i = 0; i < graph->V; ++i) {
    graph->array[i].pred = NIL;
    graph->array[i].color = WHITE;
    graph->array[i].dValue = INF;    
  } 
}

/*
 * buildThisGraph
 * construct edges from airport to airport
 */
Graph* buildGraph(char* fn) {
  FILE* fp = fopen(fn, "r");
  if (fp==NULL){
    printf("ERROR: Could not open file for reading\n");
    return NULL;
  }
  Graph* graph = (Graph*) malloc(sizeof(Graph));
  // malloc may have failed.
  if (graph == NULL) return NULL;   
  // Initialize the number of nodes.

  // Read and Check number of cities; otherwise,
  // return NULL to indicate nothing was created.
  fscanf(fp,"%d\n", &(graph->V));
  if( graph->V <= 0) return NULL;
  
  // Create an array of adjacency lists.  Size of array is V.
  graph->array = (AdjList*) malloc(graph->V * sizeof(AdjList));
  
  // malloc may have failed.
  if (graph->array == NULL) {
    free(graph);
    return NULL;  
  }
  
  // Create a jump table of labels to node numbers.  Simply
  // use the string array that was handed to this function.

  // check Dijkstra for waht the jump array does!
  //graph->jump = labels;  
  // Initialize the graph data structure.
  // Initialize each adjacency list as empty by making head as NULL
  // and labeling the node using the name read from the file
  char name[MAXWIDTH+1], or[MAXWIDTH+1], dest[MAXWIDTH+1];
  name[MAXWIDTH]='\0';
  int dist;
  or[MAXWIDTH]='\0';
  dest[MAXWIDTH]='\0';
  for(int i=0;i<graph->V; i++){
    fscanf(fp,"%3s\n", name);
 
    graph->array[i].pred = NIL;
    graph->array[i].color = WHITE;
    graph->array[i].dValue = INF;    
    graph->array[i].head = NULL;
    strncpy(graph->array[i].label,name, MAXWIDTH);
  } 
  // Initialize the graph's edges.
    while (fscanf(fp,"%s %s %d\n", or, dest, &dist)==3){
    //printf("Read: %s %s %d\n", or, dest, dist);
    addEdge(graph, or, dest, dist);
  }
  fclose(fp);
  return graph;
}

/*
 * addEdge
 * Add an edge to an undirected graph using the src, dest, and cost for the 
 * edge.
 */
int addEdge(Graph * graph, char * src, char * dest, int cost) {
  // check arguments
  if (graph == NULL || src == NULL || dest == NULL) return ADD_PARAM_FAILURE;
  if (cost < 0) return ADD_COST_FAILURE;

  // Add an edge from src to dest.  A new node is added to the adjacency
  // list of src.  The node is added to the beginning of the list.
 
  // look up the source name
  int i = 0;
  int srcNum = -1;
  while(i < graph->V && srcNum == -1) {
    if( strncmp(src, graph->array[i].label, MAXWIDTH) == 0) {
      srcNum = i;
    }
    i++;
  }

  // look up the dest name
  i = 0;
  int destNum = -1;
  while(i < graph->V && destNum == -1) {
    if(strncmp(dest, graph->array[i].label, MAXWIDTH) == 0) {
      destNum = i;
    }
    i++;
  }
  
  // If the source's name or destination's name couldn't be found,
  // then return a failure code.
  if(srcNum == -1 || destNum == -1)  return ADD_NOT_FOUND; 

  // Create a new node.  Make sure it was created successfully.
  AdjListNode* newNode = makeAdjListNode(destNum, cost);
  if(newNode == NULL) return ADD_ALLOC_FAILURE;

  // Add the new node to the appropriate linked list.
  newNode->next = graph->array[srcNum].head; 
  // Put new node at beginning
  graph->array[srcNum].head = newNode;
 
  // Since graph is undirected, add an edge from dest to src also
  //newNode = makeAdjListNode(srcNum, cost);
  //newNode->next = graph->array[destNum].head;
  //graph->array[destNum].head = newNode;

  return ADD_SUCCESS;
}

/*
 * printLabels
 * print all labels and their integer values
 * helpful for debugging
 */
void printLabels(Graph * graph) {
  // cannot print labels for NULL graph
  if(graph == NULL) return;

  int v;
  for (v = 0; v < graph->V; ++v) {
    printf("City %d: %s.  dValue: %d\n", v, graph->array[v].label, 
	   graph->array[v].dValue);
  }
  return;
}

/*
 * printGraph
 * prints the array of adjacency lists for the graph
 * helpful for debugging
 *
 */
void printGraph(Graph * graph) {
  // cannot print labels for an empty graph.
  if(graph == NULL) return;
  
  int v;  
  // For each adjacency list, print every node in the adjacency list,
  // using the graph's jump table to convert from a vertex's integer
  // value to a label.
  for (v = 0; v < graph->V; ++v) {
    AdjListNode* curr = graph->array[v].head;
    printf("\n Adjacency list for city #%d: %s\n", v, graph->array[v].label);
    
    // As long as the adjacency list still has nodes...
    while (curr != NULL) {
      printf("-> %s ", graph->array[curr->dest].label);
      curr = curr->next;
    }
    printf("\n");
  }
  return;
}

/*
 * freeGraph
 * frees all memory allocated to the graph
 */
void freeGraph(struct Graph ** graphAddr) {
  AdjListNode* nextNode;

  if (graphAddr == NULL || *graphAddr == NULL){
    return;
  }
  
  for(int i = 0; i < (*graphAddr)->V; i++){
    //start at head node of the current graph node
    AdjListNode* currNode = (*graphAddr)->array[i].head;
    AdjListNode* temp;

    while (currNode != NULL) { //free entire linked list
      temp = currNode;
      currNode = currNode->next;
      free(temp); 
    }
   
  }
  free((*graphAddr)->array);
  free((*graphAddr));
  *graphAddr = NULL;
}
