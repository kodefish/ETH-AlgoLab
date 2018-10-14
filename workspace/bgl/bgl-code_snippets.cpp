// ALGOLAB BGL Tutorial 1
// Code snippets demonstrating 
// - graph definitions
// - several algorithms (components, distance-based algorithms, maximum matching)
// - how to pass exterior property maps
// - use of iterators

// Compile and run with one of the following:
// g++ -std=c++11 -O2 bgl-code_snippets.cpp -o bgl-code_snippets; ./bgl-code_snippets
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 bgl-code_snippets.cpp -o bgl-code_snippets; ./bgl-code_snippets

// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/max_cardinality_matching.hpp>


// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
		boost::no_property,				// interior properties of vertices	
		boost::property<boost::edge_weight_t, int> 		// interior properties of edges
		>					Graph;
typedef boost::graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef boost::graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef boost::graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// to iterate over all outgoing edges of a vertex
typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t


// Functions
// ========= 
void testcases() {
	// Create Graph, Vertices and Edges
	// ================================
	int V = 7;
	Graph G(V);	// creates an empty graph on n vertices
	WeightMap weightmap = boost::get(boost::edge_weight, G);	// start by defining property maps for all interior properties defined in Lines 37, 38
	std::vector<std::vector<int> > graphedges = {{0,2,14},{0,3,12},{1,2,19},{1,3,15},{2,3,13},{4,5,17},{4,6,16},{5,6,18}};
	int E = graphedges.size();
	for (int i = 0; i < E; ++i) {
		Edge e;	bool success;
		Vertex u = graphedges[i][0];
		Vertex v = graphedges[i][1];
		int w = graphedges[i][2];
		boost::tie(e, success) = boost::add_edge(u, v, G);	// Adds edge from u to v. If parallel edges are allowed, success is always true.
							// Caveat: if u or v don't exist in G, G is automatically extended!
		weightmap[e] = w;			// Otherwise it is false in case of failure when the edge is a duplicate
		assert(boost::source(e, G) == u && boost::target(e, G) == v);	// This shows how to get the vertices of an edge
	}
	std::cout << std::endl;


	// Connected components
	// ====================
	std::vector<int> componentmap(V);	// We MUST use such a vector as an Exterior Property Map: Vertex -> Component
	int ncc = boost::connected_components(G, boost::make_iterator_property_map(componentmap.begin(), boost::get(boost::vertex_index, G))); 
	std::cout << "Connected components: " << ncc << "\n";
	std::vector<int> componentsize(ncc);
	// Iterate over all vertices
	for (int i = 0; i < V; ++i)
		++componentsize[componentmap[i]];
	for (int i = 0; i < ncc; ++i)
		std::cout << "Component " << i << " has size " << componentsize[i] << ".\n";
	std::cout << std::endl;


	// Dijkstra shortest paths
	// =======================
	std::vector<Vertex> predmap(V);	// We will use this vector as an Exterior Property Map: Vertex -> Dijkstra Predecessor
	std::vector<int> distmap(V);		// We will use this vector as an Exterior Property Map: Vertex -> Distance to source
	Vertex start = 0;
	boost::dijkstra_shortest_paths(G, start, // We MUST provide at least one of the two maps
		boost::predecessor_map(boost::make_iterator_property_map(predmap.begin(), boost::get(boost::vertex_index, G))).	// predecessor map as Named Parameter
		distance_map(boost::make_iterator_property_map(distmap.begin(), boost::get(boost::vertex_index, G))));	// distance map as Named Parameter
	int maxdist = 0;
	Vertex furthest = start;
	int reachable = 0;
	for (int i = 0; i < V; ++i) {
		if (distmap[i] < INT_MAX) {	// check whether vertex i is reachable, i.e. has "finite" distance from the start
			++reachable;		// INT_MAX comes from climits and the type of distmap (vector<int>).
			if (distmap[i] > maxdist) {
				maxdist = distmap[i];
				furthest = i;
			}
		}
	}
	std::cout << "Dijkstra starting in node " << start << " reaches " << reachable << " out of " << V << " vertices.\n";
	std::cout << "The furthest reachable vertex is " << furthest << " with distance " << maxdist << ".\n";
	std::cout << "Edges in Dijkstra's shortest paths tree:\n";
	// EdgeIterators
	// =============
	EdgeIt ebeg, eend;
	for (boost::tie(ebeg, eend) = boost::edges(G); ebeg != eend; ++ebeg) {	// edges(G) returns a pair of iterators which define a range of all edges. 
		// For undirected graphs, each edge is visited once, with some orientation.
		// ebeg is EdgeIterator, *ebeg is EdgeDescriptor
		Vertex u = source(*ebeg, G);
		Vertex v = target(*ebeg, G);
		if (predmap[u] == v || predmap[v] == u)
			std::cout << u << " -- " << v << " (weight " << weightmap[*ebeg] << ")\n";

	}
	std::cout << "List of disconnected single nodes: ";
	for (int i = 0; i < V; ++i)
		if (predmap[i] == i && i != start)
			std::cout << i << " ";
	std::cout << "\n" << std::endl;
	

	
	// Prim minimum spanning tree
	// ==========================
	std::vector<Vertex> primpredmap(V);	// We MUST use this vector as an Exterior Property Map: Vertex -> Prim Predecessor
	start = 6;
	boost::prim_minimum_spanning_tree(G, boost::make_iterator_property_map(primpredmap.begin(), boost::get(boost::vertex_index, G)));	// Prim from *vertices(G).first (usually 0)
	boost::prim_minimum_spanning_tree(G, boost::make_iterator_property_map(primpredmap.begin(), boost::get(boost::vertex_index, G)),	// Prim from user-defined start,
			boost::root_vertex(start));									// old primpredmap gets reset first!	
	std::cout << "Prim builds a minimum spanning tree (of node " << start << "'s component) of total weight: ";
	int totalweight = 0;
	for (int i = 0; i < V; ++i) {
		if (primpredmap[i] != i) {
			Edge e; bool success;
			boost::tie(e, success) = boost::edge(i, primpredmap[i], G);
			totalweight += weightmap[e];
		}
	}
	std::cout << totalweight << "\n";
	std::cout << "Edges in Prim's minimum spanning:\n";	
	// iterate over all vertices
	for (int i = 0; i < V; ++i) {
		// OutEdgeIterators
		// ================
		Vertex u = i;
		OutEdgeIt oebeg, oeend;
		for (boost::tie(oebeg, oeend) = boost::out_edges(u, G); oebeg != oeend; ++oebeg) {
			Vertex v = boost::target(*oebeg, G);	// source(*oebeg, G) is guaranteed to be u, even in an undirected graph.
			if (primpredmap[u] == v) {
				std::cout << u << " -- " << v << " (weight " << weightmap[*oebeg] << ")\n";
			}
		}
	}
	std::cout << std::endl;


	// Kruskal minimum spanning tree
	// =============================
	std::vector<Edge>	mst; // We must use this vector to store the MST edges (not as a property map!)
	// We can use the following vectors as Exterior Property Maps if we want to access additional information computed by Union-Find:	
	std::vector<Vertex>	kruskalpredmap(V);	// Stores predecessors needed for Union-Find (NOT the MST!)
	std::vector<int>	rankmap(V);		// Stores ranks needed for Union-Find
	boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst),	// kruskal_minimum_spanning_tree(G, back_inserter(mst)); would be fine as well 
			boost::rank_map(boost::make_iterator_property_map(rankmap.begin(), boost::get(boost::vertex_index, G))).
			predecessor_map(boost::make_iterator_property_map(kruskalpredmap.begin(), boost::get(boost::vertex_index, G))));			
	std::cout << "Kruskal builds a minimum spanning tree of total weight: ";
	totalweight = 0;
	// go through the minimum spanning tree with an iterator
	std::vector<Edge>::iterator	mstbeg, mstend = mst.end();
	for (mstbeg = mst.begin(); mstbeg != mstend; ++mstbeg) {
		totalweight += weightmap[*mstbeg];
	}
	std::cout << totalweight << "\n";
	std::cout << "Edges in Kruskal's minimum spanning tree:\n";
	// go through the minimum spanning tree 
	for (size_t i = 0; i < mst.size(); ++i) {
		Edge e = mst[i];
		Vertex u = boost::source(e, G);
		Vertex v = boost::target(e, G);
		std::cout << u << " -- " << v << " (weight " << weightmap[e] << ")\n";
	}
	std::cout << std::endl;


	// Edmonds' maximum cardinality matching
	// =====================================
	std::vector<Vertex> matemap(V);		// We MUST use this vector as an Exterior Property Map: Vertex -> Mate in the matching
	boost::edmonds_maximum_cardinality_matching(G, boost::make_iterator_property_map(matemap.begin(), get(boost::vertex_index, G)));
	// Using the matemap 
	// =================
	const Vertex NULL_VERTEX = boost::graph_traits<Graph>::null_vertex();	// unmatched vertices get the NULL_VERTEX as mate.
	int matchingsize = matching_size(G, boost::make_iterator_property_map(matemap.begin(), get(boost::vertex_index, G)));
	std::cout << "A maximum matching has " << matchingsize << " edges " << " and " << (V-2*matchingsize) << " unmatched vertices.\n"; 
	std::cout << "Edges in the maximum matching:\n";
	for (int i = 0; i < V; ++i) 
		if (matemap[i] != NULL_VERTEX && i < matemap[i])	// i is matched && we only print the edge once
			std::cout << i << " -- " << matemap[i] << "\n";
	std::cout << "List of unmatched vertices: ";
	for (int i = 0; i < V; ++i) 
		if (matemap[i] == NULL_VERTEX)	// i is not matched
			std::cout << i << " ";
	std::cout << "\n" << std::endl;
}

// Main function looping over the testcases
int main() {
	std::ios_base::sync_with_stdio(false); // if you use cin/cout. Do not mix cin/cout with scanf/printf calls!
	int T;	T = 1;
	while(T--)	testcases();
	return 0;
}

