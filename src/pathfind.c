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
		colore[v] 	= BIANCO;
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
		if ( PRINTALL ) graph->maze[getCoord ( graph, u )->y][getCoord ( graph, u )->x].path = true;
		if ( u == target ) {
			return pred;			
		}
		while ( !isEmpty ( AdjList ) ) {
			v = getInt ( getFront ( AdjList ) );
			
			if ( colore[v] == BIANCO ) {
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
		if ( PRINTALL ) graph->maze[getCoord ( graph, u )->y][getCoord ( graph, u )->x].path = true;

		while ( !isEmpty ( AdjList ) ) {
			v = getInt ( getFront ( AdjList ) );
			AdjList = dequeue ( AdjList );
			if ( colore[v] == BIANCO ) {
				colore[v] = GRIGIO;
				pred[v] = u;
				frontier = enqueue ( frontier, setInt ( v ) );
			}
			if ( v == target ) {
				//predPrint ( graph, pred, "bfs.txt" );
				return pred;
			}
		}
		frontier = dequeue ( frontier );
		colore[u] = NERO;
	}

	return pred;
}

int heuristic ( GRAPHOBJ *graph, int s, int t ) {
	VCOORD *	start 		= getCoord ( graph, s );
	VCOORD *	target 		= getCoord ( graph, t );
	//return ( abs ( start->x - target->x ) + abs ( start->y - target->y ) );
	return pow ( pow ( ( target->x - start->x ), 2 ) + pow ( ( target->y - start->y ), 2 ), 0.5 );
}

int *a_star ( GRAPHOBJ *graph, int s, int t ) {

	void 	*	tmp;
	Set 	*	AdjList		= NULL;
	Set 	*	closed 		= NULL;
	Heap 	*	open 		= NULL;
	int 	*	pred		= ( int *) malloc ( graph->vNum * sizeof ( int ) );

	int g[graph->vNum];
	int f[graph->vNum];
	int i;
	for ( i = 0; i < graph->vNum; i++ ) {
		f[i] 		= INFINITE;
		g[i] 		= 0;
		pred[i] 	= NIL;
	}

	f[s] = heuristic ( graph, s, t );
	int x, y;
	int cost;
	open = initializeHeap ( minHeapify );
	bool found = false;
	insert ( open, new_HeapData ( s, 0 ) );
	while ( !isHeapEmpty ( open ) && found == false ) {
		tmp 	= extractFirst ( open );
		push ( closed , setInt ( x ) );
		x 		= getData ( tmp );
		f[x]	= getKey ( tmp );
		if ( PRINTALL ) graph->maze[getCoord ( graph, x )->y][getCoord ( graph, x )->x].path = true;
		//printf ("x: %d ", x);
		if ( x == t ) {
			found = true;
		} else {
			
			AdjList = graph->getAdjList ( graph, x );

			while ( !isEmpty ( AdjList ) ) {
				y = getInt ( getFront ( AdjList ) );
				g[y] = g[x] + graph->getWeight ( graph, x, y );
				//cost = g[y] + heuristic ( graph, y, t );
				cost = g[y] + heuristic ( graph, y, t );
				bool yInOpen = heapIntSearch ( open, y );
				bool yInClosed = intSearch ( closed , y );
				if ( yInOpen && cost < f[y] ) { //1
					decreaseKey ( open, y, cost );
				} 
				
				else if (  yInClosed && cost < f[y] ) { //2
					deleteFromSet ( closed, y );
					insert ( open, new_HeapData ( y, cost ) );
				}
				
				else if ( !yInClosed && !yInOpen ) { // 3
					insert ( open, new_HeapData ( y, cost ) );
				}
				AdjList = dequeue ( AdjList );
				
				if ( pred[y] == NIL )
					pred[y] = x;
				//printf ("%d ", y);
				//printf ("pred[%d]: %d\n", y, x);
				
			}

		}
				
	}
	pred[s] = NIL;
	printf ("\n\n");
/*
	for ( i = 0; i < graph->vNum; i++ ) {
		printf ("pred[%d]: %d\n", i, pred[i]);
	}

	int cur = t;
	printf ("<--prev-->\n");
	printf ("%d ", cur );
	
	while ( cur >= 0 && pred[cur] != NIL ) {
		printf ("%d ", pred[cur] );
		cur = pred[cur];
	}

	*/
	return pred;
}
int getMatrixWeight ( GRAPHOBJ *graph, int u, int v ) {
	int weight = 2;
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