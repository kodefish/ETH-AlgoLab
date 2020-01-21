// STL includes
#include <iostream>
#include <vector>
#include <climits>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::property<boost::edge_weight_t, int> edge_weight;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
                              boost::no_property, edge_weight>
    weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;

std::vector<int> dijkstra_dists(const weighted_graph &G, int start) {
    int n = boost::num_vertices(G);
    std::vector<int> dist_map(n);

    boost::dijkstra_shortest_paths(G, start,
                                   boost::distance_map(boost::make_iterator_property_map(
                                       dist_map.begin(), boost::get(boost::vertex_index, G))));
    return dist_map;
}

int vertex_index(const int i, const int j, const int k) {
    if (i > j || i < 1 || i < 1 || j > k) {
        return -1;
    }

    return (((j - 1) * j) / 2) + i - 1;
};

void testcase() {
    int k; std::cin >> k;
    weighted_graph G((k * (k + 1)));
    std::vector<int> weights((k * (k + 1)) / 2);

    for (int y = 1; y <= k; y++) {
        for (int x = 1; x <= y; x++) {
            int idx = vertex_index(x, y, k);
            std::cin >> weights[idx];

            int inVertex = 2 * idx;
            int outVertex = inVertex + 1;
            boost::add_edge(inVertex, outVertex, edge_weight(weights[idx]), G);

            std::vector<int> neighbors;
            neighbors.push_back(vertex_index(x - 1, y, k));
            neighbors.push_back(vertex_index(x + 1, y, k));

            neighbors.push_back(vertex_index(x - 1, y - 1, k));
            neighbors.push_back(vertex_index(x, y - 1, k));

            neighbors.push_back(vertex_index(x, y + 1, k));
            neighbors.push_back(vertex_index(x + 1, y + 1, k));

            for (auto v : neighbors) {
                if (v != -1) boost::add_edge(outVertex, 2 * v, edge_weight(0), G);
            }
        }
    }

    const int v11 = vertex_index(1, 1, k);
    const int vk1 = vertex_index(1, k, k);
    const int vkk = vertex_index(k, k, k);

    const auto dists_v11 = dijkstra_dists(G, 2 * v11);
    const auto dists_vk1 = dijkstra_dists(G, 2 * vk1);
    const auto dists_vkk = dijkstra_dists(G, 2 * vkk);

    int best = INT_MAX;
    for (int y = 1; y <= k; y++) {
        for (int x = 1; x <= y; x++) {
            int idx = vertex_index(x, y, k);
            int weight = weights[idx];

            int sum = dists_v11[2 * idx] + dists_vk1[2 * idx] + dists_vkk[2 * idx] + weight;
            best = std::min(best, sum);
        }
    }

    std::cout << best << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}

