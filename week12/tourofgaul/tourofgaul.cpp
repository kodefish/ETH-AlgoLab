// STL includes
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
    edge_desc add_edge(int from, int to, long capacity, long cost) {
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
        return e;
    }
};

void testcase() {
    int n, m; std::cin >> n >> m;

    // Create graph, edge adder class and propery maps
    graph G(n);
    edge_adder adder(G);  
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);
    auto w_map = boost::get(boost::edge_weight, G);

    const int v_source = boost::add_vertex(G);
    const int v_target = boost::add_vertex(G);
    const int v_super_source = boost::add_vertex(G);
    const int MAX_WEIGHT = 128; // pdf -> 2^7

    const edge_desc super_src_e = adder.add_edge(v_super_source, v_source, INT_MAX, 0);

    int last_c = 0;
    for (int i = 0; i < n; i++) {
        int c = 0;
        if (i < n - 1) {
            std::cin >> c;
            adder.add_edge(i, i + 1, c, MAX_WEIGHT);
            // If outgoing capacity is greater than incoming, than ensure that c is met by adding c - last_c from the source
            // c = (c - last_c) + last_c = c
            if (c > last_c) adder.add_edge(v_source, i, c - last_c, MAX_WEIGHT * i); // min supply to vertex trick
        }

        if (i > 0) {
            // If outgoing capacity is greater than the incoming, then redirect the surplus to the sink (by last_c - c)
            // c = last_c - (last_c - c) = c
            if (c < last_c || i == n - 1) adder.add_edge(i, v_target, last_c - c, MAX_WEIGHT * ((n - 1) - i)); // max out is c2, rest goes to sink
        }

        last_c = c;
    }

    for (int i = 0; i < m; i++) {
        int a, b, d; std::cin >> a >> b >> d;

        // Trick to shift negative weights -> all s-t paths shifted by same amount
        int cost = ((b - a) * MAX_WEIGHT) -d;

        // Get rid of parallel edges -> gets the last 10 points
        // So check if same edge already exists, and if it does, increase it's capapcity
        out_edge_it ebeg, eend; bool found = false;
        for (boost::tie(ebeg, eend) = boost::out_edges(a, G); ebeg != eend; ebeg++) {
            if (boost::target(*ebeg, G) == b) {
                if (w_map[*ebeg] == cost) {
                    c_map[*ebeg]++;
                    found = true;
                    break;
                }
            }
        }
        if (found) continue;

        adder.add_edge(a, b, 1, cost);
    }

    boost::successive_shortest_path_nonnegative_weights(G, v_super_source, v_target);
    int cost = boost::find_flow_cost(G);
    // Just a fancy way of computing the flow
    int flow = c_map[super_src_e] - rc_map[super_src_e];
    // (n - 1) is the max length of an s-t path
    std::cout << ((n - 1) * MAX_WEIGHT * flow) - cost << std::endl;
}


int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}

