#ifndef FUNCTIONS_H
#define FUNCTIONS_H


#include "structures.h"
#include "set.h"
#include "heap.h"
/*=====================

funzioni generiche

=======================*/
GRAPHOBJ *	initializeGraph ( BDEF build, IEDEF insertEdge, WDEF getWeight, 
	VINS insertVertex, TDEF transpose, ADEF getAdjList, PATH path );
GraphT 		matrixEdgeInizialize(void);
GRAPHOBJ *	buildGraph ( GRAPHOBJ *graphObj, int size );
void 		matrixInsertInterface ( GRAPHOBJ *graph );
GraphT	**	appendOneVertexIntoGraph ( GRAPHOBJ *graph );
GraphT	**	removeOneVertexFromGraph ( int vertex, GRAPHOBJ * graph ) ;
int 		getMatrixWeight ( GRAPHOBJ *graphObj, int x, int y );
void 		insertEdgeIntograph ( GRAPHOBJ *graph, int x, int y, int weight );
void 		deleteEdgeFromgraph ( GRAPHOBJ *graph, int x, int y );
void 		randFillGraph (int inf, int sup, GRAPHOBJ *graph);
int 		randomIntGenerator (int, int);
void 		FillGraph ( GRAPHOBJ *graph, insDEF insertMethod, int inf, int sup );
Set 	*	getAdjList ( GRAPHOBJ *graph, int u );


/**
 * MAZE
 * @param  width   [description]
 * @param  height  [description]
 * @param  mazeStr [description]
 * @return         [description]
 */
VCOORD	*	getCoord ( GRAPHOBJ *graph, int u );
int 		coordToID ( GRAPHOBJ *graph, VCOORD *c );
VCOORD	**	buildMap ( GRAPHOBJ * graph, char *mazeStr );
GRAPHOBJ * 	convertToExplicitMap( GRAPHOBJ *graph, GRAPHOBJ *newGraph, char *mazeStr );
Set 	*	explicitPrintPath ( GRAPHOBJ *graph, int s , int v, int *pred, Set *succ, FILE *stream );
Set 	*	printPath ( GRAPHOBJ *graph, int s , int v, int *pred, Set *succ, FILE *stream );
Set 	*	minPath ( GRAPHOBJ *graph, int s, int v );
void 		cPrintMaze ( GRAPHOBJ *graph, Set * succ, int s, int v );


#endif
