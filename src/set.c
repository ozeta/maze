#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "./headers/set.h"
#include "./headers/structures.h"


/** mette in coda un puntatore a elemento void
in modo ricorsivo, e restituisce la coda
*/

void printSet ( Set *set, FILE *stream ) {
	if ( set ) {
		Set* curr = set;
		while ( curr ) {
			fprintf (stream, "%d\n", getInt ( curr->data ) );
			curr = curr->next;
		}
	} else {
		fprintf ( stream, "set vuoto\n");
	}
}
bool intSearch ( Set *set, int data ) {
	bool result = false;
	if ( set ) {
		if (  * ( int * ) set->data == data ) {
			result = true;
		} else {
			result = intSearch ( set->next, data );
		}
	} 
	return result;
}

/*wrapper per numero intero*/
void *setInt ( int i ) {
	int *Int = ( int *) malloc ( sizeof ( int ) );
	*Int = i;
	return Int;
}

int getInt ( void *i ) {
	if ( i ) {
		return * ( int * ) i;
	} else
		return NIL;
}

/**stampa lo stack*/
void stampaIntSet ( Set *st, printDEF print, FILE *stream ) {

	if ( st ) {
		//dataPrint ( st->data, print, stream );
		stampaIntSet ( st->next, print, stream );
	}
}

/*wrapper per coordinata grafo*/
void *setCoord ( int x, int y ) {

	VCOORD *vCoord = ( VCOORD * ) malloc ( sizeof ( VCOORD ) );
	vCoord->x = x;
	vCoord->y = y;

	return vCoord;
}




/**
 *
 *
 * 
 */
Set *newSet ( void *data ) {
	Set *set = ( Set * ) malloc ( sizeof ( Set ) );
	set->data = data;
	set->next = NULL;
	set->last = NULL;	
	return set;
}
/*
*/
Set *enqueue (Set *queue, void *data) {

	if ( queue == NULL ) {
		queue = newSet ( data );
	} else {
		queue->next = enqueue ( queue->next, data );
	}

	return queue;

}

/**rimuove il primo elemento della coda
restituisce NULL se la coda e' vuota
*/
Set *dequeue ( Set *queue ) {

	if ( queue ) {
		Set *temp = queue;
		queue  = queue->next;
		free ( temp );	
	}
	return queue;
}

//restituisce il primo elemento dalla coda
void *getFront ( Set *queue ) { 
	if ( queue ) 
	    return queue->data;
	else
		return NULL;
}

void *getTail ( Set *queue ) { 
	if ( !queue ) {
		return NULL;
	}
	Set *prev = queue;
	Set *cur = queue->next;
	while ( cur ) {
		prev = cur;
		cur = cur->next;
	}

	prev->next = NULL;
	void *res = prev->data;
	free ( cur );
	return res;
}

/******************************************************************************

operazioni stack

******************************************************************************/


/**stack vuoto?:
se lo stack è vuoto e restituisce 1*/
int stackVuoto (Set *st) {
	return 1 ? st == NULL : 0;
}

/**push:	inserisce un nuovo elemento nello stack
			restituisce il nuovo stack
*/
Set *push (Set *stack, void *data) {

    Set *temp = (Set *) malloc (sizeof (Set));
    if ( !temp )
        printf ("low memory"), exit (EXIT_FAILURE);
    temp->data = data;
    temp->next = stack;

    return temp;
}

void *top (Set *stack) {
	if ( stack )
	    return stack->data;
	else
		return NULL;
}


Set *pop (Set *stack) {

    Set *temp = stack;

    if ( temp ) {
        stack = stack->next;
        free (temp);
    } 
    return stack;
}

/*
int hasBothChild ( node * temp ) {

    return temp && temp->left && temp->right ;
}
*/

int isEmpty ( Set *queue ) {
    if ( !queue )
        return 1;
    else
        return 0;
}

void clearSet ( Set *set ) {
	
	while ( ( set = pop ( set ) ) != NULL );
}