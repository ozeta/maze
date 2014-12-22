#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "./headers/functions.h"
#include "./headers/lib.h"

#define NORTH(y) y - 1
#define SOUTH(y) y + 1
#define EAST(y) y + 1
#define WEST(y) y - 1

void dialogPanel ( char *message ) {
	printf ( "%s", message );
	printf ("\n");
}
GRAPHOBJ *initializeGraph ( BDEF build, IEDEF insertEdge, WDEF getWeight, 
								VINS insertVertex, TDEF transpose, ADEF getAdjList, PATH path ) {

	GRAPHOBJ * 	graph = ( GRAPHOBJ * ) malloc ( sizeof ( GRAPHOBJ ) );

	if ( check ( graph, "initializeGraph" ) ) {

		graph->maze 			= NULL;
		graph->vNum 			= 0;
		graph->width 			= 0;
		graph->height 			= 0;
		graph->build 			= build;
		graph->insertEdge 		= insertEdge;
		graph->getWeight 		= getWeight;
		graph->insertVertex 	= insertVertex;
		graph->transpose 		= transpose;
		graph->getAdjList		= getAdjList;
		graph->path				= path;
	}

	return graph;
}

GraphT matrixEdgeInizialize () {
	GraphT edge;
	edge.weight = 0;
	edge.ID 	= -1;
	edge.k 		= -1;	
	return edge;
}
/**
//input: numero di vertici da allocare
//return: nuovo puntatore a grafo di vertexNum vertici
*/
GRAPHOBJ *buildGraph ( GRAPHOBJ *graphObj, int size ) {
	int i, j;
	char msg[256];
	GraphT **graph = NULL; 
	graph = ( GraphT ** ) malloc ( ( size ) * sizeof ( GraphT * ) );
	if (graph == NULL) {
		dialogPanel ("memoria insufficiente: graph"), exit(EXIT_FAILURE);
	}
	for ( i = 0; i < size; i++) {
		graph[i] = (GraphT *) malloc (size * sizeof (GraphT));
		if (graph[i] == NULL){
			sprintf ( msg, "memoria insufficiente: graph[%d]", i );
			dialogPanel ( msg ), exit(EXIT_FAILURE);
		}
		for ( j = 0; j < size; j++) {
		graph[i][j] = matrixEdgeInizialize ();
		}
	} 

	graphObj->matrix = graph;
	
	return graphObj;
}

void matrixInsertInterface ( GRAPHOBJ *graph ) {
	if ( !check ( graph, "insert interface" ) )	return;
	graph->matrix = appendOneVertexIntoGraph ( graph );
}
/**
//input: numero corrente di vertici, puntatore ad graph
//return: nuovo puntatore ad graph di dimensione n+1
*/
GraphT **appendOneVertexIntoGraph ( GRAPHOBJ *graph ) {

	//aggiorno il numero di vertici
	++graph->vNum;
	char msg[256];
 	int i = 0;

	//rialloco l'array di puntatori aggiungendo il posto per
	//il nuovo vertice
	graph->matrix = ( GraphT ** ) realloc ( graph->matrix, ( graph->vNum ) * sizeof ( GraphT * ) );
	if (graph->matrix == NULL)
		dialogPanel ( ">realloc: memoria insufficiente" ), exit ( EXIT_FAILURE );
	//alloco memoria per il nuovo array di archi
	graph->matrix[graph->vNum-1] = ( GraphT * ) malloc (graph->vNum  * sizeof ( GraphT ) );
	if  ( graph->matrix[graph->vNum-1] == NULL ) {
		sprintf ( msg, ">realloc: memoria insufficiente: graph->matrix[%d]", graph->vNum -1 );
			dialogPanel  ( msg ), exit ( EXIT_FAILURE );
	}
	//rialloco memoria per tutti gli array di archi ad eccezione di "vertexnum-1"
	for  (  i = 0; i < graph->vNum; i++ ) {
		if  ( i != graph->vNum-1 ) {
			graph->matrix[i] = ( GraphT * ) realloc  ( graph->matrix[i], ( graph->vNum ) * sizeof  ( GraphT ) );
			if  ( graph->matrix[i] == NULL ){
				sprintf ( msg, ">realloc: memoria insufficiente: graph->matrix[%d]", i );
				dialogPanel  ( msg ), exit ( EXIT_FAILURE );
			}
		}
		//inizializzo il nuovo array di archi e i nuovi archi per ogni nodo
		graph->matrix[graph->vNum-1][i] = matrixEdgeInizialize ();
		graph->matrix[i][graph->vNum-1] = matrixEdgeInizialize ();
	}

    return graph->matrix;
}
/*estrae il peso da un arco*/
int getMatrixWeight ( GRAPHOBJ *graph, int x, int y ) {
	int weight = 0;

	if ( graph && graph->matrix ) {
		weight = graph->matrix[x][y].weight;
	}
	return weight;
}

//procedura che modifica il peso di un arco
void insertEdgeIntograph ( GRAPHOBJ *graph, int x, int y, int weight ) {
	if ( graph ) {
			if ( x < graph->vNum && y < graph->vNum ) {
				graph->matrix[x][y].weight = weight;
			} else {
				printf ("coordinata errata\n");
			}
		} else {
		dialogPanel ("grafo non esistente\n");
		}
}

//elimina arco nel grafo
void deleteEdgeFromgraph ( GRAPHOBJ *graph, int x, int y ) {
	if ( graph ) {
		if ( x < graph->vNum && y < graph->vNum )
			graph->matrix[x][y].weight = 0;
	} else {
		dialogPanel ("grafo non esistente");
	}
}
void FillGraph ( GRAPHOBJ *graph, insDEF insertMethod, int inf, int sup) {
	if ( !check ( graph , "FillGraph" ) ) {
		return;
	}	
	srand (time(NULL));
	int i, j;
	for ( i = 0; i < graph->vNum; i++ ) {
		for ( j = 0; j < graph->vNum; j++ ) {
			graph->insertEdge ( graph, i, j, insertMethod (inf, sup) );
		}
	}
}


/**********************************
*IMPLICIT GRID MAZE SECTION
************************************/
VCOORD *getCoord ( GRAPHOBJ *graph, int u ) {
	VCOORD *vCoord = ( VCOORD * ) malloc ( sizeof ( VCOORD ) );
	int x = u % graph->width;
	int y = u / graph->width;
	vCoord->x = x;
	vCoord->y = y;

	return vCoord;
}

int coordToID ( GRAPHOBJ *graph, VCOORD *c ) {
	int res = ( graph->width ) * ( c->y ) + ( c->x );
	//printf ( "graph->width:%4d, c->y:%4d, c->x:%4d, res:%4d\n", graph->width, c->y, c->x, res );
	return res;
}

/**
stampa del cammino minimo
*/
Set *printPath ( GRAPHOBJ *graph, int s , int v, int *pred, Set *succ, FILE *stream ) {

    if (v == s) {
 		if ( stream )
 			fprintf ( stream, "s: %d\n", s  );
 		graph->maze[getCoord ( graph, s )->y][getCoord ( graph, s )->x].path = true;
 		succ = enqueue ( succ, setInt ( s ) );
    } else if ( pred[v] == NIL ) {
    	if ( stream )
			fprintf ( stream, "\n--non esiste cammino tra s e v--\n");
    } else {
    	if ( stream )
			fprintf ( stream, "v: %d\n", v );
		succ = enqueue ( succ, setInt ( v ) );

		graph->maze[getCoord ( graph, v )->y][getCoord ( graph, v )->x].path = true;
		printPath (graph, s, pred[v], pred, succ, stream );
    }
    return succ;
}
Set *explicitPrintPath ( GRAPHOBJ *graph, int s , int v, int *pred, Set *succ, FILE *stream ) {

    if (v == s) {
 		if ( stream )
 			fprintf ( stream, "s: %d\n", s  );
 		succ = enqueue ( succ, setInt ( s ) );
    } else if ( pred[v] == NIL ) {
    	if ( stream )
			fprintf ( stream, "\n--non esiste cammino tra s e v--\n");
    } else {
    	if ( stream )
			fprintf ( stream, "v: %d\n", v );
		succ = enqueue ( succ, setInt ( v ) );
		explicitPrintPath (graph, s, pred[v], pred, succ, stream );
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
Set *minPath ( GRAPHOBJ *graph, int s, int v ) {

	int 		*	pred 	= NULL;
	Set 		*	succ 	= NULL;
	Set 		*	def 	= NULL;

	pred = graph->path ( graph, s, v );
	if ( pred != NULL ) {
	//if ( 1 ) {

		FILE *stream = NULL;
		//stream = openStream ( "pred.txt", "w+" );
		succ = printPath ( graph, s, v, pred, succ, stream );
		//printAllpreds ( graph, pred );
		//closeStream ( stream );
	} else { 
		printf ("\n--target not found--\n");
	}
	//stampa tutti i nodi visitati
	//FILE *stream1 = openStream ( "def.txt", "w+" );
	//printSet ( def, stream1 );
	//closeStream ( stream1 );
	free ( pred );
	return succ;
}
Set *getAdjList ( GRAPHOBJ *graph, int u ) {
	Set *res = NULL;
	int weight;
	int i;
	for (i = 0; i < graph->vNum ; i++) {
		weight = graph->getWeight ( graph, u, i );
		if ( weight != 0 && weight != INT_MIN ) {
			res = enqueue (res, setInt ( i ) );
		}
	}
	return res;
}
void cPrintMaze ( GRAPHOBJ *graph, Set * succ, int s, int v ) {

	int i, j, k;
	VCOORD *cur;

	for ( i = 0; i < graph->height; i++ ) {
		for ( j = 0; j < graph->width; j++ ) {
			

			if ( i*graph->width+j == s ||  i*graph->width+j == v ) {

				printf (ANSI_COLOR_RED"%d"ANSI_COLOR_RESET, graph->maze[i][j].k);

			} else 

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

GRAPHOBJ *buildExplicitGraph ( GRAPHOBJ *graph, GRAPHOBJ *newGraph, char *mazeStr ) {
	bool DEBUG 			= false;
	int width 			= graph->width;
	int height 			= graph->height;
	int size 			= graph->vNum;	
	newGraph->width 	= graph->width;
	newGraph->height 	= graph->height;

	int y, x, ID, adjID = -1;

	newGraph->vNum = graph->vNum;
	newGraph = buildGraph ( newGraph, newGraph->vNum );
	
	for ( y = 0; y < graph->height; y++ ) {
		for ( x = 0; x < graph->width; x++ ) {
			if ( graph->maze[y][x].k != 9 ) {
				ID = ( graph->width ) * ( y ) + ( x );

				if ( DEBUG ) printf ("ID: %4d\n", ID);

				if ( NORTH(y) > 0 && graph->maze[NORTH(y)][x].k != 9 ) {
					adjID = ( graph->width ) * ( NORTH(y) ) + ( x );
					newGraph->insertEdge ( newGraph, ID, adjID, 1 );
					if ( DEBUG ) printf ("adiacente Nord: %4d\n", adjID );
				}
				if ( EAST(x) < graph->width && graph->maze[y][EAST(x)].k != 9  ) {
					adjID = ( graph->width ) * ( y ) + ( EAST(x) );
					newGraph->insertEdge ( newGraph, ID, adjID, 1 );
					if ( DEBUG ) printf ("adiacente Est: %4d\n", adjID );
				}
				if ( SOUTH(y) < graph->height && graph->maze[SOUTH(y)][x].k != 9 ) {
					adjID = ( graph->width ) * ( SOUTH(y) ) + ( x );
					newGraph->insertEdge ( newGraph, ID, adjID, 1 );	
					if ( DEBUG ) printf ("adiacente Sud: %4d\n", adjID );
				}
				if ( WEST(x) > 0 && graph->maze[y][WEST(x)].k != 9 ) {
					adjID = ( graph->width ) * ( y ) + ( WEST(x) );
					newGraph->insertEdge ( newGraph, ID, adjID, 1 );
					if ( DEBUG ) printf ("adiacente Ovest: %4d\n", adjID );
				}			
			}
		}
	}
	return newGraph;
}
VCOORD **buildMap ( GRAPHOBJ * graph, char *mazeStr ) {

	int width 		= graph->width;
	int height 		= graph->height;
	int size 		= width * height;

	VCOORD **maze 	= ( VCOORD ** ) malloc ( height * sizeof ( VCOORD * ) );
	if ( maze == NULL ) {
		printf ("errore maze **\n");
		exit ( -1 );
	}
	int i;
	for ( i = 0; i < height; i++ ) {
		maze[i] = ( VCOORD * ) malloc ( width * sizeof ( VCOORD ) );
		if ( maze[i] == NULL ) {
			printf ( "width %d\n", graph->width );
			printf ( "errore maze * %d\n", i );
			exit ( -1 );
		}
	}
	int row;
	int col;
	int k;
	i = 0;
	while ( i < size ) {
		row = i % width;
		col = i / width;
		if ( mazeStr[i] == '|' ) {
			k = 9;
		} else if ( mazeStr[i] == '-' ) {
			k = 9;
		} else if ( mazeStr[i] == '+' ) {
			k = 9;
		} else if ( mazeStr[i] == ' ' ) {
			k = 1;
		} else if ( mazeStr[i] == 's' ) {
			k = 2;
		} else if ( mazeStr[i] == 't' ) {
			k = 3;
		} else if ( mazeStr[i] == 'f' ) {
			k = 3;
		}

		maze[col][row].k = k;
		maze[col][row].ID = i;
		maze[col][row].path = false;
		
		i++;
		++graph->vNum;
	}
	return maze;
}

