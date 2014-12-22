//bene
int *a_star ( GRAPHOBJ *graph, int s, int t ) {

	int x, y, i;
	int cost;
	bool found 				= false;
	bool yInOpen 			= false;
	bool yInClosed 			= false;
	void 	*	tmp;					//used to store Heap first element
	Set 	*	AdjList		= NULL;		//Adjiacence list implemented as queue
	Set 	*	closed 		= NULL;		//Closed set implemented as queue
	Heap 	*	open 		= NULL;		//Open set implemented as Heap
	int 	*	pred		= ( int *) malloc ( graph->vNum * sizeof ( int ) );

	int g[graph->vNum];					//g distance function
	int f[graph->vNum];					//f distance function

	//initialization
	for ( i = 0; i < graph->vNum; i++ ) {
		f[i] 		= INFINITE;
		g[i] 		= 0;
		pred[i] 	= NIL;
	}
	f[s] = heuristic ( graph, s, t );
	open = initializeHeap ( minHeapify );
	insert ( open, new_HeapData ( s, 0 ) );

	while ( !isHeapEmpty ( open ) && found == false ) {

		//extracting the first Object of the Heap ( Open )
		tmp 	= extractFirst ( open );
		//pushing the node into the Closed set
		push ( closed , setInt ( x ) );
		//get the int number from the extracted Object
		x 		= getData ( tmp );
		//get the ditance f from the extracted Object
		f[x]	= getKey ( tmp );

		//debug
		if ( PRINTALL ) graph->maze[getCoord ( graph, x )->y][getCoord ( graph, x )->x].path = true;
		printf ("x: %d \n", x);
		if ( x == t ) {
			found = true;
		} else {
			
			AdjList = graph->getAdjList ( graph, x );

			while ( !isEmpty ( AdjList ) ) {
				//getting the first element of the adj. list
				y 				= getInt ( getFront ( AdjList ) );
				g[y] 			= g[x] + graph->getWeight ( graph, x, y );
				cost 			= g[y] + heuristic ( graph, y, t );

				//checking if y is in the open set
				bool yInOpen 	= heapIntSearch ( open, y );
				//checking if y is in the closed set
				bool yInClosed 	= intSearch ( closed , y );

				if ( yInOpen && cost < f[y] ) { // case 1
					decreaseKey ( open, y, cost );
				} else if (  yInClosed && cost < f[y] ) { // case 2
					deleteFromSet ( closed, y );
					insert ( open, new_HeapData ( y, cost ) );
				} else if ( !yInClosed && !yInOpen ) { // case 3
					insert ( open, new_HeapData ( y, cost ) );
				}
				AdjList = dequeue ( AdjList );
				
				if ( pred[y] == NIL )
					pred[y] = x;
				printf ("y: %d ", y);
				//printf ("pred[%d]: %d\n", y, x);
				
			}
			printf ("\n");
		}
        push ( closed , setInt ( x ) );				
	}
	pred[s] = NIL;
	printf ("\n");

	return pred;
}
