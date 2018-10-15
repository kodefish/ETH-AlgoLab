# Ant Challenge
# Description
The problem is to get from point a to point b using various networks. Each network is a subset of a shared graph G. The problem asks what's the shortest distance using these various sub-networks. Each subnetwork is constructed via Prim's algorithm for a minimum spanning tree. Every edge in the network has a different weight depending on the type of network, so 2 networks can contain the same edge, but with different weights.

## Approach
1. Compute each subnetwork by using BGL's implementaition of Prim's algorithm to compute MSTs.
2. Combine the MSTs to get a general network using only edges which are part of a sub-network. 
3. Using Dijkstra's shortest paths algorithms, find the shortest path from _a_ to _b_.

## Remarks
When combiing the MST, allow parallel edges, because Dijkstra's shortest paths algorithm will automatically select the fastest one.
