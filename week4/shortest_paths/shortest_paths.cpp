#include <iostream>
#include <vector>
#include <climits>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
                              boost::no_property,
                              boost::property<boost::edge_weight_t, int>> graph;
typedef boost::property_map<graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<graph>::edge_descriptor edge_desc;

int main() {
    std::ios::sync_with_stdio(false);
    // Read in the number of vertices, directed streets and queries
    int n, m, q; std::cin >> n >> m >> q;

    // Read in the positions of the vertices in the real world
    for (int i = 0; i < n; i++) {
        int lon, lat; std::cin >> lon >> lat;
    }

    // Init graph and weight map
    graph g(n);
    weight_map weights = boost::get(boost::edge_weight, g);

    // Read in the edges of the graph
    for (int i = 0; i < m; i++) {
        int a, b, c; std::cin >> a >> b >> c;
        edge_desc e; bool s;
        boost::tie(e, s) = boost::add_edge(a, b, g);
        weights[e] = c;
    }

    // Read in queries
    for (int i = 0; i < q; i++) {
        int s, t; std::cin >> s >> t;
        std::vector<int> dist_map(n);
        std::vector<int> pred_map(n);

        boost::dijkstra_shortest_paths(g, s,
                                       boost::distance_map(boost::make_iterator_property_map(dist_map.begin(), boost::get(boost::vertex_index, g)))
                                       .predecessor_map(boost::make_iterator_property_map(pred_map.begin(), boost::get(boost::vertex_index, g)))
        );

        if (dist_map[t]<INT_MAX) std::cout << dist_map[t] << std::endl;
        else std::cout << "unreachable" << std::endl;
    }
}
