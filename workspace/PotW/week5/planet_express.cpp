#include <iostream>
#include <vector>
#include <set>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
            boost::no_property,
            boost::property<boost::edge_weight_t, double>>   Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor   Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor     Edge;
typedef boost::property_map<Graph, boost::edge_weight_t>::type WeightMap;

const int sec_in_micro = 1000000;

void testcase() {
    // Num planets (vertices), num edges, num warehouses, num teleportation hubs
    int n, m, k, t; std::cin >> n >> m >> k >> t;

    // Init reverse universe graph
    Graph universe_r(n);
    std::set<int> teleportation_vertices;
    WeightMap weight_map = boost::get(boost::edge_weight, universe_r);

    // Read in teleportation network
    for(int i = 0; i < t; i++) {
        int t_i; std::cin >> t_i; 
        teleportation_vertices.insert(t_i);
    }

    // Read in edges
    for(int i = 0; i < m; i++) {
        int u, v, c; std::cin >> u >> v >> c;
        Edge e; bool s;
        // Insert reverse edge
        boost::tie(e, s) = add_edge(v, u, universe_r);
        if (s) { // If insert successfull
            weight_map[e] = 2*c;
        }
    }

    // Compute connected components
    std::vector<int> scc(n);
    int num_scc = boost::strong_components(universe_r,
        boost::make_iterator_property_map(scc.begin(), boost::get(boost::vertex_index, universe_r)));

    // Compute map cc -> list of vertices s.t. all vertices are in the teleportation network
    std::map<int, std::vector<int>> cc_verts;
    for(auto t_i : teleportation_vertices) {
        cc_verts[scc[t_i]].push_back(t_i);
    }

    for(int cc = 0; cc < num_scc; cc++) {
        if(cc_verts[cc].size() > 0) {
            std::vector<int> reachable = cc_verts[cc];
            for(int i = 0; i < reachable.size(); i++) {
                int u = reachable[i];
                Edge e; bool s;
                boost::tie(e, s) = boost::add_edge(u, n + cc, universe_r);
                if (s) { // If insert successfull
                    weight_map[e] = reachable.size() - 1;
                }
                boost::tie(e, s) = boost::add_edge(n + cc, u, universe_r);
                if (s) { // If insert successfull
                    weight_map[e] = reachable.size() - 1;
                }
            }
        }
    }

    std::vector<int> pred_map(n + num_scc);
    std::vector<int> dist_map(n + num_scc);       // Dijkstra distance map
    Vertex start = n - 1;
    boost::dijkstra_shortest_paths(universe_r, start,
                    boost::predecessor_map(boost::make_iterator_property_map(pred_map.begin(), boost::get(boost::vertex_index, universe_r))).
                    distance_map(boost::make_iterator_property_map(dist_map.begin(), boost::get(boost::vertex_index, universe_r))));

    int min_dist = sec_in_micro + 1;
    int min_idx = -1;
    for(int i = 0; i < k; i++) {
        int dist_i = dist_map[i]/2;
        if (dist_i < min_dist) {
            min_dist = dist_i;
            min_idx = i;
        } 
    }

    if (min_dist > sec_in_micro) {
        std::cout << "no" << std::endl;
    } else {
        std::cout << min_dist << std::endl;
    }

}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t--) testcase();
    return 0;
}
