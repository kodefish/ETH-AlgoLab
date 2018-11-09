#include <iostream>
#include <map>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>


typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::bidirectionalS> Traits;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS,
			      boost::no_property,
			      boost::property<boost::edge_capacity_t, long,
					      boost::property<boost::edge_residual_capacity_t, long,
							      boost::property<boost::edge_reverse_t, Traits::edge_descriptor>>>> Graph;
typedef Graph::vertex_descriptor Vertex;
typedef Graph::edge_descriptor Edge;
typedef boost::property_map<Graph, boost::edge_capacity_t>::type EdgeCapacityMap;
typedef boost::property_map<Graph, boost::edge_residual_capacity_t>::type ResidualCapacityMap;
typedef boost::property_map<Graph, boost::edge_reverse_t>::type EdgeReverseMap;

class EdgeAdder {
  Graph &g;
  EdgeCapacityMap &capacity_map;
  EdgeReverseMap &reverse_map;

public:
  EdgeAdder(Graph &g,
	    EdgeCapacityMap &capacity_map,
	    EdgeReverseMap &reverse_map)
    :g(g), capacity_map(capacity_map), reverse_map(reverse_map) {}

  void add_edge(int u, int v, int cap) {
    Edge e, rev_e; bool s;
    boost::tie(e, s) = boost::add_edge(u, v, g);
    boost::tie(rev_e, s) = boost::add_edge(v, u, g);

    // If new edge
    // Add capcities
    capacity_map[e] = cap;
    capacity_map[rev_e] = 0;

    // Add reverse edge
    reverse_map[e] = rev_e;
    reverse_map[rev_e] = e;
  }
};

void testcase() {
  // Read in width of playing field and number of blocks
  int w, n; std::cin >> w >> n;

  // Graph contains all the possible locations [0, w]
  Graph g(2*w + 1);

  // Init flow stuff
  EdgeCapacityMap cap_map = boost::get(boost::edge_capacity, g);
  EdgeReverseMap rev_map = boost::get(boost::edge_reverse, g);

  // Init graph helper
  EdgeAdder ea(g, cap_map, rev_map);

  // Limit cap of each vertice to 1
  for(int i = 1; i < w; i++) {
    ea.add_edge(i, i + (w - 1), 1);
  }

  std::map<std::pair<int, int>, int> cracks;
  // Read in the blocks
  for(int i = 0; i < n; i++) {
    // Read in the block locations
    int loc_a, loc_b; std::cin >> loc_a >> loc_b;
    int min = std::min(loc_a, loc_b), max = std::max(loc_a, loc_b);
    if (min != 0 && min != w) {
      min = min + (w - 1);
    }
    if (max == w) {
      max = (2 * w) - 1;
    }
    cracks[std::make_pair(min, max)]++;
  }

  for(std::map<std::pair<int, int>, int>::iterator it = cracks.begin(); it != cracks.end(); it++) {
    std::pair<int, int> edge = it->first;
    int cap =  it->second;
    ea.add_edge(edge.first, edge.second, cap);
  }

  // Call flow algo
  long flow = boost::push_relabel_max_flow(g, 0, (2*w) - 1);

  std::cout << flow << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) testcase();
  return 0;
}
