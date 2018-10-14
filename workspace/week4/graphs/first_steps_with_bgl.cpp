#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace boost;

// Directed graph with integer weights on edges.
typedef adjacency_list<vecS, vecS, undirectedS,
        no_property,
        property<edge_weight_t, int>
        >                                           Graph;
typedef graph_traits<Graph>::vertex_descriptor      Vertex;
typedef graph_traits<Graph>::edge_descriptor        Edge;
typedef graph_traits<Graph>::edge_iterator          EdgeItr;
typedef property_map<Graph, edge_weight_t>::type    WeightMap;

void testcase(){
    // Read num vertices and num edges;
    int n, m; std::cin >> n >> m;
    Graph G(n); // Empty graph with n vertices;
    // Define property map for all interior properties defined in lines 16 and 17
    WeightMap weight_map = get(edge_weight, G);
    for (int i = 0; i < m; ++i) {
        int u, v, w; 
        std::cin >> u >> v >> w;
        Edge e; bool success;
        std::tie(e, success) = add_edge(u, v, G);
        if(success) {
            weight_map[e] = w;
        }
    }

    // Compute minimum spanning tree using Prim's algorithm
    std::vector<Vertex> primpredmap(n);
    prim_minimum_spanning_tree(G, make_iterator_property_map(primpredmap.begin(), get(vertex_index, G)));

    int totalweight = 0;
    for(int i = 0; i < n; i++) {
        if (primpredmap[i] != i) {
            Edge e; bool success;
            std::tie(e, success) = edge(i, primpredmap[i], G);
            totalweight += weight_map[e]; 
        }
    }

    // Compute distance from node 0 to all other nodes, and keep the furthest one
    std::vector<Vertex> pred_map(n);    // vertex -> dijkstra predecessor
    std::vector<int> dist_map(n);       // Vertex -> distance to src

    Vertex start = 0;

    // Predecessor and distance maps as Named parameters
    dijkstra_shortest_paths(G, start, 
                    predecessor_map(make_iterator_property_map(pred_map.begin(), get(vertex_index, G))).
                    distance_map(make_iterator_property_map(dist_map.begin(), get(vertex_index, G))));

    int max_dist = 0;
    for(int i = 0; i < n; ++i) {
        if (dist_map[i] < INT_MAX) { // If vertex is reachable
            if (dist_map[i] > max_dist) {
                max_dist = dist_map[i];
            }
        }
    }

    std::cout << totalweight << " " << max_dist << std::endl;
    

}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    for(int i = 0; i < t; i++) {
        testcase();
    }
}
