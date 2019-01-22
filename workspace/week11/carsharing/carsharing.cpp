// Includes
// ========
// STL includes
#include <iostream>
#include <cstdlib>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// BGL Graph definitions
// ===================== 
// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, Traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > Graph; // new!
// Interior Property Maps
typedef boost::property_map<Graph, boost::edge_capacity_t>::type      EdgeCapacityMap;
typedef boost::property_map<Graph, boost::edge_weight_t >::type       EdgeWeightMap; // new!
typedef boost::property_map<Graph, boost::edge_residual_capacity_t>::type ResidualCapacityMap;
typedef boost::property_map<Graph, boost::edge_reverse_t>::type       ReverseEdgeMap;
typedef boost::graph_traits<Graph>::vertex_descriptor          Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor            Edge;
typedef boost::graph_traits<Graph>::out_edge_iterator  OutEdgeIt; // Iterator

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map, weight map and reverse edge map
// ===============================================================
class EdgeAdder {
    Graph &G;
    EdgeCapacityMap &capacitymap;
    EdgeWeightMap &weightmap;
    ReverseEdgeMap  &revedgemap;

public:
    EdgeAdder(Graph &G, EdgeCapacityMap &capacitymap, EdgeWeightMap &weightmap, ReverseEdgeMap &revedgemap) 
        : G(G), capacitymap(capacitymap), weightmap(weightmap), revedgemap(revedgemap) {}

    void addEdge(Vertex u, Vertex v, long c, long w) {
        Edge e, rev_e;
        boost::tie(e, boost::tuples::ignore) = boost::add_edge(u, v, G);
        boost::tie(rev_e, boost::tuples::ignore) = boost::add_edge(v, u, G);
        capacitymap[e] = c;
        weightmap[e] = w; // new!
        capacitymap[rev_e] = 0;
        weightmap[rev_e] = -w; // new
        revedgemap[e] = rev_e; 
        revedgemap[rev_e] = e; 
    }
};

void testcase() {
    int N, S; std::cin >> N >> S;

    // Create Graph and Maps
    Graph G;
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);
    
    // Add the edges
    Vertex v_source = boost::add_vertex(G);
    Vertex v_target = boost::add_vertex(G);

    std::vector<Vertex> stations(S);
    for (int i = 0; i < S; i++) {
        int num_cars; std::cin >> num_cars;
        Vertex v = boost::add_vertex(G);
        eaG.addEdge(v_source, v, num_cars, 0);
        stations[i] = v;
    }

    std::vector<std::map<int, Vertex>> vert_time_map(S);
    for (int i = 0; i < N; i++) {
        int s_i, t_i, d_i, a_i, p_i; 
        std::cin >> s_i >> t_i >> d_i >> a_i >> p_i; 
        // Create vertices s_i.d_i t_i.a_i if not already done
        if (vert_time_map[s_i - 1][d_i] == 0) vert_time_map[s_i - 1][d_i] = boost::add_vertex(G);
        if (vert_time_map[t_i - 1][a_i] == 0) vert_time_map[t_i - 1][a_i] = boost::add_vertex(G);

        Vertex dep = vert_time_map[s_i - 1][d_i];
        Vertex arr = vert_time_map[t_i - 1][a_i];

        // Add edge s_i - s_i.d_i
        eaG.addEdge(stations[s_i - 1], dep, 1, 0);

        // Add edge s_i.d_i - t_i.a_i
        eaG.addEdge(dep, arr, 1, -p_i);

        // Add edge t_i.a_i - v_target
        eaG.addEdge(arr, v_target, 1, 0);
    }

    boost::graph_traits<Graph>::edge_iterator ei, ei_end;

    // Option 1: Min Cost Max Flow with cycle_canceling
    int flow = boost::push_relabel_max_flow(G, v_source, v_target);
    boost::cycle_canceling(G);
    int cost = boost::find_flow_cost(G);
    std::cout << -cost << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}
