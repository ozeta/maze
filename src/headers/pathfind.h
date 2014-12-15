#ifndef PATH_H
#define PATH_H

#include <panel.h>
#include <ncurses.h>
#include "structures.h"
#include "set.h"
#include "heap.h"
/*=====================


=======================*/
int 		getMatrixWeight ( GRAPHOBJ *graph, int x, int y );
Set 	*	getAdjList ( struct gObj *graph, int u );

/**
 * BFS
 */
int 	*	breadth_first_search ( GRAPHOBJ *graph, int s, int target );

/**
 * DIJKSTRA
 */
int 	*	dijkstraHeap ( GRAPHOBJ *graph, int s, int target );
void 		relax ( GRAPHOBJ *graph, Heap *Q, int u, int v, int *dist, int *prev);

/**
 * A STAR
 */
int heuristic ( GRAPHOBJ *graph, int s, int t );

int *a_star ( GRAPHOBJ *graph, int s, int t );
#endif