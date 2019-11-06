#include <iostream>
#include <vector>

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

        void add_edge(int from, int to, long capacity) {
            auto c_map = boost::get(boost::edge_capacity, G);
            auto r_map = boost::get(boost::edge_reverse, G);
            const auto e = boost::add_edge(from, to, G).first;
            const auto rev_e = boost::add_edge(to, from, G).first;
            c_map[e] = capacity;
            c_map[rev_e] = 0; // reverse edge has no capacity!
            r_map[e] = rev_e;
            r_map[rev_e] = e;
        }
};

void testcase() {
    int n, m; std::cin >> n >> m;

    // Graph has a vertex for each round and each player, plus a source and a sink
    graph G(n + m + 2);
    const int v_source = m + n;
    const int v_target = v_source + 1;
    edge_adder adder(G);

    // Read in rounds
    for (int i = 0; i < m; i++) {
        int a, b, c; std::cin >> a >> b >> c;
        // Add edge from round to target (each round must have a winner)
        adder.add_edge(n + i, v_target, 1);

        // Add edge from player a to round i if he won or if score is unknown
        if (c == 1 || c == 0) {
            adder.add_edge(a, n + i, 1);
        }

        // Add edge from player b to round i if he won or if score is unknown
        if (c == 2 || c == 0) {
            adder.add_edge(b, n + i, 1);
        }
    }

    // Read in scores
    int required_flow = 0;
    for (int i = 0; i < n; i++) {
        int s; std::cin >> s;
        required_flow += s;
        adder.add_edge(v_source, i, s);
    }

    // Compute flow
    long flow = boost::push_relabel_max_flow(G, v_source, v_target);
    if (flow == std::max(m, required_flow)) std::cout << "yes" << std::endl;
    else std::cout << "no" << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}
