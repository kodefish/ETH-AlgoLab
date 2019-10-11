// STL includes
#include <iostream>
#include <climits>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                              boost::no_property,
                              boost::property<boost::edge_weight_t, int>> weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;

void testcase() {
    // Read in num vertices and num edges
    int n, m; std::cin >> n >> m;

    // Construct graph
    weighted_graph g(n);
    // Get weight map (maps edges to weights)
    weight_map weights = boost::get(boost::edge_weight, g);
    // Read in edges
    for (int i = 0; i < m; i++) {
        int u, v, w; std::cin >> u >> v >> w;
        edge_desc e = boost::add_edge(u, v, g).first;
        weights[e] = w;
    }

    // Compute minimum spanning tree
    std::vector<edge_desc> mst;
    boost::kruskal_minimum_spanning_tree(g, std::back_inserter(mst));

    // Sum edge weights of minimum spanning tree
    int min_spanning_tree_weight = 0;
    for (auto it = mst.begin(); it != mst.end(); ++it) {
        min_spanning_tree_weight += weights[*it];
    }

    // Compute shortest distances (dijkstra) from 0
    std::vector<int> dist_map(n);
    std::vector<vertex_desc> pred_map(n);

    boost::dijkstra_shortest_paths(g, 0,
                                   boost::distance_map(boost::make_iterator_property_map(
                                                           dist_map.begin(), boost::get(boost::vertex_index, g)
                                                       ))
                                   .predecessor_map(boost::make_iterator_property_map(
                                                        pred_map.begin(), boost::get(boost::vertex_index, g))));

    // Find node furthest away from 0
    int max_dist = INT_MIN;
    for (int i = 0; i < n; i++) {
        if (max_dist < dist_map[i]) max_dist = dist_map[i];
    }

    std::cout << min_spanning_tree_weight << " " << max_dist << std::endl;
}

int main() {
    std::ios::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}
