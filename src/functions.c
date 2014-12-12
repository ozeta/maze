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
		}else if ( mazeStr[i] == '-' ) {
			k = 9;
		}else if ( mazeStr[i] == ' ' ) {
			k = 1;
		}else if ( mazeStr[i] == 's' ) {
			k = 2;
		}else if ( mazeStr[i] == 'f' ) {
			k = 3;
		}

		maze[col][row].k = k;
		maze[col][row].ID = i;
		maze[col][row].path = false;
		
		i++;
		++graph->vNum;
	}
	printf ("vert: %d\n\n", graph->vNum );
	return maze;
}