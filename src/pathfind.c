#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
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

//stampa l'array dei vertici precedenti su stream
void predPrint ( GRAPHOBJ *graph, int *pred, char *str ) {
	int i;
	FILE *stream = openStream ( str, "w+" );
	for (i = 0; i < graph->vNum; ++i) {
		fprintf ( stream, "pred[%4d]= %4d\n", i, pred[i] );
	}
	closeStream ( stream );
}

//restituisce il peso di un arco nella mappa implicita
//si è associato ad ogni arco peso 1
int getMazeWeight ( GRAPHOBJ *graph, int u, int v ) {
	int weight = 1;
	return weight;
}

//restituisce l'insieme dei vertici adiacenti sulla mappa implicita
//sono state create 4 direzioni principali ( N S E W ) e lasciate 
//facoltative le diagonali ( flag toroidal )
Set *getMazeAdjList ( struct gObj *graph, int u ) {
	assert ( graph );
	Set *Adj = NULL;
	int x 			= u % graph->width;
	int y 			= u / graph->width;
	bool diagonals 	= false;
	bool toroid 	= false;
	int neighbor;

	if ( toroid && NORTH(y) == 0 && graph->maze[NORTH(y)][x].k != 9 ) {
		neighbor = graph->width * (  0 ) + ( x );
		Adj = enqueue ( Adj, setInt ( neighbor ) );		
	}
	if ( toroid && NORTH(y) < 0 && graph->maze[graph->height -1][x].k != 9 ) {
		neighbor = graph->width * (  graph->height -1 ) + ( x );
		Adj = enqueue ( Adj, setInt ( neighbor ) );		
	}	
	if ( NORTH(y) > 0 && graph->maze[NORTH(y)][x].k != 9 ) {
	//if ( NORTH(y) > 0 ) {
		neighbor = graph->width * ( NORTH(y) ) + ( x );
		Adj = enqueue ( Adj, setInt ( neighbor ) );
	}

	if ( toroid && EAST(x) == graph->width - 1 && graph->maze[y][EAST(x)].k != 9 && graph->maze[y][0].k != 9) {
		neighbor = graph->width * ( y ) + ( EAST(x) );		
		Adj = enqueue ( Adj, setInt ( neighbor ) );
	}
	if ( toroid && EAST(x) > graph->width - 1 && graph->maze[y][0].k != 9 ) {
		neighbor = graph->width * ( y ) + ( 0 );		
		Adj = enqueue ( Adj, setInt ( neighbor ) );
	}	
	if ( EAST(x) < graph->width && graph->maze[y][EAST(x)].k != 9   ) {
	//if ( EAST(x) < graph->width  ) {
		neighbor = graph->width * ( y ) + ( EAST(x) );		
		Adj = enqueue ( Adj, setInt ( neighbor ) );
	}
	
	if ( toroid && SOUTH(y) == graph->height -1  && graph->maze[SOUTH(y)][x].k != 9 ) {
		neighbor = graph->width * (  SOUTH(y) ) + ( x );
		Adj = enqueue ( Adj, setInt ( neighbor ) );		
	}	
	if ( toroid && SOUTH(y) > graph->height -1  && graph->maze[(y)][x].k != 9&& graph->maze[0][x].k != 9 ) {
		neighbor = graph->width * (  0 ) + ( x );
		Adj = enqueue ( Adj, setInt ( neighbor ) );		
	}
	if ( SOUTH(y) < graph->height && graph->maze[SOUTH(y)][x].k != 9 ) {
	//if ( SOUTH(y) < graph->height ) {
		neighbor = graph->width * ( SOUTH(y) ) + ( x );		
		Adj = enqueue ( Adj, setInt ( neighbor ) );
	}
	
	if ( toroid && WEST(x) == 0 && graph->maze[y][WEST(x)].k != 9 ) {
		neighbor = graph->width * ( y ) + ( WEST(x) );		
		Adj = enqueue ( Adj, setInt ( neighbor ) );
	}

	if ( toroid && WEST(x) < 0 && graph->maze[y][(x)].k != 9 && graph->maze[y][graph->width - 1].k != 9) {
		neighbor = graph->width * ( y ) + ( graph->width - 1 );		
		Adj = enqueue ( Adj, setInt ( neighbor ) );
	}

	if ( WEST(x) > 0 && graph->maze[y][WEST(x)].k != 9 ) {
	//if ( WEST(x) > 0 ) {
		neighbor = graph->width * ( y ) + ( WEST(x) );		
		Adj = enqueue ( Adj, setInt ( neighbor ) );
	}

	return Adj;
}
/**
 * visita breadth_first_search su matrice di adiacenza
*/
int *dijkstraHeap ( GRAPHOBJ *graph, int s, int target ) {
	assert ( graph );	
	/**
	 * inizializzazioni
	 */
	Heap 	*		frontier 	= NULL;
	Set 	*		AdjList 	= NULL;
	Data 	*		tmp;
	int 	*		pred;


	FILE *stream;
	frontier = initializeHeap ( minHeapify );
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
	insert ( frontier, new_HeapData ( s, 0 ) );

	char path[50];
	buildHeap ( frontier );

	//main loop
	while ( frontier->heapsize > 1 ) {
		//estrazione del nodo con la distanza minore
		tmp 	= extractFirst ( frontier );
		u 		= getData ( tmp );
		dist[u] = getKey ( tmp );
		colore[u] = BLACK;
		AdjList = graph->getAdjList ( graph, u );
		//debug
		if ( PRINTALL ) graph->maze[getCoord ( graph, u )->y][getCoord ( graph, u )->x].path = true;
		if ( u == target ) {
		 	freeHeap ( frontier );
			return pred;			
		}
		//analisi dei nodi adiacenti
		while ( !isEmpty ( AdjList ) ) {
			v = getInt ( getFront ( AdjList ) );
			
			if ( colore[v] == WHITE ) {
				relax ( graph, frontier, u, v, dist, pred );
			}
			AdjList = dequeue ( AdjList );
		}
	}
	free ( pred );
 	freeHeap ( frontier );
	return NULL;
}
//funzione di rilassamento arco
void relax ( GRAPHOBJ *graph, Heap *frontier, int u, int v, int *dist, int *pred) {
	int alt;
	alt = dist[u] + graph->getWeight ( graph, u, v );
	if ( alt < dist[v] ) {
		dist[v] = alt;
		pred[v] = u;
		if ( heapIntSearch ( frontier , v ) ) {
			decreaseKey ( frontier, v, alt ) ;
		} else {
			insert ( frontier, new_HeapData ( v, alt ) );
		}
	}
}

//BFS
int *breadth_first_search ( GRAPHOBJ *graph, int s, int target ) {

	assert ( graph );

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

	//estrazione nodo dalla frontiera
	while ( !isEmpty ( frontier ) ) {
		u = getInt ( getFront ( frontier ) );
		AdjList = graph->getAdjList ( graph, u );

		//debug
		if ( PRINTALL ) graph->maze[getCoord ( graph, u )->y][getCoord ( graph, u )->x].path = true;

		//inserimento degli adiacenti nella frontiera
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
	free ( pred );
	return NULL;
}

//funzione euristica per A*
int heuristic ( GRAPHOBJ *graph, int s, int t ) {
	assert ( graph );
	int res;
	VCOORD *	start 		= getCoord ( graph, s );
	VCOORD *	target 		= getCoord ( graph, t );
	/*manhattan*///res = ( abs ( start->x - target->x ) + abs ( start->y - target->y ) );
	/*euclidian*/res = pow ( pow ( ( target->x - start->x ), 2 ) + pow ( ( target->y - start->y ), 2 ), 0.5 );
	free ( start );
	free ( target );
	return res;
}

//A*
int *a_star ( GRAPHOBJ *graph, int s, int t ) {
	assert ( graph );
	int i, cost;
	int current;
	bool found 				= false;
	void 	*	tmp;
	Set 	*	AdjList		= NULL;		//lista di adiacenza ( coda )
	Heap 	*	frontier	= NULL;		//Open set ( Heap )
	frontier 				= initializeHeap ( minHeapify );
	int 	*	pred		= ( int *) malloc ( graph->vNum * sizeof ( int ) );
	int 	*	g			= ( int *) malloc ( graph->vNum * sizeof ( int ) );


	//initialization
	for ( i = 0; i < graph->vNum; i++ ) {
		pred[i] 	= NIL;
		g[i] 	= INFINITE;
	}
	insert ( frontier, new_HeapData ( s, 0 ) );
	g[s] 	= 0;
	//estrazione del nodo con priorità minore dalla frontiera
	while ( !isHeapEmpty ( frontier ) ) {
		tmp 	= extractFirst ( frontier );
		current = getData ( tmp );
		//debug
		if ( PRINTALL ) graph->maze[getCoord ( graph, current )->y][getCoord ( graph, current )->x].path = true;

		if ( current == t ) {
			free ( g );
			freeHeap ( frontier );		
			return pred;
		}
		AdjList = graph->getAdjList ( graph, current );
		//estrazione da lista di adiacenza
		while ( !isEmpty ( AdjList ) ) {
			int y = getInt ( getFront ( AdjList ) );
			AdjList = dequeue ( AdjList );
			cost = g[current] + graph->getWeight ( graph, current, y );
			if ( g[y] == INFINITE || cost < g[y] ) {
				g[y] = cost;
				int priority = cost + heuristic ( graph, y, t );
				insert ( frontier, new_HeapData ( y, priority ) );
				pred[y] = current;
			}
		}
		free ( tmp );
 	}
 	free ( pred );
 	free ( g );
 	freeHeap ( frontier );
	return NULL;
}
