#ifndef STRUCTURES_H
#define STRUCTURES_H


#include "set.h"
#define WHITE 		0
#define GREY 		1
#define	BLACK 		2
#define INFINITE 	1000000
#define NIL			-1000000

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

/*
typedef struct edgeType {

	int	ID;
	int	weight;

	struct edgeType *	nextEdge;

} EDGE;

typedef struct vertex {

	int ID;
	struct vertex 	*	next;
	EDGE 			*	edge;

} VERTEX;
*/
typedef struct GraphT_Struct {

	int		weight;
	int 	ID;
	int 	k;
} GraphT;

typedef struct {
	int x;
	int y;
	int k;		//value of the position
	int ID;
	bool path;	//temporary field used to print the path
} VCOORD;

typedef struct gObj {

	
	//puntatore a matrice di adiacenza
	GraphT 	** 		matrix;
	/*
	//puntatore a lista di adiacenza
	VERTEX 	* 		list;
	*/
	//numero vertici
	int 			vNum;			//number of vertices

	VCOORD 	**		maze;			//pointer to implicit maze
	int 			width;			//width of the maze
	int 			height;			//height of the maze

	//function pointers
	struct gObj * 	( *build) 			( struct gObj *, int );
	Set *			( *getAdjList )		( struct gObj *graph, int u );
	
	void 			( *insertEdge ) 	( struct gObj *, int , int , int  );
	void 			( *insertVertex )	( struct gObj *graph );
	
	int 			( *getWeight )		( struct gObj *, int, int );
	int 			( *transpose ) 		( struct gObj *graph );
	int 		* 	( *path)			( struct gObj *, int , int );
} GRAPHOBJ;

/*
typedef struct dfs_variables {

	int 	*		d;
	int 	*		f;
	int		*		pred;
	int 	* 		colore;
	int 	*		loop;
	Set 	* 		coda;
} DFSVAR;

*/
typedef int				( *insDEF )		( int, int );
typedef int				( *compDEF )	( void *, void * );
typedef int 			( *WDEF )		( struct gObj *, int, int );
typedef int 			( *TDEF ) 		( GRAPHOBJ *graph );
typedef GRAPHOBJ *		( *BDEF) 		( struct gObj *, int );
typedef void 			( *IEDEF ) 		( struct gObj *, int , int , int  );
typedef void 			( *VINS ) 		( struct gObj *graph );
typedef Set *			( *ADEF )		( GRAPHOBJ *graph, int u );
typedef int	* 			( *PATH)		( struct gObj *, int , int );

#endif
