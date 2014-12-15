#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "./headers/functions.h"
#include "./headers/lib.h"


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


VCOORD **buildMap ( GRAPHOBJ * graph, char *mazeStr ) {

	int width 		= graph->width;
	int height 		= graph->height;
	int size 		= width * height;
	VCOORD **maze 		= ( VCOORD ** ) malloc ( height * sizeof ( VCOORD * ) );
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
	//printf ("vert: %d\n\n", graph->vNum );
	return maze;
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
	if ( pred != NULL ) {
	//if ( 1 ) {

		FILE *stream = NULL;
		//stream = openStream ( "pred.txt", "w+" );
		succ = printPath ( graph, s, v, pred, succ, stream );
		//closeStream ( stream );
		cPrintMaze ( graph, succ, s, v );
	} else { 
		printf ("pred nullo\n");
	}
	int i,j;
	int k = 0;
		//printAllpreds ( graph, pred );
	//FILE *stream1 = openStream ( "def.txt", "w+" );
	//printSet ( def, stream1 );
	//closeStream ( stream1 );
	free ( pred );

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
