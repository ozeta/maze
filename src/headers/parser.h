#ifndef PARSER_H
#define PARSER_H

#include "./structures.h"
#include <stdbool.h>
#define STRLEN 256


typedef struct strObjstruct {

	char * 	string;
	int 	cur;
} STRING;

bool isArrayFull ( int i, int size );
char *fileToString ( char *stream );
int writeOnFile ( GRAPHOBJ *graph, char *stream ) ;
int parseFile ( GRAPHOBJ *graph, char *stream );

void sintax_error ( const char *string );
int num_nodi ( STRING *str );
int parseDigit ( STRING *str );
int adiacenze ( GRAPHOBJ *graph, STRING *str );
int parseArrow ( STRING *str );
int parsePeso ( STRING * str );
int adiacenzaNodo ( GRAPHOBJ *graph, STRING *str );
int listaArchi ( GRAPHOBJ *graph, STRING * str, int ID, int vNum ) ;

int arco ( GRAPHOBJ *graph, STRING * str, int ID, int vertexNum );

/*MAZE*/
char *checkResizeArray ( char *buffer, int i, int *size );
char *mazeToString ( char *stream, int *width, int *height );


#endif
