// STL includes
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

// BGL graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
	boost::property<boost::edge_capacity_t, long,
		boost::property<boost::edge_residual_capacity_t, long,
			boost::property<boost::edge_reverse_t, traits::edge_descriptor> > > >	graph;
// Interior Property Maps
typedef	boost::graph_traits<graph>::edge_descriptor			edge_desc;
typedef	boost::graph_traits<graph>::out_edge_iterator			out_edge_it;
typedef	boost::graph_traits<graph>::edge_iterator			edge_it;

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};


// Main
void testcase() {
    int n, m, s, d; std::cin >> n >> m >> s >> d;

    graph G(2*n);
    edge_adder adder(G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);
    const int src = boost::add_vertex(G);
    const int sink = boost::add_vertex(G);

    // Add edges in between in and out vertices
    for (int i = 0; i < n; i++) {
        adder.add_edge(2*i, 2*i + 1, 1);
    }

    // Read in edges
    for (int i = 0; i < m; i++) {
        int u, v; std::cin >> u >> v;
        adder.add_edge(2*u + 1, 2*v, 1);
    }

    // Read in sources
    for (int i = 0; i < s; i++) {
        int s_i; std::cin >> s_i;
        adder.add_edge(src, 2*s_i, 1);
    }

    // Read in sinks
    for (int i = 0; i < d; i++) {
        int d_i; std::cin >> d_i;
        adder.add_edge(2 * d_i + 1, sink, 1);
    }

    std::cout << boost::push_relabel_max_flow(G, src, sink) << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}
