#include <iostream>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace std;

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > undirected_graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it;
typedef boost::graph_traits<undirected_graph>::edge_descriptor  u_edge_desc;

// Custom edge adder class
class edge_adder {
    graph &G;

    public:
    explicit edge_adder(graph &G) : G(G) {}
    void add_edge(int from, int to, long capacity, long cost) {
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);
        auto w_map = boost::get(boost::edge_weight, G); // new!
        const edge_desc e = boost::add_edge(from, to, G).first;
        const edge_desc rev_e = boost::add_edge(to, from, G).first;
        c_map[e] = capacity;
        c_map[rev_e] = 0; // reverse edge has no capacity!
        r_map[e] = rev_e;
        r_map[rev_e] = e;
        w_map[e] = cost;   // new assign cost
        w_map[rev_e] = -cost;   // new negative cost
    }
};

struct street {
    int a, b, c, d;
};


void testcase() {
    int n, m, s, f; cin >> n >> m >> s >> f;

    undirected_graph G_input(n);
    auto w_map = boost::get(boost::edge_weight, G_input);

    vector<street> streets;
    for (int i = 0; i < m; i++) {
        int a, b, c, d; cin >> a >> b >> c >> d;
        const u_edge_desc e = boost::add_edge(a, b, G_input).first;
        w_map[e] = d;
        streets.push_back(street{a, b, c, d});
    }

    std::vector<int> dist_map(n);
    boost::dijkstra_shortest_paths(G_input, s,
                                   boost::distance_map(boost::make_iterator_property_map(
                                                           dist_map.begin(), boost::get(boost::vertex_index, G_input))));

    const int shortest_length = dist_map[f];

    graph G_max(n);
    edge_adder adder(G_max);
    auto c_map = boost::get(boost::edge_capacity, G_max);
    auto r_map = boost::get(boost::edge_reverse, G_max);
    auto rc_map = boost::get(boost::edge_residual_capacity, G_max);

    for (auto street : streets) {
        if (dist_map[street.a] < dist_map[street.b]) adder.add_edge(street.a, street.b, street.c, street.d);
        else adder.add_edge(street.b, street.a, street.c, street.d);
    }

    boost::successive_shortest_path_nonnegative_weights(G_max, s, f);
    int max_flow = 0;
    out_edge_it e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(s, G_max), G_max); e != eend; ++e) {
        max_flow += c_map[*e] - rc_map[*e];
    }

    int lower = 0, upper = max_flow;
    while (lower <= upper) {
        int m = (lower + upper) / 2;

        graph G(n);
        edge_adder adder(G);
        auto c_map = boost::get(boost::edge_capacity, G);
        auto rc_map = boost::get(boost::edge_residual_capacity, G);
        const int v_source = boost::add_vertex(G);
        const int v_target = boost::add_vertex(G);

        for (auto street : streets) {
            if (dist_map[street.a] < dist_map[street.b]) adder.add_edge(street.a, street.b, street.c, street.d);
            else adder.add_edge(street.b, street.a, street.c, street.d);
        }

        adder.add_edge(v_source, s, m, 0);
        adder.add_edge(f, v_target, m, 0);

        boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
        double cost = (double) boost::find_flow_cost(G);
        double flow = 0;
        out_edge_it e, eend;
        for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
            flow += c_map[*e] - rc_map[*e];
        }

        double avg_length = cost / flow;
        if (avg_length > shortest_length) {
            upper = m - 1;
        } else {
            lower = m + 1;
        }
    }

    std::cout << lower - 1 << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) testcase();
    return 0;
}
