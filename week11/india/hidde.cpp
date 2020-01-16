//
// Created by Hidde Lycklama on 11/26/18.
//

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

    std::pair<Edge, Edge> addEdge(int u, int v, long c, long w) {
        Edge e, rev_e;
        boost::tie(e, boost::tuples::ignore) = boost::add_edge(u, v, G);
        boost::tie(rev_e, boost::tuples::ignore) = boost::add_edge(v, u, G);
        revedgemap[e] = rev_e;
        revedgemap[rev_e] = e;
        auto pair = std::pair<Edge, Edge>(e, rev_e);
        mutateEdge(pair, c, w);
        return pair;
    }

    void mutateEdge(std::pair<Edge, Edge> pair, long c, long w) {
        Edge e = pair.first;
        Edge rev_e = pair.second;
        capacitymap[e] = c;
        weightmap[e] = w; // new!
        capacitymap[rev_e] = 0;
        weightmap[rev_e] = -w; // new
    }
};

void testcase() {
    int numCities, numGuides, budget, s, t;
    std::cin >> numCities >> numGuides >> budget >> s >> t;

    Graph G(numCities + 2);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);

    for (int i = 0; i < numGuides; i++) {
        int u, v, cost, capacity;
        std::cin >> u >> v >> cost >> capacity;

        eaG.addEdge(u, v, capacity, cost);
    }

    Vertex start = numCities;
    Vertex finish = numCities + 1;

    auto es = eaG.addEdge(start, s, 1, 0);
    auto ef = eaG.addEdge(t, finish, 1, 0);

    // we do a binary search ... ?
    int upperBound = 1;
    bool foundUpperBound = false;
    while (true) {
        eaG.mutateEdge(es, upperBound, 0);
        eaG.mutateEdge(ef, upperBound, 0);

        boost::successive_shortest_path_nonnegative_weights(G, start, finish);

        int resFlow = rescapacitymap[es.first];
        if (resFlow > 0) {
            // we are not increasing anymore...

            // this can probably be more efficient by looking at the residual capacity !!!
            break;
        }

        int cost = boost::find_flow_cost(G);
        if (cost > budget) {
            break;
        }
        upperBound *= 2;
    }

    std::cerr << upperBound << std::endl;

    int i = 0;
    while (i != upperBound) {
        int mid = (upperBound + i) / 2;

        eaG.mutateEdge(es, mid, 0);
        eaG.mutateEdge(ef, mid, 0);

        boost::successive_shortest_path_nonnegative_weights(G, start, finish);

        int cost = boost::find_flow_cost(G);
        if (rescapacitymap[es.first] > 0 || cost > budget) {
            upperBound = mid;
        } else {
            i = mid + 1;
        }
    }

    std::cout << i - 1 << std::endl;

//    std::cout << "I " << i << std::endl;

//    if (i == 0) {
//        std::cout << 0 << std::endl;
//    } else {
//
//        // Calculate the flow again for previous val
//        eaG.mutateEdge(es, i, 0);
//        eaG.mutateEdge(ef, i, 0);
//
//        boost::successive_shortest_path_nonnegative_weights(G, s, t);
//
//        int s_flow = 0;
//        // Iterate over all edges leaving the source to sum up the flow values.
//        OutEdgeIt e, eend;
//        for(boost::tie(e, eend) = boost::out_edges(boost::vertex(s,G), G); e != eend; ++e) {
////        std::cout << "edge from " << boost::source(*e, G) << " to " << boost::target(*e, G)
////                  << " with capacity " << capacitymap[*e] << " and residual capacity " << rescapacitymap[*e] << std::endl;
//            s_flow += capacitymap[*e] - rescapacitymap[*e];
//        }
//
//
//        std::cout << s_flow << std::endl;
//    }


//    std::cout << cost << std::endl;
}

int main() {
    int t;std::cin >> t;
    while(t--) {
        testcase();
    }
    return 0;
}
