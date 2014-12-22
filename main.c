#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "./src/headers/structures.h"
#include "./src/headers/menu.h"
#include "./src/headers/parser.h"
#include "./src/headers/functions.h"
#include "./src/headers/pathfind.h"
#include "./src/headers/lib.h"
#include "./src/headers/set.h"

bool CLOCKT = false;

void pathfind ( GRAPHOBJ *graph, VCOORD *start, VCOORD *target, PATH alg ) {
	clock_t start_t, end_t, total_t;

	graph->path = alg;
	start_t = clock();

	Set *succ = minPath ( graph, coordToID ( graph, start ) , coordToID( graph, target ) );
	cPrintMaze ( graph, succ, coordToID ( graph, start ), coordToID( graph, target ) );
	if ( CLOCKT ) {
		end_t = clock();
		printf("End of the big loop, end_t = %ld\n", end_t);
		total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
		printf("Total time taken by CPU: %f\n", ( double ) total_t  );	
		printf ( "\n" );
	}
	printf ( "\n" );

}
void pathfindExplicit ( GRAPHOBJ *graph, VCOORD *start, VCOORD *target, PATH alg ) {


	int 		*	pred 	= NULL;
	Set 		*	succ 	= NULL;
	Set 		*	def 	= NULL;
	char		*	string 	= "pred.txt";
	graph->path = alg;
	pred = graph->path ( graph, coordToID ( graph, start ), 
						coordToID ( graph, target ) );

	if ( pred != NULL ) {
		printf ("array of prev vertexes printed on %s\n", string );
		FILE *stream = NULL;
		stream = openStream ( string, "w+" );
		succ = explicitPrintPath ( graph, coordToID ( graph, start ), 
				coordToID ( graph, target ), pred, succ, stream );
		closeStream ( stream );
	} else { 
		printf ("\n--target not found--\n");
	}
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
		if ( l < 0 || l > 7 ) {
		printf ( "usage: ./labirinto n\n");
		printf ( "n is a value from 0 to 7 \n");
		exit ( 0 );			
		}
	} else {
		printf ( "usage: ./labirinto n\n");
		printf ( "n is a value from 0 to 7 \n");
		exit ( 0 );
	}

	GRAPHOBJ *graph  = initializeGraph ( buildGraph, insertEdgeIntograph, 
		getMatrixWeight, matrixInsertInterface, 
		NULL, getAdjList, NULL );
	GRAPHOBJ *newGraph  = initializeGraph ( buildGraph, insertEdgeIntograph, 
		getMatrixWeight, matrixInsertInterface, 
		NULL, getAdjList, NULL );

	char *mazeStr;
	mazeStr = mazeToString ( graph, str[l] );

	int i, j;

	VCOORD start;
	start.x = start.y = -1;
	VCOORD target;
	target.x = target.y = -1;
	
	//implicit map callbacks
	graph->getWeight 	= getMazeWeight;
	graph->getAdjList 	= getMazeAdjList;
	//explicit map callbacks
	newGraph->getWeight 	= getMatrixWeight;
	newGraph->getAdjList 	= getAdjList;	
	graph->maze 			= buildMap ( graph, mazeStr );
	newGraph				= buildExplicitGraph ( graph, newGraph, mazeStr ); 

	FILE *stream = openStream ( "adj4.txt", "w+");
	for ( i = 0; i < newGraph->vNum; i++) {
		for ( j = 0; j < newGraph->vNum; j++) {
			fprintf (stream, "%d", newGraph->matrix[i][j].weight );
		}
		fprintf (stream, "\n");
	}
	closeStream ( stream );
	printf ( "numero vertici mappa: %4d\n", graph->vNum );

	intializePaths ( graph, &start, &target );		

	if ( start.y == -1 || start.x == -1 ) {
		printf ("\n--start 's' must be on the map--\n");
		exit ( -1 );
	}
	
	if ( target.y == -1 || target.x == -1 ) {
		printf ("\n--target 't' must be on the map--\n");
		exit ( -1 );
	}
	
		printf ( "start: %4d (x:%4d,y:%4d)\n", 
			coordToID ( graph, &start ), start.x, start.y );
		coordToID ( graph, &start );
		printf ( "target: %d (x:%4d,y:%4d)\n", 
			coordToID ( graph, &target ), target.x, target.y );
		coordToID( graph, &target );
		
		printf ( "\n" );
	/*
	pathfind ( graph, &start, &target, a_star );
	pathfind ( graph, &start, &target, dijkstraHeap );
*/
	pathfindExplicit ( newGraph, &start, &target, breadth_first_search );

	return 0;
}
