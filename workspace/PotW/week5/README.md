# Planet Express
## Description
Given a graph of planets, find the warehouse that is closest to the customer. The customer is always at node n-1 (nodes are 0-indexed).
Some planets are part of a teleportation network. Teleportation between 2 planets is only possible if the two are both in the network and can be reached by non-teleportative means.
The warehouses are located on planets 0 to k-1. 

## Approach
The goal here is to find out which nodes you can teleport from and add them to the graph. As stated before: _Teleportation between 2 planets is only possible if the two are both in the network and can be reached by non-teleportative means_. In other words, if two nodes are in the teleportation network and they are part of the same strong component, then there is an edge between them with weight equal to the _size of the strong component - 1_.
Now the naive approach is to add all such edges to the graph. Unfortunately this does not scale well. At all. It's horrible. Don't do it. The key idea here is to replace all these paths by a "central hub". So for each connected component, add a vertice. Then add an edge from every planet in the connected component to the hub and back, with weight _(size of the strong component - 1)/2_. This is equivalent to the mess of edges we had before.

Now that we have this teleportation network overlay, we can use Dijkstra shortest paths to find the warehouse with the smallest distance to the customer. To do so, we store a reverse graph. So if the input gives us edge _(u, v)_ with weight _w_, we store edge _(v, u)_ with weight _w_.

## Remarks
To deal with the division by 2 in the central hub edges, I multiplied all the weights by 2, and did not do the division. Then after running Dijkstra shortest paths I divide everything by 2. This is guaranteed to be round number.
