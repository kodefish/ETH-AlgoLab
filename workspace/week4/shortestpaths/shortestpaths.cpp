#include <iostream>
#include <vector>
#include <climits>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
			      boost::no_property,
			      boost::property<boost::edge_weight_t, int>> Graph;
typedef Graph::vertex_descriptor Vertex;
typedef Graph::edge_descriptor Edge;
typedef boost::property_map<Graph, boost::edge_weight_t>::type EdgeWeightMap;

int main() {
  int n, m, q;
  std::cin >> n >> m >> q;

  Graph g(n);
  EdgeWeightMap cap = boost::get(boost::edge_weight, g);
  for (int i = 0; i < n; i++) {
    int lon, lat; std::cin >> lon >> lat;
  }

  for (int i = 0; i < m; i++) {
    int a, b, c; std::cin >> a >> b >> c;
    Edge e; bool s;
    boost::tie(e, s) = boost::add_edge(a, b, g);
    cap[e] = c;
  }

  for (int i = 0; i < q; i++) {
    int s, t; std::cin >> s >> t;
    std::vector<int> dist(n);
    boost::dijkstra_shortest_paths(g, s,
				   distance_map(boost::make_iterator_property_map(dist.begin(), boost::get(boost::vertex_index, g))));
    if (dist[t] < INT_MAX) {
      std::cout << dist[t] << std::endl;
    } else {
      std::cout << "unreachable" << std::endl;
    }
  }
  
}
