#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
			      boost::no_property,
			      boost::property<boost::edge_capacity_t, long,
					      boost::property<boost::edge_residual_capacity_t, long,
							      boost::property<boost::edge_reverse_t, Traits::edge_descriptor>>>> Graph;
typedef Graph::vertex_descriptor Vertex;
typedef Graph::edge_descriptor Edge;
typedef boost::property_map<Graph, boost::edge_capacity_t>::type EdgeCapacityMap;
typedef boost::property_map<Graph, boost::edge_residual_capacity_t>::type ResidualCapacityMap;
typedef boost::property_map<Graph, boost::edge_reverse_t>::type ReverseEdgeMap;


class EdgeAdder {
  Graph &graph;
  EdgeCapacityMap &capacity_map;
  ReverseEdgeMap &rev_map;

public:
  EdgeAdder(Graph & g,
	    EdgeCapacityMap & capacity_map,
	    ReverseEdgeMap & rev_map):
    graph(g),
    capacity_map(capacity_map),
    rev_map(rev_map){}

  void add_edge(int u, int v, int capacity) {
    Edge e, rev_e; bool s;
    boost::tie(e, s) = boost::add_edge(u, v, graph);
    boost::tie(rev_e, s) = boost::add_edge(v, u, graph);

    if (s) { // If success, then edge is new -> init capacity and reverse edge
      // Set edge capacities
      capacity_map[e] = capacity;
      capacity_map[rev_e] = 0;
      
      // Set reverse edges
      rev_map[e] = rev_e;
      rev_map[rev_e] = e;
    } else { // If not, then edge is already there -> increment capacity
      ++capacity_map[e];
    }
  }
};

void testcase() {
  int n, m, s; std::cin >> n >> m >> s;

  // Init graph with an additional vertex, which will be the sink
  Graph g(n+1);
  EdgeCapacityMap capacity_map = boost::get(boost::edge_capacity, g);
  ResidualCapacityMap residual_map = boost::get(boost::edge_residual_capacity, g);
  ReverseEdgeMap rev_edge = boost::get(boost::edge_reverse, g);

  EdgeAdder ea(g, capacity_map, rev_edge);

  // Read in all the stores we want to visit
  for(int i = 0; i<s; i++) {
    int s_i; std::cin >> s_i;
    ea.add_edge(s_i, n, 1);
  }

  // Read in all the streets - read it in both ways, since we can walk a street once in either direction
  for(int i = 0; i < m; i++) {
    int u_i, v_i; std::cin >> u_i >> v_i;
    ea.add_edge(u_i, v_i, 1);
    ea.add_edge(v_i, u_i, 1);
  }

  // Compute max flow
  long flow = boost::push_relabel_max_flow(g, 0, n);


  if (flow < s) {
    std::cout << "no" << std::endl;
  } else {
    std::cout << "yes" << std::endl;
  }
    
}

int main() {
  int t; std::cin >> t;
  while(t--) testcase();
  return 0;
}
