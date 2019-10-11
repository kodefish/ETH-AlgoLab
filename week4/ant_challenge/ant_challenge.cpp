// STL includes
#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace boost;
typedef adjacency_list<vecS, vecS, undirectedS,
                       property<vertex_distance_t, int>,
                       property<edge_weight_t, int>> weighted_graph;
typedef property_map<weighted_graph, edge_weight_t>::type weight_map_t;
typedef graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef graph_traits<weighted_graph>::vertex_descriptor vertex_desc;


void testcase() {
    // Read in number of vertices (trees), edges, species
    int n, e, s; std::cin >> n >> e >> s;
    // Read in the start and finish vertices of breadcrumb carrying challenge
    int a, b; std::cin >> a >> b;

    // Init graphs (one per species)
    // Initialise all the weight maps (one per species)
    std::vector<weighted_graph> g(s, weighted_graph(n));
    std::vector<weight_map_t> weight_maps(s);
    for (int i = 0; i < s; i++) {
        weight_maps[i] = get(edge_weight, g[i]);
    }

    // Read in edges and corresponding weights for each species
    for (int i = 0; i < e; i++) {
        int u, v; std::cin >> u >> v;
        for (int j = 0; j < s; j++) {
            edge_desc e = add_edge(u, v, g[j]).first;
            int s_i; std::cin >> s_i;
            weight_maps[j][e] = s_i;
        }
    }

    // Read in each specie's hive vertice
    std::vector<int> hives;
    for (int i = 0; i < s; i++) {
        int h_i; std::cin >> h_i;
        hives.push_back(h_i);
    }

    // For every species, compute the minimum spanning tree
    std::vector<std::vector<edge_desc>> msts(s);
    for (int i = 0; i < s; i++) {
        kruskal_minimum_spanning_tree(g[i], std::back_inserter(msts[i]));
    }

    // Create new graph containing only edges from minimum spanning trees of each species network
    weighted_graph combined_g(n);
    weight_map_t combined_weights = get(edge_weight, combined_g);

    for (int i = 0; i < s; i++) {
        for (auto mst_edge = msts[i].begin(); mst_edge != msts[i].end(); mst_edge++) {
            int u = source(*mst_edge, g[i]), v = target(*mst_edge, g[i]), w = weight_maps[i][*mst_edge];
            edge_desc e = add_edge(u, v, combined_g).first;
            combined_weights[e] = w;
        }
    }

    // Compute shortes distance in combined MST graph
    std::vector<int> dist_map(n);
    std::vector<vertex_desc> pred_map(n);

    dijkstra_shortest_paths(combined_g, a,
                            distance_map(make_iterator_property_map(
                                             dist_map.begin(), get(vertex_index, combined_g)))
                            .predecessor_map(make_iterator_property_map(
                                                 pred_map.begin(), get(vertex_index, combined_g))));

    std::cout << dist_map[b] << std::endl;
}

int main() {
    std::ios::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}
