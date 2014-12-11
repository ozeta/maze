#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./src/headers/structures.h"
#include "./src/headers/menu.h"
#include "./src/headers/parser.h"
#include "./src/headers/functions.h"
#include "./src/headers/pathfind.h"


int main () {

	int width, height;
	char *mazeStr;
	GRAPHOBJ * graph;
	graph = initializeGraph ( NULL, NULL, NULL, NULL, NULL, getAdjList );
	mazeStr = mazeToString ( "./maps/implicit1.txt", &width, &height );
	graph->width = width;
	graph->height = height;
	int i, j;
	//printf ( "%s", mazeStr );

	VCOORD start;
	VCOORD target;
	int **maze = buildMap ( graph, mazeStr );
	graph->maze = maze;
	
	for ( i = 0; i < height; i++ ) {
		for ( j = 0; j < width; j++ ) {
			
			if ( maze[i][j] == 2 ) {
				start.x = i;
				start.y = j;
				printf (ANSI_COLOR_RED"%d"ANSI_COLOR_RESET, graph->maze[i][j]);
			} else if ( maze[i][j] == 3 ) {
				target.x = i;
				target.y = j;
				printf (ANSI_COLOR_RED"%d"ANSI_COLOR_RESET, graph->maze[i][j]);

			} else {
				printf ("%d", maze[i][j]);
			}
		}
	printf ( "\n" );
	}
	printf ( "start: (%d, %d)\n", start.x, start.y );
	printf ( "target: (%d, %d)\n", target.x, target.y );
	printf ( "\n" );
	
	minPath ( graph, (69*1)+1, (69*27)+67 );
	//minPath ( graph, coordToID ( graph, &start ) , coordToID( graph, &target ) );
	return 0;
}
