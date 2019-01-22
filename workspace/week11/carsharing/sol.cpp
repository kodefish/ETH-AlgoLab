#include <iostream>
#include <set>
#include <map>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>
using namespace boost;
using namespace std;

// Graph Type with nested interior edge properties for Cost Flow Algorithms.
typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
 typedef adjacency_list<vecS, vecS, directedS, no_property,
     property<edge_capacity_t, long,
         property<edge_residual_capacity_t, long,
             property<edge_reverse_t, Traits::edge_descriptor,
                 property <edge_weight_t, long> > > > > Graph;

// Interior Property Maps.
typedef property_map<Graph, edge_capacity_t>::type      EdgeCapacityMap;
typedef property_map<Graph, edge_weight_t >::type       EdgeWeightMap;
typedef property_map<Graph, edge_residual_capacity_t>::type ResidualCapacityMap;
typedef property_map<Graph, edge_reverse_t>::type       ReverseEdgeMap;
typedef graph_traits<Graph>::vertex_descriptor          Vertex;
typedef graph_traits<Graph>::edge_descriptor            Edge;
typedef graph_traits<Graph>::out_edge_iterator          OutEdgeIt;

class EdgeAdder {
    Graph &G;
    EdgeCapacityMap &capacitymap;
    EdgeWeightMap &weightmap;
    ReverseEdgeMap  &revedgemap;
public:
    EdgeAdder(Graph & G, EdgeCapacityMap &capacitymap, EdgeWeightMap &weightmap,
        ReverseEdgeMap &revedgemap) : G(G), capacitymap(capacitymap),
        weightmap(weightmap), revedgemap(revedgemap) {}
    void addEdge(int u, int v, long c, long w) {
        Edge e, reverseE;
        tie(e, tuples::ignore) = add_edge(u, v, G);
        tie(reverseE, tuples::ignore) = add_edge(v, u, G);
        capacitymap[e] = c;
        weightmap[e] = w;
        capacitymap[reverseE] = 0;
        weightmap[reverseE] = -w;
        revedgemap[e] = reverseE;
        revedgemap[reverseE] = e;
    }
};

struct Booking {
    int s, t, d, a, p;
};

void testcase() {
    int N, S; cin >> N >> S;
    vector<int> L(S);
    for (int i = 0; i < S; ++i) {
        cin >> L[i];
    }

    const int MAXL = 100;
    const int MAXT = 100000;
    const int MAXP = 100;
    const int INF = MAXL*S;

    // Infinite capacity.
    vector<Booking> B;
    vector<set<int> > times(S);

    // Sets for the coordinate compression per station.
    for (int s = 0; s < S; ++s) {
        // Add sentinels.
        times[s].insert(0);
        times[s].insert(MAXT);
    }

    vector<map<int,int> > M(S);
    // Mappings for the coordinate compression.
    for (int i = 0; i < N; ++i) {
        int s, t, d, a, p;
        cin >> s >> t >> d >> a >> p;
        s--; t--;
        // Convert to zero-based times.
        times[s].insert(d); times[t].insert(a);
        B.push_back({s,t,d,a,p});
    }

    vector<int> psum(S+1);
    // Prefix sums of coordinate lengths.
    for (int s = 0; s < S; ++s) {
        int i = 0;
        // Create the coordinate mapping for station s.
        for (auto &t : times[s]) {
            M[s][t] = i;
            i++;
        }
        psum[s+1] = psum[s] + M[s].size();
    }

    // Build the graph, setup just like in any other MCMF task.
    int T = psum.back();
    // # Vertices = total number of timesteps + 2.
    int v_source = T;
    int v_target = T+1;
    Graph G(T+2);
    EdgeCapacityMap capacitymap = get(edge_capacity, G);
    EdgeWeightMap weightmap = get(edge_weight, G);
    ReverseEdgeMap revedgemap = get(edge_reverse, G);
    ResidualCapacityMap rescapacitymap = get(edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);

    for (int i = 0; i < S; ++i) {
        // Add the edges for the timelines.
        // Source to first station node: L[i] cars, no cost.
        eaG.addEdge(v_source, psum[i], L[i], 0);

        // Last station node to target: arbitrarily many cars.
        eaG.addEdge(psum[i+1]-1, v_target, INF, 0);

        // Add compressed parking edges along each station’s timeline.
        int it = -1, lastt = 0;
        for (auto& t : times[i]) {
            if(it != -1) {
                eaG.addEdge(psum[i]+it, psum[i]+it+1, INF, MAXP*(t-lastt));
            }
            it++; lastt = t;
        }
    }

    for(int i=0; i<N; i++) {
        // Add the booking edges.
        eaG.addEdge(psum[B[i].s] + M[B[i].s][B[i].d],
                    psum[B[i].t] + M[B[i].t][B[i].a],
                    1, ((B[i].a-B[i].d)*MAXP)-B[i].p);
     }

    // Run the min cost max flow algorithm.
    successive_shortest_path_nonnegative_weights(G, v_source, v_target);
    int flow = 0;
    // Iterate over all edges leaving the source to sum up the flow.
    OutEdgeIt e, eend;
    for(tie(e, eend) = out_edges(vertex(v_source,G), G); e != eend; ++e) {
        flow += capacitymap[*e] - rescapacitymap[*e];
    }

    // Undo the offset of the flow costs.
    int cost = MAXP*MAXT*flow-find_flow_cost(G);
    cout << cost << endl;
}

int main() {
     int T; cin >> T;
     for(int t = 0; t < T; ++t) {
         testcase();
     }
}
