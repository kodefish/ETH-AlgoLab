#include <iostream>
#include <map>
#include <boost/graph/adjacency_list.hpp>

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

    // Add capcities
    capacity_map[e] = cap;
    capacity_map[rev_e] = 0;

    // Add reverse edge
    reverse_map[e] = rev_e;
    reverse_map[rev_e] = e;
  }
};

void testcase() {
  // Read input
  int l, p; std::cin >> l >> p;

  // Input graph (num locations + source + target)
  Graph input_graph(l + 2);
  std::map<Edge, int> min_capacity_map;
  std::map<Edge, int> max_capacity_map;
  int s = l;
  int t = s + 1;

  // Read locations
  for(int i = 0; i < l; i++) {
    int g_i, d_i; std::cin >> g_i >> d_i;
    // insert s -> i edge
    Edge e_s, e_t; bool success;
    boost::tie(e_s, success) = boost::add_edge(s, i, input_graph);
    min_capacity_map[e_s] = 0; // min num of soldiers is 0
    max_capacity_map[e_s] = g_i; // location i can supply g_i soldiers
    boost::tie(e_t, success) = boost::add_edge(i, t, input_graph);
    min_capacity_map[e_t] = 0; // min num of soldiers is 0
    max_capacity_map[e_t] = d_i; // location i needs d_i soldiers to defend itself
  }

  // Read paths
  for(int i = 0; i < p; i++) {
    int f_i, t_i, min_i, max_i; std::cin >> f_i >> t_i >> min_i >> max_i; 
    Edge e; bool success;
    boost::tie(e, success) = boost::add_edge(f_i, t_i, input_graph); // Path i from f_i to t_i
    min_capacity_map[e] = min_i;
    max_capacity_map[e] = max_i;
  }

  // Reduce problem to max-flow problem
  // Add new source and target s' and t'
  Graph g(l + 2 + 2);
  int s_p = t + 1;
  int t_p = s_p + 1;

  // Define accessors for interior proprety maps
  EdgeCapacityMap capacity_map = boost::get(boost::edge_capacity, g);
  ResidualCapacityMap residual_map = boost::get(boost::edge_residual_capacity, g);
  EdgeReverseMap reverse_map = boost::get(boost::edge_reverse, g);

  EdgeAdder ea(g, capacity_map, reverse_map);

  // For every vertex v in input_graph
  Graph::vertex_iterator viter, vend;
  for(boost::tie(viter, vend) = boost::vertices(input_graph); viter != vend; viter++){
    Vertex v = *viter;
    // Compute sum l of incoming lower bounds
    // Add edge s' -> v with capacity l
    // Compute sum u of outgoing lower bounds
    // Add edge v -> t' with capacity u
  }

  // For every edge e in input_graph
  Graph::edge_iterator eiter, eend;
  for(boost::tie(eiter, eend) = boost::edges(input_graph); eiter != eend; eiter++){
    Edge e = *eiter;
    // Add edge u -> v with capacity c = u(u -> v) - l(u -> v)
    int min_cap = min_capacity_map[e];
    int max_cap = max_capacity_map[e];
    int e_cap = max_cap - min_cap;

    ea.add_edge(boost::source(e), boost::target(e), e_cap);
  }

  // Add edge t' -> s' with capacity INFINITY

  // Compute flow f on new graph

  // If f < sum of all lowerbounds output no
  // else output yes
  
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) testcase();
  return 0;
}
