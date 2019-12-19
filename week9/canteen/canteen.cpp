// Includes
// ========
#include <iostream>
#include <vector>
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
    int n; std:: cin >> n;

    // Create graph, edge adder class and propery maps
    graph G(n + 2);
    edge_adder adder(G);
    const int v_source = n;
    const int v_target = v_source + 1;

    // Read in menus
    // Save amounts as we need them later
    for (int i = 0; i < n; i++) {
        int a, c; std::cin >> a >> c;
        adder.add_edge(v_source, i, a, c);
    }

    // Read in hungry students
    int num_hungry_hippos = 0;
    const int MAX_PROFIT = 20;
    for (int i = 0; i < n; i++) {
        int s, p; std::cin >> s >> p;
        adder.add_edge(i, v_target, s, MAX_PROFIT - p);
        num_hungry_hippos += s;
    }

    // For days with freezer leftovers and freezer space
    for (int i = 1; i < n; i++) {
        int v, e; std::cin >> v >> e;
        adder.add_edge(i-1, i, v, e); // from counter to students
    }

    // Min Cost Max Flow with cycle_canceling
    int flow = boost::push_relabel_max_flow(G, v_source, v_target);
    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
    int cost = boost::find_flow_cost(G);
    int profit = (MAX_PROFIT * flow) - cost;
    if (flow < num_hungry_hippos)
        std::cout << "impossible" << " ";
    else
        std::cout << "possible" << " ";
    std::cout << flow << " " << profit << std::endl;

}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}
