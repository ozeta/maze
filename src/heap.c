#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "./headers/heap.h"
#include "./headers/structures.h"
#include "./headers/set.h"
#include "./headers/lib.h"

#include "./headers/functions.h"

#define DEBUG 1

/*
metodi impiegati dall'array Heap

*/

//figlio sinistro
int left ( int i ) {
	return 2 * i;
}
//figlio destro
int right ( int i ) {
	return ( 2 * i ) + 1;
}
//padre
double father ( int i ) {
	return ( floor ( i/2 ) );
}


/**
inizializza l'oggetto Heap
*/
Heap *initializeHeap ( heapDEF heapify ) {

	Heap *H = ( Heap * ) malloc ( sizeof ( Heap ) );

	if ( !check ( H, "initializeHeap, err0" ) )
		exit (-1);

	H->arraysize 	= 1;
	H->heapsize 	= 1;
	H->heapify 		= heapify;
	/*alloco lo spazio iniziale per l'array heap*/
	H->A = ( Data ** ) malloc ( H->heapsize * sizeof ( Data * ) );

	if ( !check ( H->A, "initializeHeap, err1" ) )
		return NULL;

	memset ( H->A, 0, H->heapsize * sizeof ( Data * ) );
	//H->new 			= new_Heap;

	return H;
}



//crea una struttura Data.
//insert: callback del metodo di insermento
Data *new_HeapData ( int data, int key ) {

	int *temp;
	Data *dataStruct 			= ( Data * ) malloc ( sizeof ( Data ) );
	if ( dataStruct ) {
		dataStruct->data		= data;
		dataStruct->key 		= key;
	} else {

	}

	return dataStruct;
}
//riallocazione dello heap array
//la dimensione dell'array viene raddoppiata
void * heap_realloc ( Heap * H ) {

	H->A = ( Data ** ) realloc ( H->A, 2 * H->arraysize * sizeof ( Data * ) );
	if ( !check ( H->A, "heap->realloc" ) )
		return NULL;
	H->arraysize = 2 * H->arraysize;
	return H->A;
}

//controlla se l'array è pieno
int isHeapFull ( Heap * H ) {
	if ( H->heapsize == H->arraysize )
		return 1;
	else
		return 0;
}


//accoda un oggetto precedentemente creato nell'array.
//l'oggetto deve essere di tipo Data
//void insert ( Heap *H, void *data )
void insert ( Heap *H, Data *data ) {
	if ( isHeapFull ( H ) ) {
		if ( !( H->A = ( Data ** ) heap_realloc ( H ) ) ) {
			printf ("\n\nmemoria esaurita\n\n");
			return;
		}
	}
	H->A[H->heapsize] = data;

	int f = father ( H->heapsize );
	if ( f >= 1) {
		H->heapify ( H, f );	
	} else 
		H->heapify ( H, H->heapsize );	
	H->heapsize++;
}



//stampa tutto lo heap
void Hprint ( Heap *H, FILE *stream ) {
	int x = 1;
	if ( H == NULL || H->A == NULL ) {
		return;
	}
	fprintf ( stream, "\nElenco Heap:\n" );
	while ( x < H->heapsize ) {
		fprintf ( stream, "[%d: ", x );
		fprintf ( stream, "%d", H->A[x]->data  );
		fprintf ( stream, ", %d", H->A[x]->key );
		fprintf ( stream, "], " );

		x++;
	}
	fprintf ( stream, "\n\n" );
}

//restituisce il massimo da un maxheap
void *getFirst ( Heap *H ) {

	void *result;
	if ( !H || H->heapsize < 1 )
		result = NULL;
	else
		result = H->A[1];
	return result;
}

void *getLast ( Heap *H ) {

	void *result;
	if ( !H || H->heapsize < 1 )
		result = NULL;
	else
		result = H->A[H->heapsize-1];
	return result;
}

//estrae il primo elemento  dallo Heap
Data * extractFirst ( Heap *H ) {

	Data * tmp;
	if ( !check ( H, "extractFirst, H" ) )
		return NULL;
	else if ( H->heapsize-1 < 1 ) {
		printf ("\n\nHeap underflow!\n\n");
		tmp = NULL;
	} else {
		tmp = H->A[1];
		H->A[1] = H->A[H->heapsize-1];
		H->heapsize--;
		H->heapify ( H, 1 );
	}

	return tmp;
}



//aumenta/diminuisce il valore value dell'elemento nella posizione i dello heap
void updateKey ( Heap *H, int i, int value )  {
	if ( value < H->A[i]->key ) {
		decreaseKey ( H, i, value );
	} else {
		increaseKey ( H, i, value );
	}
}
void decreaseKey ( Heap *H, int i, int value ) {

	int k  = 1;
	while ( k < H->heapsize && H->A[k]->data != i ) {
		k++;
	}
	H->A[k]->key = value;
	int f = father ( k );
	if ( f >= 1) {
		H->heapify ( H, f );	
	}
}

/*
*/
void increaseKey ( Heap *H, int i, int value ) {

	if ( value < H->A[i]->key ) {
		printf ("\n\nErrore: nuova chiave più piccola\n\n");
	} else {
		H->A[i]->key = value;
		int root;

		while ( i > 1 && ( H->A[i]->key > H->A[root = father ( i )]->key ) ) {
			swap ( H->A[i], H->A[root] , sizeof ( Data ) );
			i = father ( i );
		}
	}
}

//scambia 2 oggetti
void swap ( void *a, void *b, size_t size ) {

		void *c = malloc ( size );
		memcpy ( c, a, size );
		memcpy ( a, b, size );
		memcpy ( b, c, size );
		free ( c );
}
 
void minHeapify ( Heap *H, int i ) {

	int l = left ( i );
	int r = right ( i );
	int max;

	if ( l < H->heapsize  && ( H->A[l]->key < H->A[i]->key ) ) {
		max = l;
	} else {
		max = i;
	}
	if ( r < H->heapsize && ( H->A[r]->key < H->A[max]->key ) ) {
		max = r;
	}
	if ( max != i ) {
		swap ( H->A[max], H->A[i] , sizeof ( Data ) );
		minHeapify ( H, max );
	}
}
void maxHeapify ( Heap *H, int i ) {

	int l = left ( i );
	int r = right ( i );
	int max;

	if ( l < H->heapsize  && ( H->A[l]->key > H->A[i]->key ) ) {
		max = l;
	} else {
		max = i;
	}
	if ( r < H->heapsize && ( H->A[r]->key > H->A[max]->key ) ) {
		max = r;
	}
	if ( max != i ) {
		swap ( H->A[max], H->A[i] , sizeof ( Data ) );
		maxHeapify ( H, max );
	}
}

//costruisce lo heap a partire da un array disordinato
void buildHeap ( Heap *H ) {

	int i = floor ( ( H->heapsize ) / 2 );
	for ( ; i > 0; i-- ) {
		H->heapify ( H, i );
	}
}

//cancella dallo heap
void removeItem ( Heap *H, int i ) {

	if ( H->heapsize < 1 ) {
		printf ("\nheap underflow\n");
	} else {
			
		H->A[i]->data = H->A[H->heapsize-1]->data;

		if ( H->A[H->heapsize-1]->key > H->A[i]->key ) {
			increaseKey ( H, i, H->A[H->heapsize-1]->key );
		} else {
			decreaseKey ( H, i, H->A[H->heapsize-1]->key );
		}
		H->heapsize--;
		
	}

}

int getData ( Data *data ) {
	return data->data;
}
int getKey ( Data *data ) {
	return data->key;
}

bool isHeapEmpty ( Heap *H ) {

	if ( H->heapsize > 1 ) {
		return false;
	}
	else {
		return true;
	}
}

bool heapIntSearch ( Heap *H , int test ) {

	int size = H->heapsize -1;
	bool result = false;
	while ( size > 0 && !result ) {
		if ( test == H->A[size]->data ) {
			result = true;
		}
		size--;
	}
	return result;
}