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

bool PRINTALL = false;
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

Set *getAdjList ( struct gObj *graph, int u ) {
	Set *Adj = NULL;
	int x = u % graph->width;
	int y = u / graph->width;
	int neighbor;
	bool diagonals = false;

	if ( NORTH(y) > 0 && graph->maze[NORTH(y)][x].k != 9 ) {
	//if ( NORTH(y) > 0 ) {
		neighbor = graph->width * ( NORTH(y) ) + ( x );
		Adj = enqueue ( Adj, setInt ( neighbor ) );
	}
	if ( EAST(x) < graph->width && graph->maze[y][EAST(x)].k != 9  ) {
	//if ( EAST(x) < graph->width  ) {
		neighbor = graph->width * ( y ) + ( EAST(x) );		
		Adj = enqueue ( Adj, setInt ( neighbor ) );
	}
	if ( SOUTH(y) < graph->height && graph->maze[SOUTH(y)][x].k != 9 ) {
	//if ( SOUTH(y) < graph->height ) {
		neighbor = graph->width * ( SOUTH(y) ) + ( x );		
		Adj = enqueue ( Adj, setInt ( neighbor ) );
	}
	if ( WEST(x) > 0 && graph->maze[y][WEST(x)].k != 9 ) {
	//if ( WEST(x) > 0 ) {
		neighbor = graph->width * ( y ) + ( WEST(x) );		
		Adj = enqueue ( Adj, setInt ( neighbor ) );
	}
	if ( diagonals == true ) {
		//diagonals
		//NE
		if ( NORTH(y) > 0 && EAST(x) < graph->width && graph->maze[NORTH(y)][EAST(x)].k != 9) {
			neighbor = graph->width * ( NORTH(y) ) + ( EAST(x) );		
			Adj = enqueue ( Adj, setInt ( neighbor ) );		
		}
		//NW
		if ( NORTH(y) > 0 && WEST(x) > 0 && graph->maze[NORTH(y)][WEST(x)].k != 9) {
			neighbor = graph->width * ( NORTH(y) ) + ( WEST(x) );		
			Adj = enqueue ( Adj, setInt ( neighbor ) );		
		}
		//SE
		if ( SOUTH(y) < graph->height && EAST(x) < graph->width && graph->maze[SOUTH(y)][EAST(x)].k != 9) {
			neighbor = graph->width * ( SOUTH(y) ) + ( EAST(x) );		
			Adj = enqueue ( Adj, setInt ( neighbor ) );		
		}	
		//SW
		if ( SOUTH(y) < graph->height && WEST(x) > 0 && graph->maze[SOUTH(y)][WEST(x)].k != 9) {
			neighbor = graph->width * ( SOUTH(y) ) + ( WEST(x) );		
			Adj = enqueue ( Adj, setInt ( neighbor ) );		
		}
	}
	return Adj;
}
/**
 * visita breadth_first_search su matrice di adiacenza
*/
int *dijkstraHeap ( GRAPHOBJ *graph, int s, int target ) {
	/**
	 * inizializzazioni
	 */
	Heap 	*		Q 			= NULL;
	Set 	*		AdjList 	= NULL;
	Data 	*		tmp;
	int 	*		pred;


	FILE *stream;
	Q = initializeHeap ( minHeapify );
	int i 	= 0;
	int v 	= 0;
	int u 	= 0;
	int alt = 0;
	pred 	= ( int *) malloc ( graph->vNum * sizeof ( int ) );
	int colore[graph->vNum];
	int dist[graph->vNum];

	for ( v = 0; v < graph->vNum; v++ ) {
		dist[v] 	= INFINITE;
		pred[v] 	= NIL;
		colore[v] 	= WHITE;
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
		colore[u] = BLACK;
		AdjList = graph->getAdjList ( graph, u );
		if ( PRINTALL ) graph->maze[getCoord ( graph, u )->y][getCoord ( graph, u )->x].path = true;
		if ( u == target ) {
			return pred;			
		}
		while ( !isEmpty ( AdjList ) ) {
			v = getInt ( getFront ( AdjList ) );
			
			if ( colore[v] == WHITE ) {
				relax ( graph, Q, u, v, dist, pred );
			}
			AdjList = dequeue ( AdjList );
		}
	}
	return pred;
}
void relax ( GRAPHOBJ *graph, Heap *Q, int u, int v, int *dist, int *pred) {
	int alt;
	alt = dist[u] + graph->getWeight ( graph, u, v );
	if ( alt < dist[v] ) {
		dist[v] = alt;
		pred[v] = u;
		if ( heapIntSearch ( Q , v ) ) {
			decreaseKey ( Q, v, alt ) ;
		} else {
			insert ( Q, new_HeapData ( v, alt ) );
		}
	}
}

int *breadth_first_search ( GRAPHOBJ *graph, int s, int target ) {

	if ( !check ( graph, "breadth_first_search" ) ) return NULL;
	if ( s >= graph->vNum ) {
		return NULL;
	}

	int u;							//start node
	int v;							//target
	int color[graph->vNum];			//color of the node
	Set *	frontier	= NULL;		//frontier implemented as queue
	Set *	AdjList 	= NULL;		//Adjiacency list implemented as queue
	int *	pred		= ( int *) malloc ( graph->vNum * sizeof ( int ) );	//reverse path
	memset ( pred, 0, graph->vNum );

	//initialization
	for ( u = 0; u < graph->vNum; u++ ) {
		color[u] 	= WHITE;
		pred[u]		= NIL;
	}
	color[s] 	= GREY;
	pred[s]		= 0;

	frontier = enqueue ( frontier, setInt ( s ) );

	while ( !isEmpty ( frontier ) ) {
		u = getInt ( getFront ( frontier ) );
		AdjList = graph->getAdjList ( graph, u );

		//getCoord: uses the ID to get the x and y coordinates
		if ( PRINTALL ) graph->maze[getCoord ( graph, u )->y][getCoord ( graph, u )->x].path = true;

		while ( !isEmpty ( AdjList ) ) {
			v = getInt ( getFront ( AdjList ) );
			AdjList = dequeue ( AdjList );
			if ( color[v] == WHITE ) {
				color[v] = GREY;
				pred[v] = u;
				frontier = enqueue ( frontier, setInt ( v ) );
			}
			if ( v == target ) {
				//predPrint ( graph, pred, "bfs.txt" );
				return pred;
			}
		}
		frontier = dequeue ( frontier );
		color[u] = BLACK;
	}

	return pred;
}

int heuristic ( GRAPHOBJ *graph, int s, int t ) {
	VCOORD *	start 		= getCoord ( graph, s );
	VCOORD *	target 		= getCoord ( graph, t );
	/*manhattan*///return ( abs ( start->x - target->x ) + abs ( start->y - target->y ) );
	/*euclidian*/return pow ( pow ( ( target->x - start->x ), 2 ) + pow ( ( target->y - start->y ), 2 ), 0.5 );
}

int getMatrixWeight ( GRAPHOBJ *graph, int u, int v ) {
	int weight = 1;
	return weight;
}

/*
int getMatrixWeight ( GRAPHOBJ *graph, int u, int v ) {
	VCOORD *uu = getCoord ( graph, u );
	VCOORD *vv = getCoord ( graph, v );
	//right
	if ( abs ( uu->x - vv->x ) > 0 ) {
		if ( graph->maze[uu->y][vv->x].k == 1 ) {
			return 1;
		} else {
			return INFINITE;
		}
	} else 	if ( abs ( uu->y - vv->y ) > 0 ) {
		if ( graph->maze[vv->y][uu->x].k == 1 ) {
			return 1;
		} else {
			return INFINITE;
		}
	}

	return 1;

}
*/
int *reconstruct_path ( int *came_from, int current ) {
	Set *total_path = NULL;
	total_path = enqueue ( total_path, setInt ( current ) );
	while ( current > 0 ) {
		printf ( "current: %4d ", current );
		current = came_from[current];
		total_path = enqueue ( total_path, setInt ( current ) );
	}
	return NULL;
}
int *a_star ( GRAPHOBJ *graph, int s, int t ) {

	int i;
	int current;
	bool found 				= false;
	void 	*	tmp;					//used to store Heap first element
	Set 	*	AdjList		= NULL;		//Adjiacence list implemented as queue
	Heap 	*	frontier	= NULL;		//Open set implemented as Heap
	frontier 				= initializeHeap ( minHeapify );
	int 	*	came_from	= ( int *) malloc ( graph->vNum * sizeof ( int ) );
	int 	*	cost_so_far	= ( int *) malloc ( graph->vNum * sizeof ( int ) );


	//initialization
	for ( i = 0; i < graph->vNum; i++ ) {
		came_from[i] 	= NIL;
		cost_so_far[i] 	= INFINITE;
	}
	insert ( frontier, new_HeapData ( s, 0 ) );
	cost_so_far[s] 	= 0;
	while ( !isHeapEmpty ( frontier ) ) {
		tmp 	= extractFirst ( frontier );
		current = getData ( tmp );
		if ( 0 ) graph->maze[getCoord ( graph, current )->y][getCoord ( graph, current )->x].path = true;

		if ( current == t ) {
			break;
		}
		AdjList = graph->getAdjList ( graph, current );
		while ( !isEmpty ( AdjList ) ) {
			int y = getInt ( getFront ( AdjList ) );
			AdjList = dequeue ( AdjList );
			int new_cost = cost_so_far[current] + graph->getWeight ( graph, current, y );
			if ( cost_so_far[y] == INFINITE || new_cost < cost_so_far[y] ) {
				cost_so_far[y] = new_cost;
				int priority = new_cost + heuristic ( graph, y, t );
				insert ( frontier, new_HeapData ( y, priority ) );
				came_from[y] = current;
			}
		}
 	}
	return came_from;
}