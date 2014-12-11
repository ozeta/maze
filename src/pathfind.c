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

Set *getAdjList ( struct gObj *graph, int u ) {
	Set *Adj = NULL;
	int x = u % graph->width;
	int y = u / graph->width;
	int neighbor;

	if ( y - 1 > 0 && graph->maze[y - 1][x] != 9 ) {
		neighbor = graph->width * ( y - 1 ) + ( x );
		Adj = enqueue ( Adj, setInt ( neighbor ) );
	}
	if ( x + 1 < graph->width && graph->maze[y][x + 1] != 9  ) {
		neighbor = graph->width * ( y ) + ( x + 1 );		
		Adj = enqueue ( Adj, setInt ( neighbor ) );
	}
	if ( y + 1 < graph->height && graph->maze[y+1][x] != 9 ) {
		neighbor = graph->width * ( y + 1 ) + ( x );		
		Adj = enqueue ( Adj, setInt ( neighbor ) );
	}
	if ( x - 1 > 0 && graph->maze[y][x - 1] != 9 ) {
		neighbor = graph->width * ( y ) + ( x - 1 );		
		Adj = enqueue ( Adj, setInt ( neighbor ) );
	}

	return Adj;
}
/**
 * visita breadth_first_search su matrice di adiacenza
*/

int *breadth_first_search ( GRAPHOBJ *graph, int s ) {

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
		}
		frontier = dequeue ( frontier );
		colore[u] = NERO;
	}
	return pred;
}
/**
stampa del cammino minimo
*/
void printPath ( GRAPHOBJ *graph, int s , int v, int *pred ) {

    if (v == s) {
 		printf ( "s: %d\n", s  );
    } else if ( pred[v] == NIL ) {
		printf ( "\n--non esiste cammino tra s e v--\n");
    } else {
		printPath (graph, s, pred[v], pred );
		printf ( "v: %d\n", v );
    }
}
/**
cammino minimo
*/
void minPath ( GRAPHOBJ *graph, int s, int v ) {

	int 		*	pred 	= NULL;
	int y = 2;
	int x = 4;

	pred = breadth_first_search ( graph, s );
	printPath ( graph, s, v, pred );
	int i,j;
	/*
	for ( i = 0; i < graph->vNum; i++ ) {
		printf ( "pred[%4d] = %d\n", i, pred[i] );
	}
*/

	for ( i = 0; i < graph->height; i++ ) {
		for ( j = 0; j < graph->width; j++ ) {
			
			if ( pred[i*69+j] != NIL ) {
				printf (ANSI_COLOR_BLUE"%d"ANSI_COLOR_RESET, graph->maze[i][j]);
			} else {
				printf ("%d", graph->maze[i][j]);
			}
		}
	printf ( "\n" );
	}	
	free ( pred );
}
