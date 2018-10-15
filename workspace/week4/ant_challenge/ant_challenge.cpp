#include <iostream>
#include <vector>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef std::vector<int> int_vec;
typedef std::vector<int_vec> int_vec_2d;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
        boost::no_property,
        boost::property<boost::edge_weight_t, int>>        Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor      Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor        Edge;
typedef boost::graph_traits<Graph>::edge_iterator          EdgeIter;
typedef boost::property_map<Graph, boost::edge_weight_t>::type    WeightMap;

void testcase() {
    // Read Inputs
    int n, e, s; Vertex a, b; std::cin >> n >> e >> s >> a >> b;

    // Create a vector of graphs, one per species, and a corresponding weight graph
    std::vector<Graph> graphs(s, Graph(n));
    std::vector<WeightMap> weight_maps(s);
    for(int i = 0; i < e; ++i) {        // Read edge
        int u, v; std::cin >> u >> v;
        for(int j = 0; j < s; j++) {    // Read weights per species
            if (i == 0) {               // Init edge map if first iteration
                weight_maps[j] = boost::get(boost::edge_weight, graphs[j]);
            } 
            int w; std::cin >> w;
            Edge e; bool success;
            std::tie(e, success) = boost::add_edge(u, v, graphs[j]);
            if (success) {
                weight_maps[j][e] = w;
            }
        }
    }

    // Hives
    std::vector<Vertex> hives(s);
    for(int i = 0; i < s; i++) {    // Read hives per species
            std::cin >> hives[i];
    }

    // 1. Compute Prim's MSTs for every species to get private networks
    std::vector<std::vector<Vertex>> primpredmaps(s, std::vector<Vertex>(n));
    for(int i = 0; i < s; i++) {    // Read hives per species
        boost::prim_minimum_spanning_tree(graphs[i], 
            boost::make_iterator_property_map(primpredmaps[i].begin(), boost::get(boost::vertex_index, graphs[i])), 
            boost::root_vertex(hives[i]));
    }

    // 2. Combine MSTs to create network
    Graph combined_mst(n);
    WeightMap wm = boost::get(boost::edge_weight, combined_mst);
    for(int i = 0; i < s; i++) {
        std::vector<Vertex> specie_mst = primpredmaps[i];
        WeightMap specie_weight_map = weight_maps[i];    
        for(int j = 0; j < n; j++) {
            if (specie_mst[j] != j) {
                Edge e; bool s;
                std::tie(e, s) = boost::edge(j, specie_mst[j], graphs[i]);
                if (s) {
                    Edge e1; bool d;
                    std::tie(e1, d) = boost::add_edge(boost::source(e, graphs[i]), boost::target(e, graphs[i]), combined_mst);
                    if (d) {
                        wm[e1] = specie_weight_map[e];
                    }
                }
            }
        }
    }

    // 3. Compute shortest path in resulting combination to compute shortest path through private nets
    std::vector<Vertex> pred_map(n);
    std::vector<int> dist_map(n);
    
    boost::dijkstra_shortest_paths(combined_mst, a, 
        boost::predecessor_map(boost::make_iterator_property_map(pred_map.begin(), boost::get(boost::vertex_index, combined_mst))).
        distance_map(boost::make_iterator_property_map(dist_map.begin(), boost::get(boost::vertex_index, combined_mst))));
    
    std::cout << dist_map[b] << std::endl;
}

int main() {
    int t; std::cin >> t;
    for(int i = 0; i < t; ++i) {
        testcase();    
    }
}
