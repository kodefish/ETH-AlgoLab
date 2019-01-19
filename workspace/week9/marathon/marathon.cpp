// ALGOLAB BGL Tutorial 3
// Code snippets demonstrating 
// - MinCostMaxFlow with arbitrary edge costs using cycle_canceling
// - MinCostMaxFlow with non-negative edge costs using successive_shortest_path_nonnegative_weights

// Compile and run with one of the following:
// g++ -std=c++11 -O2 bgl_mincostmaxflow.cpp -o bgl_mincostmaxflow; ./bgl_mincostmaxflow
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 bgl_mincostmaxflow.cpp -o bgl_mincostmaxflow; ./bgl_mincostmaxflow

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

    void addEdge(int u, int v, long c, long w) {
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

/*  To get 100points the graph needs some precomputation. The vertices are given undirected
 *  so we need to figure out the direction. The pair v, u is ordered u->v if u->v is part
 *  of a shortest path from S to F. Test set 2 can use Dijkstra to compute a shortest path, 
 *  which is guaranteed to be unique, and so add edges (pred[v], v), with capacity c and cost d.
 *  After the precomputation, we run min cost max flow, to try find the number of runners. To find
 *  the correct amount of runners, we use binary search. For each potential number of runners, we 
 *  run min cost max flow, if the distance ran by each runner (cost/flow) is greater than shortest
 *  distance S->F, then reduce the number of runners, otherwise it must be equal, so we try more
 *  runners. This part is working in the following snippet, but the precomputation is not. Have fun!
void testcase() {
    int N, M, S, F;
    std::cin >> N >> M >> S >> F;

    // Create Graph and Maps
    Graph G(N + 2);
    Vertex v_source = N;
    Vertex v_target = v_source + 1;
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);
    
    // Add the edges
    /* Stuff to pass test 1
    int MAX_RUNNERS = 0;
    for (int i = 0; i < M; i++) {
        int a, b, c, d; std::cin >> a >> b >> c >> d;
        eaG.addEdge(a, b, c, d);
        if (a == S || b == S) MAX_RUNNERS += c;
    }

    // Figure out race length
    eaG.addEdge(v_source, S, 1, 0);
    eaG.addEdge(F, v_target, 1, 0);

    // Run the algorithm
    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
    double dist_race = boost::find_flow_cost(G);
    */

    // Trying to pass test 2
    Graph graph(N);
    EdgeWeightMap wm = boost::get(boost::edge_weight, graph);
    std::map<Edge, int> cap;
    for (int i = 0; i < M; i++) {
        int a, b, c, d; std::cin >> a >> b >> c >> d;
        Edge e; bool s; 
        boost::tie(e, s) = boost::add_edge(a, b, graph);
        cap[e] = c;
        wm[e] = d;
    }

    std::vector<int> distmap(N);
    std::vector<Vertex> predmap(N);
    boost::dijkstra_shortest_paths(graph, S,
        boost::predecessor_map(boost::make_iterator_property_map( // named parameters
            predmap.begin(), boost::get(boost::vertex_index, graph))).
                distance_map(boost::make_iterator_property_map(    // concatenated by .
                    distmap.begin(), boost::get(boost::vertex_index, graph))));

    for (int v = 0; v < predmap.size(); v++) {
        int u = predmap[v];
        if (u != v) {
            Edge e; bool s;
            boost::tie(e, s) = boost::edge(u, v, graph);
            eaG.addEdge(u, v, cap[e], wm[e]);
        }
    }

    double dist_race = distmap[F];

    // Binary search the number of runners on the precomputed graph.
    int lower = 1, upper = INT_MAX, num_runners = ((upper - lower) / 2) + lower;
    while (true) {
        // Remove any previous runners
        boost::remove_edge(v_source, S, G);
        boost::remove_edge(F, v_target, G);
        
        // Allow num_runners runners in the race
        eaG.addEdge(v_source, S, num_runners, 0);
        eaG.addEdge(F, v_target, num_runners, 0);

        // Find out how many fit and how much each athlete runs
        boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
        int cost = boost::find_flow_cost(G);
        int flow = 0;
        // Iterate over all edges leaving the source to sum up the flow values.
        OutEdgeIt e, eend;
        for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
            flow += capacitymap[*e] - rescapacitymap[*e];
        }

        double dist_per_runner = (double) cost / (double) flow;

        // Binary search
        // Increment lower bound 
        if (flow == num_runners && dist_per_runner <= dist_race) {
            lower = num_runners;               
        } 
        // Decrement upper bound
        if (dist_per_runner > dist_race || num_runners > flow) {
            upper = num_runners;
        }
        num_runners = ((upper - lower) / 2) + lower;
        if (lower == upper - 1) break;
    }

    std::cout << num_runners << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}

