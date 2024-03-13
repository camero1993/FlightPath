#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "graph.h"

void dijkstra(Graph * g, char * source)
{
  int src = -1;
  for (int i = 0; i < g->V; i++){ //match source to its corresponding node
    if(strcmp(source,g->array[i].label) == 0){
      src = i; //save the node
      break;
    }
  }

  if(src == -1){ //not found
    return;
  }
  
  g->array[src].dValue = 0; //set the source's distance from itself to 0
  
  while(isEmpty(g) == 0){ //iterate through array until all are black
    int U = getMin(g);

    if (U ==-1){ //if no more white nodes
      break;
    }
    
    AdjListNode* N = g->array[U].head; //first neighbor is the start of list
    
    while (N != NULL) {
      int nbr = N->dest;
      
      if (g->array[nbr].dValue > g->array[U].dValue + N->cost){
	// if found new minimum
	
	g->array[nbr].dValue = g->array[U].dValue + N->cost; //save new shortest path
	g->array[nbr].pred = U; //set predecessor
      }
      N = N->next; //check next neighbor
      
    }
    g->array[U].color = BLACK; //done looking at node
  }
  
}


int isEmpty(Graph * g){
  //1 for empty, 0 for non empty
  if (g == NULL){ //check if graph is null
    return 1;
  }
    
  for (int i = 0; i < g->V; i++){ //if found any white, non empty
    if (g->array[i].color == WHITE){
      return 0;
    }
  }
  return 1;
}

int getMin(Graph * g){
  if (g == NULL) {
    return -1;
  }
  if (isEmpty(g) == 1){
    return -1;
  }
  
  int smallestV = INF; //initialize smallest Value and smallest Index
  int smallestI = -1;
  for (int i = 0; i < g->V; i++){//iterate through graph, find smallest index
    if (g->array[i].color == WHITE){
      if (g->array[i].dValue < smallestV){
	smallestV = g->array[i].dValue;
	smallestI = i;
      }
    }
  }
  return smallestI; //return smallest index
}
