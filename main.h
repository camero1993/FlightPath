/* 
 * Author: Tammy VanDeGrift, Tanya Crenshaw, Martin Cenek
 * Fall 2023 HW5 starter code
 * main.h
 * contains constants and function prototypes
 */

#ifndef _MAIN_H_
#define _MAIN_H_

#include "graph.h"

// Define constants for main
#define REQ_PARAM 2
#define SRC 2
#define DST 3

// Define the function prototypes for the functions implemented in main.c
int compare (const void * elem1, const void * elem2);
void err(char * s);
void usage(char * s);

// These functions are what students implement in dijkstra.c
void dijkstra(Graph * g, char * source);  
int isEmpty(Graph * g);
int getMin(Graph * g);

#endif
