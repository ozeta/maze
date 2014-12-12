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

void predPrint ( GRAPHOBJ *graph, int *pred, char *str ) {
	int i;
	FILE *stream = openStream ( str, "w+" );
	for (i = 0; i < graph->vNum; ++i) {
		fprintf ( stream, "pred[%4d]= %4d\n", i, pred[i] );
	}
	closeStream ( stream );
}

void printAdj ( GRAPHOBJ *graph, int u, Set *Adj, FILE *stream ) {
	char *s ="-------------------";
	if ( Adj ) {
		fprintf ( stream, "nodo %3d:\n", u );
		fprintf ( stream, "Adiacenze:\n" );
		printSet ( Adj, stream );
		fprintf ( stream, "\n%s%s%s\n", s, s, s );
	} else {
		fprintf ( stream, "nessuna adiacenza\n" );
	}
}
void printDist ( GRAPHOBJ *graph, FILE *stream, int *dist ) {
	fprintf ( stream, "distanze:\n\n");
	int k;
	for ( k = 0; k < graph->vNum; k++ ) {
		fprintf ( stream, " [%d]->%d, ", k, dist[k]);
	}
	fprintf ( stream, "\n\n" );	
}
int getMatrixWeight ( GRAPHOBJ *graph, int x, int y ) {
	int weight = 0;

	if ( graph && graph->matrix ) {
		weight = graph->matrix[x][y].weight;
	}
	return weight;
}

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
int *dijkstraHeap ( GRAPHOBJ *graph, int s, int target ) {

	Heap 	*		Q 			= NULL;
	Set 	*		AdjList 	= NULL;
	Data 	*		tmp;
	int 	*		pred;


	int DEBUG = 0;
	FILE *stream;
	Q = initializeHeap ( minHeapify );
	int i 	= 0;
	int v 	= 0;
	int u 	= 0;
	int alt = 0;
	pred 	= ( int *) malloc ( graph->vNum * sizeof ( int ) );
	int colore[graph->vNum];
	int dist[graph->vNum];

	//initializations
	for ( v = 0; v < graph->vNum; v++ ) {
		dist[v] 	= INFINITE;
		pred[v] 	= NIL;
		colore[v] 	= BIANCO;
		//if ( s != v )
		//	insert ( Q, new_HeapData ( v, INFINITE ) );
	}
	dist[s] = 0;
	pred[s] = s;
	insert ( Q, new_HeapData ( s, 0 ) );

	char path[50];
	buildHeap ( Q );

	//main loop
	while ( Q->heapsize > 1 ) {
		//remove and return the best vertex
		tmp 	= extractFirst ( Q );
		u 		= getData ( tmp );
		dist[u] = getKey ( tmp );
		colore[u] = NERO;
		AdjList = graph->getAdjList ( graph, u );
		//printf ( "%d ", u);
		//graph->maze[getCoord ( graph, u )->y][getCoord ( graph, u )->x].path = true;
		while ( !isEmpty ( AdjList ) ) {
			v = getInt ( getFront ( AdjList ) );
			
			if ( colore[v] == BIANCO ) {
				insert ( Q, new_HeapData ( v, dist[u] ) );
				relax ( graph, Q, u, v, dist, pred );
			}

			AdjList = dequeue ( AdjList );
		}
		if ( u == target ) {
			printf ("\n---target: %4d(%3d,%d)---\n",u, (getCoord ( graph, u))->x, (getCoord ( graph, u ))->y);
			//relax ( graph, Q, u, v, dist, pred );
			//pred[u] = v;
			predPrint ( graph, pred, "dijkstra.txt" );	
			return pred;			
		}
	}

	//predPrint ( graph, pred, "dijkstra.txt" );	
	return pred;
}
void relax ( GRAPHOBJ *graph, Heap *Q, int u, int v, int *dist, int *pred) {
	int alt;
	alt = dist[u] + graph->getWeight ( graph, u, v );
	if ( dist[v] > alt ) {
		dist[v] = alt;
		pred[v] = u;
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

				predPrint ( graph, pred, "bfs.txt" );
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
void printAllpreds ( GRAPHOBJ *graph, int *pred ) {
	int i,j;
	for ( i = 0; i < graph->height; i++ ) {
		for ( j = 0; j < graph->width; j++ ) {
		
			if ( pred[i*graph->width+j] != NIL ) {
				printf (ANSI_COLOR_BLUE"%d"ANSI_COLOR_RESET, graph->maze[i][j].k);
			} else {
				printf ("%d", graph->maze[i][j].k);
			}
		}
		printf ( "\n" );
	}	
	printf ( "\n" );
}
void minPath ( GRAPHOBJ *graph, int s, int v ) {

	int 		*	pred 	= NULL;
	Set 		*	succ 	= NULL;
	Set 		*	def 	= NULL;
	int y = 2;
	int x = 4;
	pred = graph->path ( graph, s, v );

	FILE *stream = openStream ( "pred.txt", "w+" );
	succ = printPath ( graph, s, v, pred, succ, stream );
	closeStream ( stream );
	int i,j;
	int k = 0;
		//printAllpreds ( graph, pred );
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
			if ( i*graph->width+j == s ||  i*graph->width+j == v ) {
			//if ( 0 ) {
			//	printf ("--%d--",s);
				printf (ANSI_COLOR_RED"%d"ANSI_COLOR_RESET, graph->maze[i][j].k);

			} else 
			//if ( 0 ) {
			if ( graph->maze[i][j].path == true ) {
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