// STL include
#include <iostream>
#include <vector>
#include <algorithm>

// BGL include
#include <boost/graph/adjacency_list.hpp>
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
    int n, m; long k, l; std::cin >> n >> m >> k >> l;

    // How many police stations per intersection
    std::vector<long> police_stations(k);
    for (int i = 0; i < k; i++) std::cin >> police_stations[i];

    std::vector<long> photographs(l);
    for (int i = 0; i < l; i++) std::cin >> photographs[i];

    // Graph with edges at capacity k, to be used to get from police stations to photos
    graph G((2* n) + 2);
    edge_adder adder(G);
    const int v_source = 2 * n, v_target = v_source + 1;

    for (int i = 0; i < m; i++) {
        int x, y; std::cin >> x >> y;
        adder.add_edge(x, y, k);
        adder.add_edge(x + n, y + n, 1);
    }

    for (int i = 0; i < l; i++) {
        adder.add_edge(photographs[i], photographs[i] + n, 1);
    }

    for (int i = 0; i < k; i++) {
        adder.add_edge(v_source, police_stations[i], 1);
        adder.add_edge(police_stations[i] + n, v_target, 1);
    }

    long flow =  boost::push_relabel_max_flow(G, v_source, v_target);
    std::cout << flow << std::endl;

}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}
