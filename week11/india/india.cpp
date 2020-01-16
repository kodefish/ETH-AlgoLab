// Includes
// ========
#include <iostream>
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
typedef boost::graph_traits<graph>::vertex_descriptor           vertex_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator
typedef boost::property_map<graph, boost::edge_residual_capacity_t>::type rc_map;

// Custom edge adder class
class edge_adder {
    graph &G;

    public:
    explicit edge_adder(graph &G) : G(G) {}
    std::pair<edge_desc, edge_desc> add_edge(int from, int to, long capacity, long cost) {
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
        return std::make_pair(e, rev_e);
    }

    void mutate_edge(std::pair<edge_desc, edge_desc> pair, long c, long w) {
        auto c_map = boost::get(boost::edge_capacity, G);
        auto w_map = boost::get(boost::edge_weight, G); // new!
        edge_desc e = pair.first;
        edge_desc rev_e = pair.second;
        c_map[e] = c;
        w_map[e] = w; // new!
        c_map[rev_e] = 0;
        w_map[rev_e] = -w; // new
    }
};

void testcase() {
    int n, g, b, k, a; std::cin >> n >> g >> b >> k >> a;

    // Create graph, edge adder class and propery maps
    graph G(n + 2);
    edge_adder adder(G);
    rc_map rc_map = boost::get(boost::edge_residual_capacity, G);

    // Add the edges
    for (int i = 0; i < g; i++) {
        int x, y, d, e; std::cin >> x >> y >> d >> e;
        adder.add_edge(x, y, e, d);
    }

    vertex_desc start = n;
    vertex_desc finish = start + 1;

    auto es = adder.add_edge(start, k, 1, 0);
    auto ef = adder.add_edge(a, finish, 1, 0);

    // Idea is to use exponential search to find the upper bound on the flow
    int upper = 1;
    while (true) {
        adder.mutate_edge(es, upper, 0);
        adder.mutate_edge(ef, upper, 0);

        boost::successive_shortest_path_nonnegative_weights(G, start, finish);
        int res_flow = rc_map[es.first];
        if (res_flow > 0) break;

        int cost = boost::find_flow_cost(G);
        if (cost > b) break;
        upper *= 2;
    }

    std::cerr << upper << std::endl;

    // Binary search for the optimal amount of flow
    int lower = 0;
    while (lower != upper) {
        int m = (upper + lower)/2;

        adder.mutate_edge(es, m, 0);
        adder.mutate_edge(ef, m, 0);

        boost::successive_shortest_path_nonnegative_weights(G, start, finish);
        int cost = boost::find_flow_cost(G);
        if (rc_map[es.first] > 0 || cost > b) {
            upper = m;
        } else {
            lower = m + 1;
        }

    }
    std::cout << lower - 1 << std::endl;

}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}

