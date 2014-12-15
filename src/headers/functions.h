#ifndef FUNCTIONS_H
#define FUNCTIONS_H


#include "structures.h"
#include "set.h"
#include "heap.h"
/*=====================

funzioni generiche

=======================*/
GRAPHOBJ *initializeGraph ( BDEF build, IEDEF insertEdge, WDEF getWeight, 
	VINS insertVertex, TDEF transpose, ADEF getAdjList, PATH path );

/**
 * MAZE
 * @param  width   [description]
 * @param  height  [description]
 * @param  mazeStr [description]
 * @return         [description]
 */
VCOORD *getCoord ( GRAPHOBJ *graph, int u );
int coordToID ( GRAPHOBJ *graph, VCOORD *c );
VCOORD **buildMap ( GRAPHOBJ * graph, char *mazeStr );

Set 	*	printPath ( GRAPHOBJ *graph, int s , int v, int *pred, Set *succ, FILE *stream );
void 		minPath ( GRAPHOBJ *graph, int s, int v );
void 		cPrintMaze ( GRAPHOBJ *graph, Set * succ, int s, int v );


#endif
