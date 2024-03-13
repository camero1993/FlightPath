/*
 * Author: Tammy VanDeGrift, Tanya Crenshaw, Martin Cenek
 * main.c
 * Fall 2023 HW5 starter code
 * creates a graph of airports and runs dijkstra's algorithm to find
 * the shortest cost flight path from a source to a destination
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <limits.h>
#include "main.h"
#include "graph.h"

/* 
 * main
 * Creates a graph of airports and runs dijkstra's algorithm to find 
 * the shortest path from a source airport to a destination airport.
 * usage: hw6 SampleRoutes.txt PDX MCO
 */
int main(int argc, char * argv[]) {
  // Check command line arguments.
  if(argc != REQ_PARAM) {
    usage(argv[0]);
  }
  Graph* graph = buildGraph(argv[1]);
  if (graph == NULL ) exit(0);
  // Sort the array of airport names lexicographically using the built-in
  // C function qsort().
  //qsort(airports, sizeof(airports)/sizeof(*airports), sizeof(*airports), compare);

  //printf("Creating a graph for %d airports.\n", numberOfAirports);

  // Given a number and a list of labels, initialize a graph data structure
  // for that many nodes, labeled with the labels.
  // &numberOfAirports does not exist any more get it from graph->V

  printGraph(graph);

  printf("Enter search source and destination airports separated by space\n");
  printf("Non existent airport will terminate the program\n");
  printf("Usage Example: \"PDX ATL\"\n");
  int rc = 0;
  char source[MAXWIDTH+1], dest[MAXWIDTH+1];
  source[MAXWIDTH]  ='\0';
  dest[MAXWIDTH]    ='\0';
  while((rc=scanf("%3s %3s", source, dest)) == 2){
  	 // Are the city names too long?
    if(strlen(source) > MAXWIDTH) {
      err(source);
    }
  	if(strlen(dest) > MAXWIDTH) {
    	err(dest);
  	}
  
    // Are the names given at the command line in the list of airports?
	 int srcfound = 0;
	 int dstfound = 0;
	 for(int i = 0; i < graph->V; i++) {
	   if(strncmp(source, graph->array[i].label, MAXWIDTH) == 0) srcfound = 1;
	    if(strncmp(dest, graph->array[i].label, MAXWIDTH) == 0) dstfound = 1;
	 }
	  
  	if(!srcfound) {
  	  err(source);
  	}
  	if(!dstfound) {
  	  err(dest);
  	}

    printf("\n------------------------------\n\n");

    // Find the shortest paths and print the result.  
     dijkstra(graph, source);
    printLabels(graph);

    printf("\n------------------------------\n\n");
  
    int destNum = -1;
    int srcNum = -1;
    // Get the index number of the destination and the source.
    int i = 0;
    while(i < graph->V && srcNum == -1) {
      if( strncmp(source, graph->array[i].label, MAXWIDTH) == 0) {
        srcNum = i;
      }
      i++;
    }

    i = 0;
    while(i < graph->V && destNum == -1) {
      if( strncmp(dest, graph->array[i].label, MAXWIDTH) == 0) {
        destNum = i;
      }
      i++;
    }
    
    // Start at the destination, and going backwards to the source
    // find all of the members of the path from the source to the destination.
    // As I find each one, keep track of it in the path array.
    int path[graph->V];

    printf("Getting the path from %s to %s. \n", graph->array[srcNum].label, graph->array[destNum].label);

    int j = 0;
    int pathFinder = destNum;
     int valid = 1;
    while(pathFinder != srcNum && valid == 1 ) {
      if(graph->array[pathFinder].pred<0){
        fprintf(stderr,"Destination is not reachable, try again\n");
        valid =-1;
      }
      path[j] = graph->array[pathFinder].pred;
      pathFinder = graph->array[pathFinder].pred;
      j++;
    } 
    if(valid ==1){
      printf("The flight path is: \n\n");
      printf("The flight path is %d flight(s) long: \n\n", j);

      // Print the path backwards.
      j--;
      while(j >= 0) {
        printf("%s -> ", graph->array[path[j]].label);
        j--;
      }
    
      // Print the final destination.
      printf("%s ", dest);
    
      // Print the total mileage.
      printf("%d miles.\n", graph->array[destNum].dValue); 
      printf("\n------------------------------\n");
      }
    reset(graph);
    } 
  freeGraph(&graph);

  return EXIT_SUCCESS;
}

 

/*
 * compare
 * Compare two strings.  That is, given two elements, extract the char
 * pointers from them and compare the strings using strncmp().
 */
int compare (const void * elem1, const void * elem2) {
  
  char * s1 = *(char **)elem1;
  char * s2 = *(char **)elem2;
  
  return  strncmp(s1, s2, MAXWIDTH); 
}


/* 
 * usage
 * Print the usage for this program and exit program.
 */
void usage(char * s) {
  printf("usage: %s <source city name> <dest. city name>\n", s);
  printf("  This program finds the shortest flight path from the given source city to destination.\n");
  exit(-1);
}

/* 
 * err
 * Print an error message for this program and exit proram.
 */
void err(char * s) {
  printf("%s is not in this set of airports.\n", s);
  exit(-1);
}
