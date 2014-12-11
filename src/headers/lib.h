#ifndef LIB_H
#define LIB_H
#include <curses.h>
#include "./structures.h"

/**
apre uno stream su file
*/
FILE *openStream ( const char *path, const char *mode );
/**
chiude uno stream su file
*/
void closeStream ( FILE *stream );
void clear_screen( ) ;

/**
controlla che il puntatore a test non sia nullo
*/
int check ( void *test, const char *message );

#endif