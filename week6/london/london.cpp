#include <iostream>
#include <string>
#include <map>

// BGL include
#include <boost/graph/adjacency_list.hpp>

// BGL flow include *NEW*
#include <boost/graph/push_relabel_max_flow.hpp>

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

// Custom edge adder class, highly recommended
class edge_adder {
  graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  edge_desc add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    return e;
  }
};

void make_it_flow() {
}

void testcase() {
    int h, w; std::cin >> h >> w;
    std::string note; std::cin >> note;

    // Count the number of each char in the note
    std::map<char, int> count_chars;
    for (int i = 0; i < note.length(); i++) {
      count_chars[note[i]]++;
    }

    //  Create graph with each double sided letter as a vertex + each character in the note + source and sink
    graph G;
    edge_adder adder(G);

    // Add dummy vertex to reroute all the unneeded letters to it
    // (it's index is 0, and we use maps that by default init to 0)
    const vertex_desc dummy = boost::add_vertex(G);

    //    Add special vertices source and sink
    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_target = boost::add_vertex(G);

    // Map every char to an index, and add edge from it to the target with the required number of letters
    std::map<char, int> letter_vertex_index;
    for (auto pairs = count_chars.begin(); pairs != count_chars.end(); pairs++) {
      const vertex_desc vertex_idx = boost::add_vertex(G);
      letter_vertex_index[pairs->first] = vertex_idx;
      adder.add_edge(vertex_idx, v_target, pairs->second);
    }

    // Count number of similar pieces of paper
    std::map<std::pair<char, char>, int> count_papers;

    // Read in front side of each page
    std::vector<char> front_side(h*w);
    for (int i = 0; i < h; i++) {
      for (int j = 0; j < w; j++) {
        char a; std::cin >> a;
        int piece_of_paper_num = w*i + j;
        front_side[piece_of_paper_num] = a;
      }
    }

    // Read in back side of each page
    for (int i = 0; i < h; i++) {
      for (int j = 0; j < w; j++) {
        int piece_of_paper_num = w*i + w - 1 - j;
        char a = front_side[piece_of_paper_num];
        char b; std::cin >> b;
        count_papers[std::make_pair(a, b)]++;
      }
    }

    // Add vertices for each piece of paper
    for (auto piece_of_paper = count_papers.begin(); piece_of_paper != count_papers.end(); piece_of_paper++) {
      const vertex_desc vertex_idx = boost::add_vertex(G);
      // Add edge from piece of paper to each char and from source, all of capacity = #of identical pieces of paper
      adder.add_edge(vertex_idx, letter_vertex_index[piece_of_paper->first.first], piece_of_paper->second);
      adder.add_edge(vertex_idx, letter_vertex_index[piece_of_paper->first.second], piece_of_paper->second);
      adder.add_edge(v_source, vertex_idx, piece_of_paper->second);
    }

    // Compute flow
    long flow = boost::push_relabel_max_flow(G, v_source, v_target);

    // Check to see that the note can be spelled in it's entirety
    if (flow < note.length()) std::cout << "No" << std::endl;
    else  std::cout << "Yes" << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}
