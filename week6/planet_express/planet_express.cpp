// STL includes
#include <iostream>
#include <vector>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/strong_components.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
                              boost::no_property,
                              boost::property<boost::edge_weight_t, int>> weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<weighted_graph>::edge_iterator edge_itr;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;

void testcase() {
    int n, m, k, t;
    std::cin >> n >> m >> k >> t;

    std::vector<int> teleportation_network(t);
    for (int i = 0; i < t; i++) {
        std::cin >> teleportation_network[i];
    }

    // Build graph representation of the universe
    weighted_graph universe(n);
    weight_map weights = boost::get(boost::edge_weight, universe);
    for (int i = 0; i < m; i++) {
        int u, v, c; std::cin >> u >> v >> c;
        edge_desc e; bool s;
        boost::tie(e, s) = boost::add_edge(v, u, universe);
        weights[e] = c;
    }

    // Compute teleportation network
    // 2 nodes are connected if they are strongly connected
    // So we create a new node for every connected component, and use it as a "hub"
    // Travelling to the hub costs t(u) (which is the same for all nodes in the component)
    // Travelling from the hub is free.
    // 1. Compute strongly connected components
    std::vector<int> scc_map(n);
    int nscc = boost::strong_components(universe,
                                        boost::make_iterator_property_map(scc_map.begin(), boost::get(boost::vertex_index, universe)));

    // Compute size of each scc
    std::vector<int> scc_size(nscc, 0);
    for (int i = 0; i < t; i++) {
        scc_size[scc_map[teleportation_network[i]]]++;
    }

    // Add teleportation hub connections
    for (int i = 0; i < t; i++) {
        int u = teleportation_network[i];
        edge_desc e; bool s;

        // Add edge from u to hub_u of cost t(u), which is the size of the component minus 1 (itself)
        boost::tie(e, s) = boost::add_edge(u, n+scc_map[u], universe);
        weights[e] = scc_size[scc_map[u]] - 1;

        // Add edge from hub_u to u of cost 0
        boost::tie(e, s) = boost::add_edge(n+scc_map[u], u, universe);
        weights[e] = 0;
    }

    // Find warehouse closest to destination
    // For every warehouse, compute distance to client and take the minimal one
    int min_dist_to_client = INT_MAX;
    std::vector<int> dist_map(n + nscc);
    boost::dijkstra_shortest_paths(universe, n-1,
                                    boost::distance_map(boost::make_iterator_property_map(
                                                            dist_map.begin(), boost::get(boost::vertex_index, universe))));
    for (int i = 0; i < k; i++) {
       min_dist_to_client = std::min(min_dist_to_client, dist_map[i]);
    }

    // Print "no" if one-second delivery is impossible, otherwise print time it takes
    if (min_dist_to_client > 1000000) std::cout << "no" << std::endl;
    else std::cout << min_dist_to_client << std::endl;

}

int main() {
    std::ios::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}

