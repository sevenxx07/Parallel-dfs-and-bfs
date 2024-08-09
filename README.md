# Formulation of the problem 

A discrete dynamical system is uniquely determined by its initial state, a set of possible actions, 
and a transition function that determines the next state for a given state and a given action. The transition function tells us which 
action we can take to get in some new state or even the same. 

The state space for a given discrete dynamical system is then the set of all configurations that the system can assume. Based on the 
transition function, the state space graph determines how we can move between individual configurations by performing actions. 
I implemented parallel versions of two basic algorithms for searching state spaces (that is, potentially infinite graphs).

* Breadth-first search guarantees finding the shortest path to a goal in an unweighted graph (provided that the goal is reachable at finite depth). In the case that we have no other information about the problem we are solving to direct us to the goal, breadth-first search is one of the most efficient algorithms. Its main disadvantage is the high memory complexity, since we have to remember all the nodes that are reachable in a given number of steps. This is unacceptable if the nearest target is at a greater depth and/or the graph branches rapidly.
* Depth-first search (DFS) in infinite graphs does not even guarantee finding any path in the graph, let alone the shortest one. Its great advantage is the low memory complexity, since we only need to remember the nodes on the currently considered path. One way to get around the lack of guarantees is to use iterative deepening (ID-DFS algorithm). The principle of iterative deepening can be briefly summarized as follows: We always search the state space to a certain finite depth, which we set adaptively. We start with a shallow/zero depth and depth-search all paths up to that length. If no solution is found, we increase the depth and search again. The price for the guarantee of finding the shortest using iterative deepening is that we usually have to search the state space many times.
