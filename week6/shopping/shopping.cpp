// Algolab BGL Tutorial 2 (Max flow, by taubnert@ethz.ch)
// Flow example demonstrating how to use push_relabel_max_flow using a custom edge adder
// to manage the interior graph properties required for flow algorithms
#include <iostream>
#include <vector>

// BGL include
#include <boost/graph/adjacency_list.hpp>

// BGL flow include
#include <boost/graph/push_relabel_max_flow.hpp>

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

// Custom edge adder class, highly recommended
class edge_adder {
    graph &G;

    public:
    explicit edge_adder(graph &G) : G(G) {}

    void add_edge(int from, int to, long capacity) {
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);
        const auto e = boost::add_edge(from, to, G).first;
        const auto rev_e = boost::add_edge(to, from, G).first;
        c_map[e] = capacity;
        c_map[rev_e] = 0; // reverse edge has no capacity!
        r_map[e] = rev_e;
        r_map[rev_e] = e;
    }
};

void testcase() {
    int n, m, s; std::cin >> n >> m >> s;
    // Add super source and sink
    graph G(n + 2);
    int v_source = n;
    int v_target = v_source + 1;
    edge_adder adder(G);

    // Count number of stores at each intersection
    std::vector<int> store_locations(n, 0);
    for (int i = 0; i < s; i++) {
        int store; std::cin >> store;
        store_locations[store]++;
    }

    // Read streets, represent them as edges in the graph with capacity 1 (can only be travelled once)
    for (int i = 0; i < m; i++) {
        int u, v; std::cin >> u >> v;
        adder.add_edge(u, v, 1);
        adder.add_edge(v, u, 1);
    }

    // Add edge from super source to home intersection with capacity s (since Rick wants to make s trips)
    adder.add_edge(v_source, 0, s);

    // Add edge from each intersection to the supersink with capacity = #stores on intersection
    for (int i = 0; i < n; i++) {
        if (store_locations[i] > 0) adder.add_edge(i, v_target, store_locations[i]);
    }

    // Compute flow and check that it matches s
    long flow = boost::push_relabel_max_flow(G, v_source, v_target);
    if (flow < s) std::cout << "no" << std::endl;
    else std::cout << "yes" << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}

