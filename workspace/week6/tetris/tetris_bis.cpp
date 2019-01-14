// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>


// BGL Graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
	boost::property<boost::edge_capacity_t, long,
		boost::property<boost::edge_residual_capacity_t, long,
			boost::property<boost::edge_reverse_t, Traits::edge_descriptor> > > >	Graph;
// Interior Property Maps
typedef	boost::property_map<Graph, boost::edge_capacity_t>::type		EdgeCapacityMap;
typedef	boost::property_map<Graph, boost::edge_residual_capacity_t>::type	ResidualCapacityMap;
typedef	boost::property_map<Graph, boost::edge_reverse_t>::type		ReverseEdgeMap;
typedef	boost::graph_traits<Graph>::vertex_descriptor			Vertex;
typedef	boost::graph_traits<Graph>::edge_descriptor			Edge;
typedef	boost::graph_traits<Graph>::edge_iterator			EdgeIt;


// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
class EdgeAdder {
	Graph &G;
	EdgeCapacityMap	&capacitymap;
	ReverseEdgeMap	&revedgemap;

public:
	// to initialize the Object
	EdgeAdder(Graph & G, EdgeCapacityMap &capacitymap, ReverseEdgeMap &revedgemap):
		G(G), capacitymap(capacitymap), revedgemap(revedgemap){}

	// to use the Function (add an edge)
	void addEdge(int from, int to, long cap) {
		Edge e, rev_e;
		bool success;
		boost::tie(e, success) = boost::add_edge(from, to, G);
		boost::tie(rev_e, success) = boost::add_edge(to, from, G);
		capacitymap[e] = cap;
		capacitymap[rev_e] = 0; // reverse edge has no capacity!
		revedgemap[e] = rev_e;
		revedgemap[rev_e] = e;
	}
};


// Functions
// =========
// Function for an individual testcase
void testcases() {
    // Read input
    int w, n; std::cin >> w >> n;
    
	// Create Graph and Maps
	Graph G(2*w);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);
    
    // Give every vertex a capacity of one
    for(int i = 1; i < w; i++) {
        eaG.addEdge(i, i + w - 1, 1);
    }

	// Add edges
    std::map<std::pair<int, int>, int> edges;
    for(int i = 0; i < n; i++) {
        int s, t; std::cin >> s >> t; 
        int start = std::min(s, t), target = std::max(s, t);
        // Remap start to duplicate
        if (start > 0 && start < w) {
            start += w - 1; 
        }
        if (target == w) {
            target += w - 1;
        }

        // Count number of same edges
        edges[std::make_pair(start, target)]++;
    }

    for(std::map<std::pair<int, int>, int>::iterator i = edges.begin(); i != edges.end(); i++ ) {
        eaG.addEdge(i->first.first, i->first.second, i->second);
    }


	// Calculate flow
    std::cout << boost::push_relabel_max_flow(G, 0, 2*w - 1) << std::endl;
}

// Main function to loop over the testcases
int main() {
	std::ios_base::sync_with_stdio(false);
	int T;	std::cin >> T;
	while (T--) testcases();
	return 0;
}

