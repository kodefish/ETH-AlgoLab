#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

using namespace boost;

typedef adjacency_list<vecS, vecS, directedS> Graph;
typedef adjacency_list<vecS, vecS, directedS, 
                                no_property, 
                                property <edge_weight_t, int>> DiGraph;
typedef property_map<DiGraph, edge_weight_t >::type             WeightMap;
typedef graph_traits<DiGraph>::vertex_descriptor                Vertex;
typedef graph_traits<DiGraph>::edge_descriptor                  Edge;

void testcase() {
    int N, M, A, S, C, D;
    std::cin >> N >> M >> A >> S >> C >> D;

    DiGraph G(N);
    WeightMap weight_map = get(edge_weight, G);

    for (int m = 0; m < M; m++) {
        char w; int x, y, z;
        std::cin >> w >> x >> y >> z;
        Edge e;
        tie(e, tuples::ignore) = add_edge(x, y, G);
        weight_map[e] = z;
        if (w == 'L') {
            tie(e, tuples::ignore) = add_edge(y, x, G);
            weight_map[e] = z;
        }
    }

    // Compute distance maps for each agent
    std::vector<std::vector<int>> distmaps(A, std::vector<int>(N));
    for (int a = 0; a < A; ++a) {
        int i; std::cin >> i; 
        dijkstra_shortest_paths(G, i,
            distance_map(make_iterator_property_map(distmaps[a].begin(),
                            get(vertex_index, G))));
    }

    // Compute pairwise graph G' a - dist to shelter i -> shelter i
    std::vector<std::vector<int>> T(A, std::vector<int>(S));
    for (int s = 0; s < S; s++) {
        int shelter; std::cin >> shelter;
        for (int a = 0; a < A; a++) {
            T[a][s] = distmaps[a][shelter];
        }
    }

    // Binary search for smallest t
    int low = 0, high = INT_MAX;
    while (low < high) {
        int mid = low + (high - low) / 2;
        Graph GG(A + C * S);
        for (int a = 0; a < A; a++) {
            for (int s = 0; s < S; s++) {
                if (T[a][s] == INT_MAX) continue; // Ignore unreachable shelters
                for (int c = 0; c < C; c++) {
                    if (T[a][s] + (c + 1) * D <= mid) // if agent can make it in time
                        add_edge(a, A + c * S + s, GG); 
                }
            }
        }

        // Compute max cardinality matching
        std::vector<Vertex> matemap(A + C * S);
        edmonds_maximum_cardinality_matching(GG, 
            make_iterator_property_map(matemap.begin(), get(vertex_index, GG)));
        const Vertex NULL_VERTEX = graph_traits<DiGraph>::null_vertex();
        int matching_size = 0;
        for (int a = 0; a < A; a++)
            matching_size += (matemap[a] != NULL_VERTEX);
        if (matching_size == A) high = mid; // All agents make it in time
        else low = mid + 1;                 // need more time
    }

    std::cout << low << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t--) testcase();
    return 0;
}
