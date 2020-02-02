// This solution is only valid for the first testcase. However, it provides the general idea of the algorithm, before
// having to shift to nonnegative weights
// Includes
// ========
#include <iostream>
#include <climits>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

// Custom edge adder class
class edge_adder {
    graph &G;

    public:
    explicit edge_adder(graph &G) : G(G) {}
    void add_edge(int from, int to, long capacity, long cost) {
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);
        auto w_map = boost::get(boost::edge_weight, G); // new!
        const edge_desc e = boost::add_edge(from, to, G).first;
        const edge_desc rev_e = boost::add_edge(to, from, G).first;
        c_map[e] = capacity;
        c_map[rev_e] = 0; // reverse edge has no capacity!
        r_map[e] = rev_e;
        r_map[rev_e] = e;
        w_map[e] = cost;   // new assign cost
        w_map[rev_e] = -cost;   // new negative cost
    }
};

void testcase() {
    int n, m; std::cin >> n >> m;

    // Create graph, edge adder class and propery maps
    graph G(n);
    edge_adder adder(G);  

    const int v_source = boost::add_vertex(G);
    const int v_target = boost::add_vertex(G);
    const int v_super_source = boost::add_vertex(G);

    adder.add_edge(v_super_source, v_source, INT_MAX, 0);

    int last_c = 0;
    for (int i = 0; i < n; i++) {
        int c = 0;
        if (i < n - 1) {
            std::cin >> c;
            adder.add_edge(i, i + 1, c, 0); // add capacity to next stop
            // If outgoing capacity is greater than incoming, than ensure that c is met by adding c - last_c from the source
            // c = (c - last_c) + last_c = c
            if (c > last_c) adder.add_edge(v_source, i, c - last_c, 0); // min supply to vertex trick
        }

        if (i > 0) {
            // If outgoing capacity is greater than the incoming, then redirect the surplus to the sink (by last_c - c)
            // c = last_c - (last_c - c) = c
            if (c < last_c || i == n - 1) adder.add_edge(i, v_target, last_c - c, 0); // max out is c2, rest goes to sink
        }

        last_c = c;
    }

    for (int i = 0; i < m; i++) {
        int a, b, d; std::cin >> a >> b >> d;
        int cost = -d;
        adder.add_edge(a, b, 1, cost);
    }

    boost::push_relabel_max_flow(G, v_super_source, v_target);
    boost::cycle_canceling(G);
    int cost = boost::find_flow_cost(G);
    std::cout << -cost << std::endl;
}


int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}
