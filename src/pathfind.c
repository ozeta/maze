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
void dijkstraHeap ( GRAPHOBJ *graph, int s, int *prev ) {

	Heap 	*		Q 			= NULL;
	Set 	*		AdjList 	= NULL;
	Data 	*		tmp;
	int DEBUG = 0;
	FILE *stream;
	Q = initializeHeap ( minHeapify );
	int i 	= 0;
	int v 	= 0;
	int u 	= 0;
	int alt = 0;
	int colore[graph->vNum];
	int dist[graph->vNum];

	//initializations
	for ( v = 0; v < graph->vNum; v++ ) {
		dist[v] 	= INFINITE;
		prev[v] 	= NIL;
		colore[v] 	= BIANCO;
		if ( s != v )
			insert ( Q, new_HeapData ( v, INFINITE ) );
	}
	dist[s] = 0;
	prev[s] = s;
	insert ( Q, new_HeapData ( s, 0 ) );

	char path[50];
	buildHeap ( Q );
	if ( DEBUG ) {
		sprintf (path, "dijkstra_%c.txt", 'a' + s); 
		stream = openStream ( path, "w+" );	
		fprintf ( stream, "from %c\n\n", 'a' + s );
		Hprint ( Q, stream );
	}

	//main loop
	while ( Q->heapsize > 1 ) {
		//remove and return the best vertex
		
		tmp 	= extractFirst ( Q );
		u 		= getData ( tmp );
		dist[u] = getKey ( tmp );
		colore[u] = NERO;
		AdjList = graph->getAdjList ( graph, u );
		if ( DEBUG ) {
			fprintf ( stream, "nodo estratto: %3d\n", u );
			Hprint ( Q, stream );
			printDist ( graph, stream, dist );
			printAdj ( graph, u, AdjList, stream );
		}
		while ( !isEmpty ( AdjList ) ) {
			v = getInt ( getFront ( AdjList ) );
			if ( colore[v] == BIANCO ) {
				relax ( graph, Q, u, v, dist, prev );
			}
			AdjList = dequeue ( AdjList );
		}
	}
	if ( DEBUG ) {	
		fprintf ( stream, "prev:\n\n" );
		for (u = 0; u < graph->vNum; u++ ) {
			fprintf ( stream, "[%d]->%d ", u, prev[u] );
		}
		fprintf ( stream, "\n" );	
		closeStream ( stream );
	}
}
void relax ( GRAPHOBJ *graph, Heap *Q, int u, int v, int *dist, int *prev) {
	int alt;
	alt = dist[u] + graph->getWeight ( graph, u, v );
	if ( dist[v] > alt ) {
		dist[v] = alt;
		prev[v] = u;
		decreaseKey ( Q, v, alt ) ;
	}
}

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
Set *printPath ( GRAPHOBJ *graph, int s , int v, int *pred, Set *succ, FILE *stream ) {

    if (v == s) {
 		fprintf ( stream, "s: %d\n", s  );
 		graph->maze[getCoord ( graph, s )->y][getCoord ( graph, s )->x].path = true;
 		succ = enqueue ( succ, setInt ( s ) );
    } else if ( pred[v] == NIL ) {
		fprintf ( stream, "\n--non esiste cammino tra s e v--\n");
    } else {
		fprintf ( stream, "v: %d\n", v );
		succ = enqueue ( succ, setInt ( v ) );

		graph->maze[getCoord ( graph, v )->y][getCoord ( graph, v )->x].path = true;
		printPath (graph, s, pred[v], pred, succ, stream );
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
	FILE *stream = openStream ( "prev.txt", "w+" );
	succ = printPath ( graph, s, v, pred, succ, stream );
	closeStream ( stream );
	int i,j;
	int k = 0;

	//FILE *stream1 = openStream ( "def.txt", "w+" );
	//printSet ( def, stream1 );
	cPrintMaze ( graph, succ, s, v );
	//closeStream ( stream1 );
	free ( pred );

}

void cPrintMaze ( GRAPHOBJ *graph, Set * succ, int s, int v ) {

	int i, j, k;
	VCOORD *cur;

	for ( i = 0; i < graph->height; i++ ) {
		for ( j = 0; j < graph->width; j++ ) {
			
			//if ( i*69+j == k ) {
			if ( i*69+j == s ||  i*69+j == v ) {
				printf (ANSI_COLOR_RED"%d"ANSI_COLOR_RESET, graph->maze[i][j].k);

			} else 
			if ( graph->maze[i][j].path == true ) {
			//if ( pred[i*69+j] != NIL ) {
				printf (ANSI_COLOR_CYAN"%d"ANSI_COLOR_RESET, graph->maze[i][j].k);
			} else {
				printf ("%d", graph->maze[i][j].k);
			}
		}
	printf ( "\n" );
	}

	while ( !isEmpty ( succ ) ) {
		k = getInt ( top ( succ ) );
		succ = pop ( succ );
		cur = getCoord ( graph, k );
		graph->maze[cur->y][cur->x].path = false;
	}


}