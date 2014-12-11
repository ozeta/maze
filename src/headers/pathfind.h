#ifndef PATH_H
#define PATH_H

#include <panel.h>
#include <ncurses.h>
#include "structures.h"
#include "set.h"
#include "heap.h"
/*=====================


=======================*/

Set *getAdjList ( struct gObj *graph, int u );
int 	*	breadth_first_search ( GRAPHOBJ *graph, int s );
void 		printPath ( GRAPHOBJ *graph, int s , int v, int *pred );
void 		minPath ( GRAPHOBJ *graph, int s, int v );
#endif