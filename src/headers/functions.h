#ifndef FUNCTIONS_H
#define FUNCTIONS_H


#include "structures.h"
#include "set.h"
#include "heap.h"
/*=====================

funzioni generiche

=======================*/
GRAPHOBJ *initializeGraph ( BDEF build, IEDEF insertEdge, WDEF getWeight, 
	VINS insertVertex, TDEF transpose, ADEF getAdjList );

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

#endif
