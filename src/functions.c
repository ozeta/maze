#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "./headers/functions.h"
#include "./headers/lib.h"


GRAPHOBJ *initializeGraph ( BDEF build, IEDEF insertEdge, WDEF getWeight, 
								VINS insertVertex, TDEF transpose, ADEF getAdjList ) {

	GRAPHOBJ * 	graph = ( GRAPHOBJ * ) malloc ( sizeof ( GRAPHOBJ ) );

	if ( check ( graph, "initializeGraph" ) ) {

		graph->matrix 			= NULL;
		graph->list 			= NULL;
		graph->vNum 			= 0;
		graph->width 			= 0;
		graph->height 			= 0;
		graph->build 			= build;
		graph->insertEdge 		= insertEdge;
		graph->getWeight 		= getWeight;
		graph->insertVertex 	= insertVertex;
		graph->transpose 		= transpose;
		graph->getAdjList		= getAdjList;
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
	return ( graph->width ) * ( c->y ) + ( c->x );
}


int **buildMap ( GRAPHOBJ * graph, char *mazeStr ) {

	int width 		= graph->width;
	int height 		= graph->height;
	int size 		= width * height;
	int **maze 		= ( int ** ) malloc ( height * sizeof ( int * ) );
	int i = 0;
	for ( i = 0; i < height; i++ ) {
		maze[i] = ( int * ) malloc ( width * sizeof ( int * ) );
		if ( maze[i] == NULL ) {
			printf ("errore malloc");
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
		}else if ( mazeStr[i] == '-' ) {
			k = 9;
		}else if ( mazeStr[i] == ' ' ) {
			k = 1;
		}else if ( mazeStr[i] == 's' ) {
			k = 2;
		}else if ( mazeStr[i] == 'f' ) {
			k = 3;
		}

		maze[col][row] = k;

		
		i++;
		++graph->vNum;
	}
	printf ("vert: %d\n\n", graph->vNum );
	return maze;
}