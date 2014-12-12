#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./src/headers/structures.h"
#include "./src/headers/menu.h"
#include "./src/headers/parser.h"
#include "./src/headers/functions.h"
#include "./src/headers/pathfind.h"


int main () {

	char *mazeStr;
	GRAPHOBJ * graph;
	graph = initializeGraph ( NULL, NULL, NULL, NULL, NULL, getAdjList );
	mazeStr = mazeToString ( graph, "./maps/implicit1.txt" );

	int i, j;
	//printf ( "%s", mazeStr );

	VCOORD start;
	VCOORD target;
	graph->maze = buildMap ( graph, mazeStr );

	for ( i = 0; i < graph->height; i++ ) {
		for ( j = 0; j < graph->width; j++ ) {
			
			if ( graph->maze[i][j].k == 2 ) {
				start.x = j;
				start.y = i;
				printf (ANSI_COLOR_RED"%d"ANSI_COLOR_RESET, graph->maze[i][j].k);
			} else if ( graph->maze[i][j].k == 3 ) {
				target.x = j;
				target.y = i;
				printf (ANSI_COLOR_RED"%d"ANSI_COLOR_RESET, graph->maze[i][j].k);

			} else {
				printf ("%d", graph->maze[i][j].k);
			}
		}
	printf ( "\n" );
	}
	printf ( "start: (%d, %d)\n", start.x, start.y );
	coordToID ( graph, &start );
	printf ( "target: (%d, %d)\n", target.x, target.y );
	coordToID( graph, &target );
	printf ( "\n" );
	
	//minPath ( graph, (69*1)+1, (69*27)+67 );
	minPath ( graph, coordToID ( graph, &start ) , coordToID( graph, &target ) );
	return 0;
}
