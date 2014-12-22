function A*(start,goal)
    closedset := the empty set    // The set of nodes already evaluated.
    openset := {start}    // The set of tentative nodes to be evaluated, initially containing the start node
    came_from := the empty map    // The map of navigated nodes.
 
    g_score[start] := 0    // Cost from start along best known path.
    // Estimated total cost from start to goal through y.
    f_score[start] := g_score[start] + heuristic_cost_estimate(start, goal)
 
    while openset is not empty
        current := the node in openset having the lowest f_score[] value
        if current = goal
            return reconstruct_path(came_from, goal)
 
        remove current from openset
        add current to closedset
        for each neighbor in neighbor_nodes(current)
            if neighbor in closedset
                continue
            tentative_g_score := g_score[current] + dist_between(current,neighbor)
 
            if neighbor not in openset or tentative_g_score < g_score[neighbor] 
                came_from[neighbor] := current
                g_score[neighbor] := tentative_g_score
                f_score[neighbor] := g_score[neighbor] + heuristic_cost_estimate(neighbor, goal)
                if neighbor not in openset
                    add neighbor to openset
 
    return failure

    function reconstruct_path(came_from,current)
    total_path := [current]
    while current in came_from:
        current := came_from[current]
        total_path.append(current)
    return total_path

1  S := empty sequence
2  u := target
3  while previous[u] is defined:                // Construct the shortest path with a stack S
4      insert u at the beginning of S           // Push the vertex into the stack
5      u := previous[u]                         // Traverse from target to source
6  end while

    for each neighbor of node:
        skip if it's already been reached (in the closedSet) // it's guaranteed to have been shorter than whatever f(x) is about to be computed)
        compute g(x) and h(x)
        if neighbor already in the openSet:
            if neighbor has a smaller g(x) than the node already in the openSet:
                update g(x) and prev pointers 
        else:
            add to the openSet

/***/
frontier = PriorityQueue()
frontier.put(start, 0)
came_from = {}
cost_so_far = {}
came_from[start] = None
cost_so_far[start] = 0

while not frontier.empty():
   current = frontier.get()

   if current == goal:
      break
   
   for next in graph.neighbors(current):
      new_cost = cost_so_far[current] + graph.cost(current, next)
      if next not in cost_so_far or new_cost < cost_so_far[next]:
         cost_so_far[next] = new_cost
         priority = new_cost + heuristic(goal, next)
         frontier.put(next, priority)
         came_from[next] = current


         int *reconstruct_path ( int *came_from, int current ) {
    Set *total_path = NULL;
    total_path = enqueue ( total_path, setInt ( current ) );
    while ( current > 0 ) {
        printf ( "current: %4d ", current );
        current = came_from[current];
        total_path = enqueue ( total_path, setInt ( current ) );
    }
    return NULL;
}