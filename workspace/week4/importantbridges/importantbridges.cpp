#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>

using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS,
            no_property,
            property<edge_index_t, int>>            Graph;      // Graph type
typedef graph_traits<Graph>::vertex_descriptor      Vertex;     // Vertex type 
typedef graph_traits<Graph>::edge_descriptor        Edge;       // Edge type
typedef graph_traits<Graph>::edge_iterator          EdgeItr;    // Edge iterator type
typedef property_map<Graph, edge_index_t>::type     EdgeMap;

bool cmp(std::pair<Vertex, Vertex> p1, std::pair<Vertex, Vertex> p2) {
    return p1.first == p2.first ? p1.second < p2.second : p1.first < p2.first;
}

void testcase() {
    // Read input
    int num_islands, num_bridges; std::cin >> num_islands >> num_bridges;

    Graph g(num_islands);
    for(int i = 0; i < num_bridges; i++) {
        int u, v; std::cin >> u >> v;
        Edge e; bool s;
        std::tie(e, s) = add_edge(u, v, g);
    }

    // Compute biconnected components and articulation points
    EdgeMap edge_map = get(edge_index, g);
    int num_bc = biconnected_components(g, edge_map);
    std::vector<int> num_bridges_per_component(num_bc, 0);
    std::map<int, Edge> important_bridges_map;

    // Count number of bridges in each biconnected component
    EdgeItr ebeg, eend;
    for(std::tie(ebeg, eend) = edges(g); ebeg != eend; ebeg++) {
        if (num_bridges_per_component[edge_map[*ebeg]] < 1) {
            important_bridges_map[edge_map[*ebeg]] = *ebeg;
        } else {
            important_bridges_map.erase(edge_map[*ebeg]);
        }
        num_bridges_per_component[edge_map[*ebeg]]++;
    }

    // Important bridges are in the singleton BCs
    std::vector<std::pair<Vertex, Vertex>> important_bridges;
    for(std::map<int, Edge>::iterator it=important_bridges_map.begin(); it!=important_bridges_map.end(); ++it) {
        Vertex u, v;
        u = source(it->second, g);
        v = target(it->second, g);
        important_bridges.push_back(u < v ? std::make_pair(u, v) : std::make_pair(v, u));
    }

    std::sort(important_bridges.begin(), important_bridges.end(), cmp);
    std::cout << important_bridges.size() << std::endl;
    for(int i = 0; i < important_bridges.size(); i++) {
        std::cout << important_bridges[i].first << " " << important_bridges[i].second << std::endl; 
    }



}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t--) testcase();
    return 0;
}
