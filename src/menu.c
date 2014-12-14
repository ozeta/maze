#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "./headers/functions.h"
#include "./headers/menu.h"
#include "./headers/parser.h"

#include "./headers/lib.h"

void menuSwitch ( GRAPHOBJ *graph, int command, int maxsize ) {

	
	GRAPHOBJ 	* 		newgraph 	= NULL;
	void 		*		result 		= NULL;
	char 		* 		str;
	int 	s, v;
	int 	x, y, weight;
	int 	res 			= 0;
	int 	check 			= -1;
	int 	dSizeY 			= 20;
	int 	dSizeX 			= 50;
		switch ( command ) {
			case 0: 
				//stampa grafo

				break;			
			case 1: 
				//leggi da file
				//printf ( "\nnome file:\n" );
				//string = getString ( );
				//check = parseFile ( graph, "output.txt" );

				break;
			case 2:
				//stampa su file
				//printf ( "\nnome file:\n" );
				//scanf ( "%s", string );		
				check = writeOnFile ( graph, "output.txt" );

				break;
			case 3: 
				//new random matrix;

				break;			
			case 4:
				//new vertex

				break;
			case 5:
				//new edge

				break;
			case 6:
				//delete vertex
				break;
			case 7:
				//delete edge
				break;
			case 8:
				//transpose
				break;
			case 9:
				//bfs
				break;
			case 10:
				//dijkstra
				break;					
			case 11:
				//dfs, analisi e ricerca cicli
				break;
			case 12:
				//convert
				break;
						
			default:
			break;

		}

} 
