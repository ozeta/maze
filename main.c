#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "./src/headers/structures.h"
#include "./src/headers/menu.h"
#include "./src/headers/parser.h"
#include "./src/headers/functions.h"
#include "./src/headers/pathfind.h"

bool CLOCKT = true;

void pathfind ( GRAPHOBJ *graph, VCOORD *start, VCOORD *target, PATH alg ) {
	clock_t start_t, end_t, total_t;

	graph->path = alg;
	start_t = clock();

	minPath ( graph, coordToID ( graph, start ) , coordToID( graph, target ) );

	if ( CLOCKT ) {
		end_t = clock();
		printf("End of the big loop, end_t = %ld\n", end_t);
		total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
		printf("Total time taken by CPU: %f\n", ( double ) total_t  );	
		printf ( "\n" );
	}
	printf ( "\n" );

}
void intializePaths ( GRAPHOBJ *graph, VCOORD *start, VCOORD *target ) {
	int i, j;
	bool DEBUG = false;

	for ( i = 0; i < graph->height; i++ ) {
		for ( j = 0; j < graph->width; j++ ) {
			
			if ( graph->maze[i][j].k == 2 ) {
				start->x = j;
				start->y = i;
				if ( DEBUG ) {	
					printf (ANSI_COLOR_RED"%d"ANSI_COLOR_RESET, graph->maze[i][j].k);
				}
			} else if ( graph->maze[i][j].k == 3 ) {
				target->x = j;
				target->y = i;
				if ( DEBUG ) {	
					printf (ANSI_COLOR_RED"%d"ANSI_COLOR_RESET, graph->maze[i][j].k);
				}
			} else {
				if ( DEBUG ) {
					printf ("%d", graph->maze[i][j].k);
				}
			}
		}
		if ( DEBUG ) {		
			printf ( "\n" );
		}
	}
}
int main ( int argc, char **argv ) {

	int l = 0;
	char *str[] = {	"./maps/implicit0.txt",
					"./maps/implicit1.txt",
					"./maps/implicit2.txt",
					"./maps/implicit3.txt",
					"./maps/implicit4.txt",
					"./maps/implicit5.txt",
					"./maps/implicit6.txt",
					"./maps/implicit7.txt" };
	if ( argc == 2 ) {
		l = atoi(argv[1]);
	}
	clock_t start_t, end_t, total_t;
	char *mazeStr;
	GRAPHOBJ * graph;
	graph = initializeGraph ( NULL, NULL, getMatrixWeight, NULL, NULL, getAdjList, NULL );
	mazeStr = mazeToString ( graph, str[l] );

	int i, j;
	//printf ( "%s", mazeStr );

	VCOORD start;
	start.x = start.y = -1;
	VCOORD target;
	target.x = target.y = -1;
	
	graph->maze = buildMap ( graph, mazeStr );


	intializePaths ( graph, &start, &target );		
	
	VCOORD target1;
	target1.x = 50;
	target1.y = 25;
	VCOORD target2;
	target2.x = start.x + 5;
	target2.y = start.y;

	
		printf ( "start: %d (%d, %d)\n", 
			coordToID ( graph, &start ), start.x, start.y );
		coordToID ( graph, &start );
		printf ( "target: %d (%d, %d)\n", 
			coordToID ( graph, &target ), target.x, target.y );
		coordToID( graph, &target );
		printf ( "\n" );
	
	//pathfind ( graph, &start, &target, breadth_first_search );
	//pathfind ( graph, &start, &target, dijkstraHeap );
	pathfind ( graph, &start, &target, a_star_bene );


	return 0;
}
