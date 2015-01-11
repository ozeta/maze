
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "./src/headers/structures.h"
#include "./src/headers/parser.h"
#include "./src/headers/functions.h"
#include "./src/headers/pathfind.h"
#include "./src/headers/lib.h"
#include "./src/headers/set.h"

//ricerca percorso e stampa su mappa implicita
void pathfind ( GRAPHOBJ *graph, VCOORD *start, VCOORD *target, PATH alg );
//ricerca percorso e stampa su mappa esplicita
void pathfindExplicit ( GRAPHOBJ *graph, VCOORD *start, VCOORD *target, PATH alg );
//inizializzazione oggetti start e target
void intializePaths ( GRAPHOBJ *graph, VCOORD *start, VCOORD *target );
//procedura di ricerca su mappa implicita
void implicit ( char *map, char *alg );
//procedura di ricerca su mappa esplicita
void explicit ( char *map, char *alg );

int main ( int argc, char **argv ) {

	int l = 0;

	char *help[] = {"usage: ./labirinto n as|bf|di [explicit]\n",
					"\tn is a value from 0 to 7",
					"\tas: astar path finder",
					"\tbf: bfs path finder",
					"\tdi: dijkstra path finder",
					"\texplicit: uses the explicit map instead of the implicit by default\n",
					"\texample: ./labirinto 4 as explicit\n" };

	char *str[] = {	"./maps/implicit0.txt",
					"./maps/implicit1.txt",
					"./maps/implicit2.txt",
					"./maps/implicit3.txt",
					"./maps/implicit4.txt",
					"./maps/implicit5.txt",
					"./maps/implicit6.txt",
					"./maps/implicit7.txt" };

	char test[200];


	switch ( argc ) {
		case 0:
		case 1:
		case 2:
		for ( l = 0; l < 7; l++ ) {
			printf ("%s\n", help[l]);
		}
			exit ( 0 );
			break;
		case 3:
			//check input: numero mappa
			l = atoi(argv[1]);
			if ( l < 0 || l > 7 ) {
				for ( l = 0; l < 7; l++ ) {
					printf ("%s\n", help[l]);
				}
				exit ( 0 );
			} else {
				//check input: algoritmo di ricerca
				if ( strcmp ( argv[2], "as" ) == 0 ||
					 strcmp ( argv[2], "bf" ) == 0 ||
					 strcmp ( argv[2], "di" ) == 0 ) {
					implicit ( str[l], argv[2] );
				} else {
					for ( l = 0; l < 7; l++ ) {
						printf ("%s\n", help[l]);
					}
					exit ( 0 );					
				}
			}			
			break;
		case 4:
		//check input: mappa esplicita
		if ( strcmp ( argv[3], "explicit" ) == 0 ) {
			explicit ( str[l], argv[2] );
		} else {
			for ( l = 0; l < 7; l++ ) {
				printf ("%s\n", help[l]);
			}
			exit ( 0 );		
		}
		break;
		default:
			for ( l = 0; l < 7; l++ ) {
				printf ("%s\n", help[l]);
			}
			exit ( 0 );			
			break;
	}
	return 0;
}


void pathfind ( GRAPHOBJ *graph, VCOORD *start, VCOORD *target, PATH alg ) {
	//alg: chiamata a funzione di ricerca percorso minimo ( A*, Dijkstra, BFS )
	graph->path = alg;
	//ricerca del percorso minimo. restituisce una coda di coordinate
	Set *succ = minPath ( graph, coordToID ( graph, start ) , coordToID( graph, target ) );
	//stampa la mappa con il percorso
	cPrintMaze ( graph, succ, coordToID ( graph, start ), coordToID( graph, target ) );

	printf ( "\n" );

}

void pathfindExplicit ( GRAPHOBJ *graph, VCOORD *start, VCOORD *target, PATH alg ) {
	int 		*	pred 	= NULL;
	Set 		*	succ 	= NULL;
	Set 		*	def 	= NULL;
	char		*	string 	= "pred.txt";
	//alg: chiamata a funzione di ricerca percorso minimo ( A*, Dijkstra, BFS )
	graph->path = alg;
	//ricerca del percorso minimo. restituisce l'array dei nodi precedenti
	pred = graph->path ( graph, coordToID ( graph, start ), 
						coordToID ( graph, target ) );

	//se l'array non è vuoto stampa il percorso su file
	if ( pred != NULL ) {
		printf ("array of prev vertexes printed on %s\n", string );
		FILE *stream = NULL;
		stream = openStream ( string, "w+" );
		//stampa percorso
		succ = explicitPrintPath ( graph, coordToID ( graph, start ), 
				coordToID ( graph, target ), pred, succ, stream );
		closeStream ( stream );
	} else { 
		printf ("\n--target not found--\n");
	}
}
//inizializza la mappa cercando  le coordinate del vertice Start e del Target sul grafo
void intializePaths ( GRAPHOBJ *graph, VCOORD *start, VCOORD *target ) {
	int i, j;
	bool DEBUG = false;

	for ( i = 0; i < graph->height; i++ ) {
		for ( j = 0; j < graph->width; j++ ) {
			//ricerca start
			if ( graph->maze[i][j].k == 2 ) {
				start->x = j;
				start->y = i;
				if ( DEBUG ) {	
					printf (ANSI_COLOR_RED"%d"ANSI_COLOR_RESET, graph->maze[i][j].k);
				}
			//ricerca target
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

//procedura di ricerca e stampa, versione mappa implicita
void implicit ( char *map, char *alg ) {
	GRAPHOBJ *graph  = initializeGraph ( buildGraph, insertEdgeIntograph, 
		getMatrixWeight, matrixInsertInterface, 
		NULL, getAdjList, NULL );
		char *mazeStr;
		//lettura mappa da file
		mazeStr = mazeToString ( graph, map );	

	int i, j;

	//inizializzazione Start e Target
	VCOORD start;
	start.x = start.y = -1;
	VCOORD target;
	target.x = target.y = -1;
	
	//implicit map callbacks
	graph->getWeight 	= getMazeWeight;
	graph->getAdjList 	= getMazeAdjList;
	//creazione mappa		
	graph->maze 		= buildMap ( graph, mazeStr );	
	//inizializzazione mappa
	intializePaths ( graph, &start, &target );		

	if ( start.y == -1 || start.x == -1 ) {
		printf ("\n--start 's' must be on the map--\n");
		exit ( -1 );
	}
	
	if ( target.y == -1 || target.x == -1 ) {
		printf ("\n--target 't' must be on the map--\n");
		exit ( -1 );
	}
	
	//stampa coordinate source e target
	printf ( "start: %4d (x:%4d,y:%4d)\n", 
		coordToID ( graph, &start ), start.x, start.y );
	coordToID ( graph, &start );
	printf ( "target: %d (x:%4d,y:%4d)\n", 
		coordToID ( graph, &target ), target.x, target.y );
	coordToID( graph, &target );
	
	printf ( "\n" );

	//ricerca e stampa
	if ( strcmp ( alg, "as" ) == 0 ) {
		pathfind ( graph, &start, &target, a_star );
	}
	if ( strcmp ( alg, "bf" ) == 0 ) {
		pathfind ( graph, &start, &target, breadth_first_search );
	}
	if ( strcmp ( alg, "di" ) == 0 ) {
		pathfind ( graph, &start, &target, dijkstraHeap );
	}	

}

void explicit ( char *map, char *alg ) {
	GRAPHOBJ *graph  = initializeGraph ( buildGraph, insertEdgeIntograph, 
		getMatrixWeight, matrixInsertInterface, 
		NULL, getAdjList, NULL );

	GRAPHOBJ *expMap  = initializeGraph ( buildGraph, insertEdgeIntograph, 
		getMatrixWeight, matrixInsertInterface, 
		NULL, getAdjList, NULL );
	
	char *mazeStr;
	mazeStr = mazeToString ( graph, map );	

	int i, j;

	VCOORD start;
	start.x = start.y = -1;
	VCOORD target;
	target.x = target.y = -1;

	//implicit map callbacks
	graph->getWeight 	= getMazeWeight;
	graph->getAdjList 	= getMazeAdjList;	
	//explicit map callbacks
	expMap->getWeight 	= getMatrixWeight;
	expMap->getAdjList 	= getAdjList;
	//creazione mappa implicita
	graph->maze 		= buildMap ( graph, mazeStr );
	//conversione mappa implicita in esplicita
	expMap				= convertToExplicitMap ( graph, expMap, mazeStr ); 

	//stampa su file mappa esplicita
	printf ("\nexplicit map printed on explicit.txt\n\n");
	FILE *stream = openStream ( "explicit.txt", "w+");
	for ( i = 0; i < expMap->vNum; i++) {
		for ( j = 0; j < expMap->vNum; j++) {
			fprintf (stream, "%d", expMap->matrix[i][j].weight );
		}
		fprintf (stream, "\n");
	}
	closeStream ( stream );

	//inizializzazione mappa
	intializePaths ( graph, &start, &target );		

	if ( start.y == -1 || start.x == -1 ) {
		printf ("\n--start 's' must be on the map--\n");
		exit ( -1 );
	}
	
	if ( target.y == -1 || target.x == -1 ) {
		printf ("\n--target 't' must be on the map--\n");
		exit ( -1 );
	}
	
	//stampa coordinate source e target
	printf ( "start: %4d (x:%4d,y:%4d)\n", 
		coordToID ( graph, &start ), start.x, start.y );
	coordToID ( graph, &start );
	printf ( "target: %d (x:%4d,y:%4d)\n", 
		coordToID ( graph, &target ), target.x, target.y );
	coordToID( graph, &target );
	
	printf ( "\n" );

	//ricerca e stampa
	if ( strcmp ( alg, "as" ) == 0 ) {
		pathfindExplicit ( expMap, &start, &target, a_star );
	}
	if ( strcmp ( alg, "bf" ) == 0 ) {
		pathfindExplicit ( expMap, &start, &target, breadth_first_search );
	}
	if ( strcmp ( alg, "di" ) == 0 ) {
		pathfindExplicit ( expMap, &start, &target, dijkstraHeap );
	}	
			
}
