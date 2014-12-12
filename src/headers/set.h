#ifndef SET_H
#define SET_H
#include <stdbool.h>


typedef void	( *printDEF )	( void *, FILE * );

/******************************************************************************

operazioni coda

******************************************************************************/
typedef struct SET {

	void *				data;
	struct SET *		next;
	struct SET *		last;
} Set;

/*wrapper per numero intero*/
bool 		intSearch ( Set *set, int data );
void 	*	setInt ( int i );
int 		getInt ( void *i );
void 		printSet ( Set *set, FILE *stream );
void 		stampaIntSet ( Set *st, printDEF print, FILE *stream );
Set 	*	enqueue ( Set *queue, void *data );
Set 	*	dequeue ( Set *queue );
void 	*	getFront ( Set *queue ) ;
int 		stackVuoto ( Set *st );
Set 	*	push ( Set *st, void *data );
void 	*	top ( Set *stack );
Set 	*	pop ( Set *stack );
int 		isEmpty ( Set *queue );
void 		clearSet ( Set *set );
void 	*	setCoord ( int x, int y );
void 	*	getTail ( Set *queue );
#endif