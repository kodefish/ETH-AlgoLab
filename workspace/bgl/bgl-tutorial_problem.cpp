// ALGOLAB BGL Tutorial 1
// Tutorial example problem

// Compile and run with one of the following:
// g++ -std=c++11 -O2 bgl-tutorial_problem.cpp -o bgl-tutorial_problem; ./bgl-tutorial_problem < bgl-tutorial_problem.in
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 bgl-tutorial_problem.cpp -o bgl-tutorial_problem; ./bgl-tutorial_problem < bgl-tutorial_problem.in

// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
// Namespaces
using namespace std;
using namespace boost;


// Directed graph with integer weights on edges.
typedef adjacency_list<vecS, vecS, directedS,
		no_property,
		property<edge_weight_t, int>
		>					Graph;
typedef graph_traits<Graph>::vertex_descriptor		Vertex;	// Vertex type		
typedef graph_traits<Graph>::edge_descriptor		Edge;	// Edge type
typedef graph_traits<Graph>::edge_iterator		EdgeIt;	// Edge iterator
// Property map edge -> weight
typedef property_map<Graph, edge_weight_t>::type	WeightMap;


// Functions
// ========= 
void testcases() {
	// Read and build graph
	int V, E, t;	// 1st line: <vertex_no> <edge_no> <target>
	cin >> V >> E >> t;
	Graph GT(V);	// Creates an empty graph on V vertices
	WeightMap weightmap = get(edge_weight, GT);
	for (int i = 0; i < E; ++i) {
		int u, v, w;		// Each edge: <from> <to> <weight>
		cin >> u >> v >> w;
		Edge e;	bool success;					// *** We swap u and v to create ***
		tie(e, success) = add_edge(v, u, GT);	// *** the reversed graph GT!   ***
		weightmap[e] = w;
	}

	// Store index of the vertices' strong component. Component indices are in the range [0, nscc)
	vector<int> sccmap(V);	// Use this vector as exterior property map
	int nscc = strong_components(GT, 
			make_iterator_property_map(
				sccmap.begin(), get(vertex_index, GT)));

	// Find universal strong component (if any)
	// Why does this approach work? Exercise.
	vector<bool> is_max(nscc, true);
	EdgeIt ebeg, eend;
	// Iterate over all edges.
	for (tie(ebeg, eend) = edges(GT); ebeg != eend; ++ebeg) {
		// ebeg is an iterator, *ebeg is a descriptor.
		Vertex u = source(*ebeg, GT), v = target(*ebeg, GT);
		if (sccmap[u] != sccmap[v])	is_max[sccmap[u]] = false;
        // this edge (u,v) in GT implies that component sccmap[u] is not minimal in G
	}
	int max_count = count(is_max.begin(), is_max.end(), true);
	if (max_count != 1) {
		cout << "NO" << endl;
		return;
	}

	// Compute shortest t-u path in GT
	vector<int> distmap(V);		// We must use at least one of these
	vector<Vertex> predmap(V);	// vectors as an exterior property map.
	dijkstra_shortest_paths(GT, t,
			predecessor_map(make_iterator_property_map(	// named parameters
					predmap.begin(), get(vertex_index, GT))).
			distance_map(make_iterator_property_map(	// concatenated by .
					distmap.begin(), get(vertex_index, GT))));
	// dijkstra_shortest_paths(GT, t,
	// 	predecessor_map(&predmap[0]). 	// Usually one can use a pointer type as a property map.
	// 	distance_map(&distmap[0]));	// Bug: this doesn't work for Dijkstra with strong_components.hpp loaded
	int res = INT_MAX;
	for (int u = 0; u < V; ++u)
		// Minimum of distances to 'maximal' universal vertices
		if (is_max[sccmap[u]])
			res = min(res, distmap[u]);
	cout << res << endl;
}

// Main function looping over the testcases
int main() {
	ios_base::sync_with_stdio(false);
	int T;	cin >> T;	// First input line: Number of testcases.
	while(T--)	testcases();
	return 0;
}

