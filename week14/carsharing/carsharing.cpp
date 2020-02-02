// STL includes
#include <iostream>
#include <vector>
#include <map>
#include <set>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

using namespace std;

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

struct booking {
    int s, t, d, a, p;
};

// Custom edge adder class
class edge_adder {
    graph &G;

    public:
        explicit edge_adder(graph &G) : G(G) {}
        void add_edge(int from, int to, long capacity, long cost) {
            //std::cerr << from << ", " << to << ", " << capacity << ", " << cost << std::endl;
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

void testcase() {
    int N, S; cin >> N >> S;
    vector<int> L(S);
    for (int i = 0; i < S; i++) {
        cin >> L[i];
    }

    const int MAX_CARS = 100;
    const int MAX_TIME = 100000;
    const int MAX_PROFIT = 100;
    const int INF = S * MAX_CARS;

    vector<booking> bookings;
    vector<set<int>> times(S);
    for (int s = 0; s < S; s++) {
        times[s].insert(0);
        times[s].insert(MAX_TIME);
    }

    vector<map<int, int>> mapping(S);

    for (int i = 0; i < N; i++) {
        int s, t, d, a, p;
        cin >> s >> t >> d >> a >> p;
        s--; t--; // zero-based stations
        times[s].insert(d);
        times[t].insert(a);
        bookings.push_back(booking{s, t, d, a, p});
    }

    vector<int> partial_sums(S+1);
    for (int s = 0; s < times.size(); s++) {
        int i = 0;
        for (auto t : times[s]) {
            mapping[s][t] = i++;
        }

        partial_sums[s + 1] = partial_sums[s] + mapping[s].size();
    }


    int T = partial_sums.back();
    int v_source = T;
    int v_target = v_source + 1;
    graph G(T + 2);
    edge_adder adder(G);  
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);

    for (int i = 0; i < S; i++) {
        adder.add_edge(v_source, partial_sums[i], L[i], 0);
        adder.add_edge(partial_sums[i + 1] - 1, v_target, INF, 0);

        int it = -1, last_time = 0;
        for (auto t : times[i]) {
            if (it != -1) adder.add_edge(partial_sums[i] + it, partial_sums[i] + it + 1, INF, MAX_PROFIT * (t - last_time));
            it++; last_time = t;
        }
    }

    for (int i = 0; i < N; i++) {
        int from = partial_sums[bookings[i].s] + mapping[bookings[i].s][bookings[i].d];
        int to = partial_sums[bookings[i].t] + mapping[bookings[i].t][bookings[i].a];
        adder.add_edge(from, to, 1, (bookings[i].a - bookings[i].d) * MAX_PROFIT - bookings[i].p);
    }

    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
    int cost = boost::find_flow_cost(G);
    int flow = 0;
    out_edge_it e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
        flow += c_map[*e] - rc_map[*e];
    }

    std::cout << MAX_PROFIT * MAX_TIME * flow - cost << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}
