#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "./headers/functions.h"
#include "./headers/set.h"
#include "./headers/lib.h"
#include "./headers/heap.h"
#include "./headers/pathfind.h"
#include "./headers/structures.h"

#define NORTH(y) y - 1
#define SOUTH(y) y + 1
#define EAST(y) y + 1
#define WEST(y) y - 1

Set *getAdjList ( struct gObj *graph, int u ) {
	Set *Adj = NULL;
	int x = u % graph->width;
	int y = u / graph->width;
	int neighbor;

	if ( NORTH(y) > 0 && graph->maze[NORTH(y)][x].k != 9 ) {
		neighbor = graph->width * ( NORTH(y) ) + ( x );
		Adj = enqueue ( Adj, setInt ( neighbor ) );
	}
	if ( EAST(x) < graph->width && graph->maze[y][EAST(x)].k != 9  ) {
		neighbor = graph->width * ( y ) + ( EAST(x) );		
		Adj = enqueue ( Adj, setInt ( neighbor ) );
	}
	if ( SOUTH(y) < graph->height && graph->maze[SOUTH(y)][x].k != 9 ) {
		neighbor = graph->width * ( SOUTH(y) ) + ( x );		
		Adj = enqueue ( Adj, setInt ( neighbor ) );
	}
	if ( WEST(x) > 0 && graph->maze[y][WEST(x)].k != 9 ) {
		neighbor = graph->width * ( y ) + ( WEST(x) );		
		Adj = enqueue ( Adj, setInt ( neighbor ) );
	}

	return Adj;
}
/**
 * visita breadth_first_search su matrice di adiacenza
*/

int *breadth_first_search ( GRAPHOBJ *graph, int s, int target ) {

	if ( !check ( graph, "breadth_first_search" ) ) return NULL;
	if ( s >= graph->vNum ) {
		//dialogPanel ( "breadth_first_search: s > dimensione" );
		return NULL;
	}

	int u;
	int v;
	int colore[graph->vNum];
	int *	pred		= ( int *) malloc ( graph->vNum * sizeof ( int ) );
	memset ( pred, 0, graph->vNum );
	Set *	frontier	= NULL;
	Set *	AdjList 	= NULL;

	for ( u = 0; u < graph->vNum; u++ ) {
		colore[u] 	= BIANCO;
		pred[u]		= NIL;
	}
	colore[s] 	= GRIGIO;
	pred[s]		= 0;

	frontier = enqueue ( frontier, setInt ( s ) );

	while ( !isEmpty ( frontier ) ) {
		u = getInt ( getFront ( frontier ) );
		AdjList = graph->getAdjList ( graph, u );

		while ( !isEmpty ( AdjList ) ) {
			v = getInt ( getFront ( AdjList ) );
			AdjList = dequeue ( AdjList );
			if ( colore[v] == BIANCO ) {
				colore[v] = GRIGIO;
				pred[v] = u;
				frontier = enqueue ( frontier, setInt ( v ) );
			}
			if ( v == target ) {
				return pred;
			}
		}
		frontier = dequeue ( frontier );
		colore[u] = NERO;
	}
	return pred;
}
/**
stampa del cammino minimo
*/
Set *printPath ( GRAPHOBJ *graph, int s , int v, int *pred, Set *succ ) {

    if (v == s) {
 		printf ( "s: %d\n", s  );
 		succ = enqueue ( succ, setInt ( s ) );
    } else if ( pred[v] == NIL ) {
		printf ( "\n--non esiste cammino tra s e v--\n");
    } else {
		printf ( "v: %d\n", v );
		succ = enqueue ( succ, setInt ( v ) );
		printPath (graph, s, pred[v], pred, succ );
    }
    return succ;
}
/**
cammino minimo
*/
void minPath ( GRAPHOBJ *graph, int s, int v ) {

	int 		*	pred 	= NULL;
	Set 		*	succ 	= NULL;
	Set 		*	def 	= NULL;
	int y = 2;
	int x = 4;
	pred = breadth_first_search ( graph, s, v );
	succ = printPath ( graph, s, v, pred, succ );
	int i,j;
	int k = 0;
/*
	k = getInt ( getTail ( succ ) );
	!isEmpty ( succ );
*/

	while ( !isEmpty ( succ ) ) {
		k = getInt ( getFront ( succ ) );
		def = push ( setInt ( k ) );
		succ = dequeue ( succ );
	}

	k = NIL;
	k = getInt ( top ( def ) );
	def = pop ( def );

	FILE *stream = openStream ( "succ.txt", "w+" );
	printSet ( succ, stream );
	for ( i = 0; i < graph->height; i++ ) {
		for ( j = 0; j < graph->width; j++ ) {
			
			if ( pred[i*69+j] != NIL ) {
				printf (ANSI_COLOR_BLUE"%d"ANSI_COLOR_RESET, graph->maze[i][j].k);
			} else {
				printf ("%d", graph->maze[i][j].k);
			}
		}
	printf ( "\n" );
	}	
	closeStream ( stream );
	free ( pred );
}
