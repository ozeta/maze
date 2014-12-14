#ifndef HEAP_H
#define HEAP_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#define MAXLEN 256

/*****************************************************************************/

//struttura che contiene la coppia dato:priorità
typedef struct data {
	int					key;
	int					data;
} Data;

//"classe" heap implementato come array di Data
typedef struct HEAPclass {

	//unità allocate per l'array
	int 				arraysize;
	//unità occupate 
	int 				heapsize;
	//array dinamico di puntatori a Data
	Data **				A;
	void 				( *heapify ) 	( struct HEAPclass *, int );
	void				( *print )		( void *, FILE * );
} Heap;

//definizione dei tipi per i callback
typedef void 	( *heapDEF ) 	( Heap *, int );

Heap *initializeHeap ( heapDEF heapify );

//figlio sinistro
int left ( int i );
//figlio destro
int right ( int i );
//padre
double father ( int i );

//crea una struttura Data.
Data *new_HeapData ( int data, int key ) ;
//riallocazione dello heap array
//la dimensione dell'array viene raddoppiata
void * heap_realloc ( Heap * H );
//controlla se l'array è pieno
int isHeapFull ( Heap * H );
//accoda un oggetto nell'array. nome precedente
void insert ( Heap *H, Data *data );
//stampa
void Hprint ( Heap *H, FILE *stream );
//restituisce il massimo da un maxheap
void *getFirst ( Heap *H ) ;
void *getLast ( Heap *H );
//estrae l'elemento minimo (o massimo) dal maxHeap
Data * extractFirst ( Heap *H );
//aumenta/diminuisce il valore nell'elemento i dello heap
void updateKey ( Heap *H, int i, int value );
void decreaseKey ( Heap *H, int i, int v ) ;
void increaseKey ( Heap *H, int i, int v ) ;
//scambia 2 oggetti
void swap ( void *a, void *b, size_t size ) ;
void minHeapify ( Heap *H, int i );
void maxHeapify ( Heap *H, int i ) ;
//costruisce lo heap a partire da un array disordinato
void buildHeap ( Heap *H ) ;
//cancella lo heap
void removeItem ( Heap *H, int i );
int getData ( Data *data );
int getKey ( Data *data );
bool isHeapEmpty ( Heap *H );
bool heapIntSearch ( Heap *H , int test );
#endif