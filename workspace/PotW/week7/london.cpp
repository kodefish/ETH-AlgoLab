#include <iostream>
#include <string>
#include <map>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
			      boost::no_property,
			      boost::property<boost::edge_capacity_t, long,
					      boost::property<boost::edge_residual_capacity_t, long,
							      boost::property<boost::edge_reverse_t, Traits::edge_descriptor>>>> Graph;
typedef Graph::edge_descriptor Edge;
typedef boost::property_map<Graph, boost::edge_capacity_t>::type EdgeCapacityMap;
typedef boost::property_map<Graph, boost::edge_residual_capacity_t>::type ResidualCapacityMap;
typedef boost::property_map<Graph, boost::edge_reverse_t>::type ReverseEdgeMap;

class EdgeAdder {
  Graph &g;
  EdgeCapacityMap &cap_map;
  ReverseEdgeMap &rev_map;

public:
  EdgeAdder(
	    Graph &g,
	    EdgeCapacityMap &cap_map,
	    ReverseEdgeMap &rev_map):
    g(g),
    cap_map(cap_map),
    rev_map(rev_map){}

  void add_edge(int u, int v, long cap) {
    Edge e, rev_e; bool s;
    // Add edges
    boost::tie(e, s) = boost::add_edge(u, v, g);
    boost::tie(rev_e, s) = boost::add_edge(v, u, g);

    // Add capacities
    cap_map[e] = cap;
    cap_map[rev_e] = 0;

    // Add reverse edge
    rev_map[e] = rev_e;
    rev_map[rev_e] = e;
  }
};

void testcase() {
  // Read in height and width
  int h, w; std::cin >> h >> w;

  // Read in note
  std::string note; std::cin >> note;

  // Read in first page
  std::vector<char> first_page(h*w);
  for(int i = 0; i < h; i++) {
    for(int j = 0; j < w; j++) {
      char c_i; std::cin >> c_i;
      first_page[i*w + j] = c_i;
    }
  }

  // Read in second page, and count the number of occurence for each pair
  std::map<std::pair<char, char>, int> occ_pair;
  for(int i = 0; i < h; i++) {
    for(int j = 0; j < w; j++) {
      // i*h -> gives the row
      // w - 1 - j -> instead of having elems read in 0, 1, ..., w-1
      //              read them in as w-1, w-2, ...., 0
      char first = first_page[i*w + ((w - 1) - j)];
      char second; std::cin >> second;
      occ_pair[std::make_pair(first, second)]++;
    }
  }

  // Count the number of occurences of every letter in the note
  std::map<char, int> occ;
  for(int i = 0; i < note.size(); i++) {
    occ[note[i]]++;
  }

  // Graph has some number of distinct pairs of letters (each pair is a vertex), one vertex per distinct letter
  // of the note, and source and target. The vertex 0 is to direct letters that are not present in the note
  // So number of vertices: 1 + occ.size() + occ_pair.size() + 2
  Graph g(1 + occ.size() + occ_pair.size() + 2);
  int s = 1 + occ.size() + occ_pair.size();
  int t = s + 1;

  // Interior property maps
  EdgeCapacityMap cap_map = boost::get(boost::edge_capacity, g);
  ReverseEdgeMap rev_map = boost::get(boost::edge_reverse, g);

  // Edge adder helper
  EdgeAdder ea(g, cap_map, rev_map);

  // For every disticnt letter c in note, add edge c -> t with cap occ[c]
  // When adding to graph, map c to an int in [0, occ.size()]
  // We start at vertex_idx = 1 because as discussed above, 0 is used for letters not present in the note
  int vertex_idx = 1;
  std::map<char, int> letter_to_vertex;
  for(std::map<char, int>::iterator it = occ.begin(); it != occ.end(); it++) {
    // Save vertex number corresponding to distinct letter
    letter_to_vertex[it->first] = vertex_idx++;
    // Add edge c -> t with cap occ[c]
    ea.add_edge(letter_to_vertex[it->first], t, occ[it->first]);
  }


  // For every letter pair (c1, c2)
  for(std::map<std::pair<char, char>, int>::iterator occ_it = occ_pair.begin(); occ_it != occ_pair.end(); occ_it++) {
    std::pair<char, char> pair = occ_it->first;
    char first = pair.first;
    char second = pair.second;
    int num_occurence = occ_it->second;
    // Every pair is represented by vertex occ.size() + i,
    // where occ.size() is the number of distinct letters in the note and
    // i is it's position on a page
    // add edge (c1, c2) -> c1 with cap 1. Only insert if one of the two in the pair
    // is part of the note string
    // All recurring pairs are aggregated into one node with capacity = num reccurences
    // Here the beauty of map in C++ comes in. If letter_to_vertex[letter] has no key-value pair,
    // it will return the default, which in our case will be 0. So all these letters get redirected to vertex 0
    ea.add_edge(vertex_idx, letter_to_vertex[first], num_occurence);

    // add edge (c1, c2) -> c2 with cap 1, if it goes to a required letter AND if it is different
    // from c1
    ea.add_edge(vertex_idx, letter_to_vertex[second], num_occurence);

    //     add edge s -> (c1, c2)  with cap 1
    ea.add_edge(s, vertex_idx, num_occurence);

    // Increment vertex counter
    vertex_idx++;
  }

  // Compute (s, t)-flow f of g
  long flow = boost::push_relabel_max_flow(g, s, t);

  // If (flow < note.size()) no else yes 
  if (flow < note.size()) {
    std::cout << "No" << std::endl;
  } else {
    std::cout << "Yes" << std::endl;
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) testcase();
  return 0;
}
