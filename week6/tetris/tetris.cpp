// Algolab BGL Tutorial 2 (Max flow, by taubnert@ethz.ch)
// Flow example demonstrating how to use push_relabel_max_flow using a custom edge adder
// to manage the interior graph properties required for flow algorithms
#include <iostream>

// BGL include
#include <boost/graph/adjacency_list.hpp>

// BGL flow include *NEW*
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
    int w, n; std::cin >> w >> n;
    // There are w+1 vertices (0 to w)
    // For each, we have v_in and v_out, and between the two we have an edge of capacity 1
    graph G(2*(w + 1));
    edge_adder adder(G);

    // Add capacity of 1 to all the vertices that do not touch the walls (we can only use them once)
    for (int i = 1; i < w; i++) {
        adder.add_edge(2*i, (2*i) + 1, 1);
    }

    // Add the blocks
    for (int i=0; i < n; i++) {
        int a, b; std::cin >> a >> b;
        if (a < b) {
            // If a is the left side, add edge from a_out to b_in
            adder.add_edge((2 * a) + 1, 2 * b, 1);
        } else {
            adder.add_edge((2 * b) + 1, 2 * a, 1);
        }
    }

    // Calculate flow from 1 (0_out) to 2*w (2w_in)
    long flow = boost::push_relabel_max_flow(G, 1, 2*w);
    std::cout << flow << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}
