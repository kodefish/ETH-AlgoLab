#include <iostream>
#include <map>
#include <climits>
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

    // Add capcities
    capacity_map[e] = cap;
    capacity_map[rev_e] = 0;

    // Add reverse edge
    reverse_map[e] = rev_e;
    reverse_map[rev_e] = e;
  }
};

void testcase() {
  // 0. Read in values
  int l, p; std::cin >> l >> p;

  // Read in locations + num garrisoned and num needed to defend
  std::vector<int> num_garrisoned(l), num_defenders(l);
  for(int i = 0; i < l; i++) {
    int g_i, d_i; std::cin >> g_i >> d_i;
    num_garrisoned[i] = g_i;
    num_defenders[i] = d_i;
  }

  // Track input graph, but keep weights seperate
  // Done so that it's easier to read incoming and outgoing edges for vertices
  Graph g(l + 2);
  int s = l;
  int t = s + 1;
  EdgeCapacityMap capacity_map = boost::get(boost::edge_capacity, g);
  EdgeReverseMap reverse_map = boost::get(boost::edge_reverse, g);

  // Create edge adder helper
  EdgeAdder ea(g, capacity_map, reverse_map);

  // Read in paths
  for(int i = 0; i < p; i++) {
    int f_i, t_i, min_i, max_i; std::cin >> f_i >> t_i >> min_i >> max_i;
    ea.add_edge(f_i, t_i, max_i - min_i);

    num_defenders[f_i] += min_i;
    num_garrisoned[t_i] += min_i;
  }

  // 1. For every vertex v
  // Compute s -> v, with c(s -> v) = g_v + sum(min(in(v))
  // Compute v -> t, with c(v -> t) = d_v + sum(min(v))
  int total_num_defenders = 0;
  for(Vertex v = 0; v < l; v++) {
    // 1.c add edge s -> v with capacity g_i + sum_min_in - sum_min_out
    ea.add_edge(s, v, num_garrisoned[v]);

    // 1.d add edge v -> with capacity d_i
    ea.add_edge(v, t, num_defenders[v]);
    total_num_defenders += num_defenders[v];
  }

  // 3. Compute (s, t)-flow f
  long f = boost::push_relabel_max_flow(g, s, t);

  // 4. If (f < sum d_v) no else yes
  if (f < total_num_defenders) {
    std::cout << "no" << std::endl;
  } else {
    std::cout << "yes" << std::endl;
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) testcase();
  return 0;
}
