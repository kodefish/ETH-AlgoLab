// STL includes
#include <iostream>
#include <vector>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
                              boost::no_property,
                              boost::property<boost::edge_weight_t, int>> weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
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
        boost::tie(e, s) = boost::add_edge(u, v, universe);
        weights[e] = c;
    }

    // Figure out which vertices can go where
    std::vector<std::vector<int>> pairwise_reachable(n);
    for (int u = 0; u < t; u++) {
        // Compute path to each other node in the teleportation network
        // If pairwise reachable, then add edge u -> v and v -> u
        // Compute distance map from u (unreachable vertices have distance INT_MAX)
        std::vector<int> dist_map_u(n);
        boost::dijkstra_shortest_paths(universe, teleportation_network[u],
                                       boost::distance_map(boost::make_iterator_property_map(
                                                               dist_map_u.begin(), boost::get(boost::vertex_index, universe))));
        for (int v = u + 1; v < t; v++) {
            // Compute distance map from v (unreachable vertices have distance INT_MAX)
            std::vector<int> dist_map_v(n);
            boost::dijkstra_shortest_paths(universe, teleportation_network[v],
                                           boost::distance_map(boost::make_iterator_property_map(
                                                                   dist_map_v.begin(), boost::get(boost::vertex_index, universe))));
            if (dist_map_u[teleportation_network[v]]  < INT_MAX && dist_map_v[teleportation_network[u]] < INT_MAX) {
                // U, v are pairwise reachable
                pairwise_reachable[u].push_back(v);
                pairwise_reachable[u].push_back(u);
            }
        }
    }

    // Actually build teleportation network
    for (int u = 0; u < n; u++) {
        for (int v = 0; v < pairwise_reachable[u].size(); v++) {
            edge_desc e; bool s;
            boost::tie(e, s) = boost::add_edge(u, pairwise_reachable[u][v], universe);
            weights[e] = pairwise_reachable[u].size();
        }
    }

    // Find warehouse closest to destination
    // For every warehouse, compute distance to client and take the minimal one
    int min_dist_to_client = INT_MAX;
    for (int i = 0; i < k; i++) {
        std::vector<int> dist_map(n);
        boost::dijkstra_shortest_paths(universe, i,
                                       boost::distance_map(boost::make_iterator_property_map(
                                                               dist_map.begin(), boost::get(boost::vertex_index, universe))));
       min_dist_to_client = std::min(min_dist_to_client, dist_map[n-1]);
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
