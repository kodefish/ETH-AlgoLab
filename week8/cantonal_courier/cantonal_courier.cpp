// Algolab BGL Tutorial 2 (Max flow, by taubnert@ethz.ch)
// Flow example demonstrating how to use push_relabel_max_flow using a custom edge adder
// to manage the interior graph properties required for flow algorithms
#include <iostream>

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
    // Idea is to compute the optimal set of zones
    int z, j; std::cin >> z >> j;
    graph G(z + j);
    edge_adder adder(G);
    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_target = boost::add_vertex(G);

    for (int i = 0; i < z; i++) {
        int c; std::cin >> c;
        adder.add_edge(v_source, i, c);
    }

    int profit = 0;
    for (int i = 0; i < j; i++) {
        int p; std::cin >> p;
        adder.add_edge(z + i, v_target, p);
        profit += p;
    }

    const int MAX_CAPACITY = 5000;
    for (int i = 0; i < j; i++) {
        int num_z; std::cin >> num_z;
        for (int k = 0; k < num_z; k++) {
            int zone; std::cin >> zone;
            adder.add_edge(zone, z + i, MAX_CAPACITY);
        }
    }

    // here the flow is equal to the sum of min(cost_of_zone, sum_of_jobs_in_zone)
    // So when we compute the flow, if the zone is profitable, we get cost of zone
    // otherwise we get the sum_of_jobs
    // When we subtract this amount from the total, if the zone is profitable (i.e. the sum of jobs
    // is greater than the cost of the zone) we get the profit. Otherwise the costs cancel out and
    // we get a 0 benefit
    long flow = boost::push_relabel_max_flow(G, v_source, v_target);
    std::cout << profit - flow << std::endl;

}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}

