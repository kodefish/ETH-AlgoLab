#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
			      boost::no_property,
			      boost::property<boost::edge_capacity_t, long,
					      boost::property<boost::edge_residual_capacity_t, long,
							      boost::property<boost::edge_reverse_t, Traits::edge_descriptor>>>> Graph;
typedef Graph::edge_descriptor Edge;
typedef Graph::vertex_descriptor Vertex;
typedef boost::property_map<Graph, boost::edge_capacity_t>::type EdgeCapacityMap;
typedef boost::property_map<Graph, boost::edge_residual_capacity_t>::type ResidualCapacityMap;
typedef boost::property_map<Graph, boost::edge_reverse_t>::type ReverseEdgeMap;

class EdgeAdder {
  Graph &G;
  EdgeCapacityMap &capacity_map; 
  ReverseEdgeMap &revedge_map;

public:
  // Init object
  EdgeAdder(Graph & G,
	    EdgeCapacityMap & capacity_map,
	    ReverseEdgeMap & revedge_map):
    G(G),
    capacity_map(capacity_map),
    revedge_map(revedge_map){}

  // Use helper to add edges
  void addEdge(int from, int to, int capacity) {
    //std::cout << "adding edge " << from << " -" << capacity << "-> " << to << std::endl;
    Edge e, rev_e; bool s;
    boost::tie(e, s) = boost::add_edge(from, to, G);
    boost::tie(rev_e, s) = boost::add_edge(to, from, G);
    capacity_map[e] = capacity;
    capacity_map[rev_e] = 0;
    revedge_map[e] = rev_e;
    revedge_map[rev_e] = e;
  }
};

void testcase() {
  int n, m; std::cin >> n >> m;
  // Define source and sink vertices;
  int s = n + m;
  int t = s + 1;
  //std::cout << n << " " << m << " " << s << " " << t << std::endl;
  // Create graph with one vertex per player, round and 2 additional ones for s and t used in flow
  Graph graph(n + m + 2); 
  EdgeCapacityMap capacity_map = boost::get(boost::edge_capacity, graph);
  ReverseEdgeMap revedge_map = boost::get(boost::edge_reverse, graph);
  ResidualCapacityMap residual_capacity = boost::get(boost::edge_residual_capacity, graph);
  EdgeAdder edgeAdder(graph, capacity_map, revedge_map);
  // Read rounds
  for(int i = 0; i < m; i++) {
    int a, b, c; std::cin >> a >> b >> c;
    // Calculate vertice corresponding to round
    int r = n + i;
    // Add edge r -> t with capacity 1
    edgeAdder.addEdge(r, t, 1);
    
    // If a won or not known, add edge a -> r
    if (c < 2)
      edgeAdder.addEdge(a, r, 1);

    // If b won or not known, add edge b -> r
    if (c == 2 || c == 0) 
      edgeAdder.addEdge(b, r, 1);
  }

  // Read in scores
  int score_sum = 0;
  for(int i = 0; i < n; i++) {
    int s_i; std::cin >> s_i;
    score_sum += s_i;
    if (s_i > 0) {
      edgeAdder.addEdge(s, i, s_i);
    }
  }

  long flow = boost::push_relabel_max_flow(graph, s, t);

  if (flow == std::max(m, score_sum)) {
    std::cout << "yes" << std::endl;
  } else {
    std::cout << "no" << std::endl;
  }
}

int main() {
  int t; std::cin >> t;
  while(t--) testcase();
  return 0;
}


