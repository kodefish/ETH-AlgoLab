// ALGOLAB BGL Tutorial 3
// Flow example demonstrating
// - breadth first search (BFS) on the residual graph

// Compile and run with one of the following:
// g++ -std=c++11 -O2 bgl_residual_bfs.cpp -o bgl_residual_bfs ./bgl_residual_bfs
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 bgl_residual_bfs.cpp -o bgl_residual_bfs; ./bgl_residual_bfs

// Includes
// ========
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


void testcase() {
    int n, m; std::cin >> n >> m;

	// build graph
	graph G(n);
	edge_adder adder(G);
	auto rc_map = boost::get(boost::edge_residual_capacity, G);

	// add edges
    for (int i = 0; i< m; i++) {
        int a, b, c; std::cin >> a >> b >> c;
        adder.add_edge(a, b, c);
    }

    // Find src such that flow is maximal
    int min_flow = INT_MAX;
    int min_src = INT_MAX;
    int min_sink = INT_MAX;
    for (int i = 0; i < n; i++) {
        int src = i;
        int sink = (i + 1) % n;
        int flow = boost::push_relabel_max_flow(G, src, sink);
        if (flow < min_flow) {
            min_flow = flow;
            min_src = src;
            min_sink = sink;
        }
    }

	// Find a min cut via maxflow
    int flow = boost::push_relabel_max_flow(G, min_src, min_sink);

	// BFS to find vertex set S
	std::vector<int> vis(n, false); // visited flags
	std::queue<int> Q; // BFS queue (from std:: not boost::)
	vis[min_src] = true; // Mark the source as visited
    int visited = 1;
	Q.push(min_src);
	while (!Q.empty()) {
		const int u = Q.front();
		Q.pop();
		out_edge_it ebeg, eend;
		for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
			const int v = boost::target(*ebeg, G);
			// Only follow edges with spare capacity
			if (rc_map[*ebeg] == 0 || vis[v]) continue;
			vis[v] = true;
            visited++;
			Q.push(v);
		}
	}

    // Print flow
	std::cout << flow << std::endl;

	// Output S
	std::cout << visited << " ";
	for (int i = 0; i < n; ++i) {
		if (vis[i]) std::cout << i << " ";
	}
	std::cout << std::endl;
}


int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}
