#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "./headers/structures.h"
#include "./headers/parser.h"
#include "./headers/lib.h"
#include "./headers/functions.h"
#include "./headers/set.h"
void sintax_error ( const char *string ) {
	char mess[256];
	memset ( mess, 0, 256 );
	sprintf ( mess, "errore-> %s", string );
	//dialogPanel ( mess );
}

bool isArrayFull ( int i, int size ) {
	if ( i == size )
		return true;
	else
		return false;
}

char *fileToString ( char *stream ) {

	char 		c 			= '\0';
	int 		size 		= 4;
	int 		i 			= 0;
	char *		buffer 		= ( char * ) malloc ( size * sizeof ( char ) );
	FILE * 		pFile 		= openStream ( stream, "r" );
	
	memset ( buffer, '\0', size );
	
	if ( !check ( pFile , "fileToString" ) ) {
		return NULL;
	}

	c = fgetc ( pFile );
	while ( c != EOF ) {
		if ( isArrayFull ( i, size ) ) {
			size = 2 * size + 1;
			buffer = ( char *) realloc ( buffer, size );				
		}
		buffer[i] = c;
		i++;
		buffer[i] = '\0';			
		c = fgetc ( pFile );
	}
	closeStream ( pFile );

	return buffer;
}
int writeOnFile ( GRAPHOBJ *graph, char *stream ) {

	FILE * pFile;

	int i, j;


	if ( !check ( graph , "writeOnFile" ) ) {
		return 0;
	}
	pFile = openStream ( stream, "w" );
	if ( !check ( pFile , "writeOnFile: stream" ) ) {
		return 0;
	}
	fprintf ( pFile, "(%d)", graph->vNum );


	for ( i = 0; i < graph->vNum; i++ ) {
		fprintf ( pFile, "%d->", i );
		for ( j = 0; j < graph->vNum; j++ ) {
			fprintf ( pFile, "(%d)", graph->getWeight( graph, i, j ) );
			fprintf ( pFile, "%d", j );
		}
		fprintf ( pFile, ";" );
	}
	fprintf ( pFile, "." );

	closeStream ( pFile );
	return 1;
}

//TODO: NUMERI NEGATIVI
int parseFile ( GRAPHOBJ *graph, char *stream ) {

	STRING 	*	stringObj 	= ( STRING *) malloc ( sizeof ( STRING ) );

	stringObj->string 		= fileToString ( stream );
	stringObj->cur 			= 0;

	if ( !check ( stringObj->string , "parseFile" ) ) {
		return 0;
	}

	graph->vNum = num_nodi ( stringObj );
	if ( graph->vNum == INT_MIN )
		return 0;

	//printf ("\nnumero vertici: %d\n", graph->vNum);
	graph = graph->build ( graph, graph->vNum );

	//TODO trasforma in bool <stdbool.h>
	if ( graph->vNum != adiacenze ( graph, stringObj ) ) {
		graph->matrix = NULL;
		return 0;
	}

		
	return 1;
}

int num_nodi ( STRING *str ) { 

	while ( str->string[str->cur] == '\n' || 
			str->string[str->cur] == ' '  || 
			str->string[str->cur] == '\t' 
			)
		;
	int num_nodi = 0;
	if ( str->string[str->cur] == '(') {
		++str->cur;
		if ( ( num_nodi = parseDigit ( str ) ) != INT_MIN ) {
			++str->cur;

			if ( str->string[str->cur] == ')') {
				++str->cur;
			} else {
				num_nodi = INT_MIN;	
				sintax_error ( "<num_nodo>')'" );
			} 
		} else {
			num_nodi = INT_MIN;
			sintax_error ( "<num_nodo>" );
		}
	} else { 
		num_nodi = INT_MIN;	
		sintax_error ( "'('<num_nodo>" );
	}
	


	return num_nodi;
}
int parseDigit ( STRING *str ) {

	char buffer[STRLEN];
	int res, i  = 0;

	memset ( buffer, 0, STRLEN );

	if ( str->string[str->cur] == '-' ) {
		buffer[i] = str->string[str->cur];
		++str->cur;
		i++;
	}
	while ( isdigit ( str->string[str->cur] ) ) {

		buffer[i] = str->string[str->cur];
		++str->cur;
		i++;
	}

	if ( i != 0 ) {
		--str->cur;
		res = atoi ( buffer );
	} else
		res = INT_MIN;


	return res;
}

int adiacenze ( GRAPHOBJ *graph, STRING *str ) {

	int check = 0;
	if ( str->string[str->cur] != ';' && str->string[str->cur] != '\0' && graph->vNum > 0 ) {
		if ( adiacenzaNodo ( graph, str ) ) {
			check = adiacenze ( graph, str ) + 1;
		}

	} 
	return check;

}


int adiacenzaNodo ( GRAPHOBJ *graph, STRING *str ) {
	int ID 		= INT_MIN;
	int check 	= 0;

	if ( str->string[str->cur] != ';' &&
		 str->string[str->cur] != '.' &&
		 str->string[str->cur] != '\0' ) {

		//<ID>
		if ( ( ID = parseDigit ( str ) ) != INT_MIN ) {
			++str->cur;
			//'->'
			if ( parseArrow ( str ) ) {
				 if ( listaArchi ( graph, str, ID, graph->vNum ) ) {
				 	++str->cur;					 	
				 	check 	= 1;
				 } else {
				 	sintax_error ( "<adiacenza_nodo>: '<lista_archi>'" );
				 	check = 0;
				 }
			} 
		} else {
			sintax_error ( "<adiacenza_nodo>: '<ID>'" );
			check = 0;
		}

	// ';'
	} else 
		++str->cur;

	return check;
}

int parseArrow ( STRING *str ) {

	int check = 0;
	if ( str->string[str->cur] == '-') {
		++str->cur;	
		if ( str->string[str->cur] == '>') {
			++str->cur;
			check = 1;
		} else 
			sintax_error ( "<adiacenza_nodo>: '>'" );
 	} else 
		sintax_error ( "<adiacenza_nodo>: '-'" );
 	

 	return check;
}

int listaArchi ( GRAPHOBJ *graph, STRING * str, int ID, int vNum ) {

	int check = 0;
	if ( str->string[str->cur] != ';' && str->string[str->cur] != '\0' && vNum > 0 ) {
		if ( arco ( graph, str, ID, vNum) ) {
			check = listaArchi ( graph, str, ID, vNum - 1 ) + 1;
		}

	} else {
		
	}
	return check;
}

	//arco:
	//(<peso>)<nodo>

int arco ( GRAPHOBJ *graph, STRING * str, int ID, int vNum ) {
	int check 	= 0;
	int nodo 	= 0;
	int peso  	= 0;

	if ( ( peso = parsePeso ( str) ) != INT_MIN ) {

		if ( ( nodo = parseDigit ( str ) ) != INT_MIN ) {	
			++str->cur;

			graph->insertEdge ( graph, ID, nodo, peso );
			//setEdge ( graph->matrix, ID, nodo, peso );
			check = 1;
		} else {
			sintax_error ( "arco: <nodo>" );
			check = 0;
		}	

	}

	return check;
}

//'('<peso>')'
int parsePeso ( STRING * str ) {

	int peso = INT_MIN;
	//'('<peso>
	if ( str->string[str->cur] == '(') {
		++str->cur;
		//<peso>>
		if ( ( peso = parseDigit ( str ) ) != INT_MIN ) {
			++str->cur;
			//<peso>')'
			if ( str->string[str->cur] == ')') {
				++str->cur;

			} else {
				sintax_error ( "parsePeso: ')'" );
				peso = INT_MIN;
			}
			
		} else {
			sintax_error ( "parsePeso: <peso>" );
			peso = INT_MIN;
		}

	} else {
			sintax_error ( "parsePeso: '('" );		
			peso = INT_MIN;
	}
	return peso;
}


char *mazeToString ( GRAPHOBJ *graph, char *stream ) {

	char 		c 			= '\0';
	int 		size 		= 40000;
	int 		i 			= 0;

	char *		buffer 		= ( char * ) malloc ( size * sizeof ( char ) );
	FILE * 		pFile 		= openStream ( stream, "r" );
	memset ( buffer, '\0', size );
	
	if ( !check ( pFile , "fileToString" ) ) {
		return NULL;
	}

	c = fgetc ( pFile );
	while ( c != EOF ) {
		buffer = checkResizeArray ( buffer, i, &size );
		if ( c == '\n' ) {
			if ( graph->width == 0 ) {
				graph->width = i;
			}
			graph->height = graph->height + 1;
			c = fgetc ( pFile );
		}
		buffer[i] = c;
		i++;
		buffer[i] = '\0';			
		c = fgetc ( pFile );
	}
	closeStream ( pFile );
	printf ("altezza: %d, larghezza: %d\n", graph->height, graph->width );

	return buffer;
}
char *checkResizeArray ( char *buffer, int i, int *size ) {
		if ( isArrayFull ( i, *size ) ) {
			*size = 2 * ( *size ) + 1;
			buffer = ( char *) realloc ( buffer, *size );				
		}
	return buffer;
}