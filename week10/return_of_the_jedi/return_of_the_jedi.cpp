// Full credit goes to YaweiYe29
//  STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <stack>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                              boost::no_property,
                              boost::property<boost::edge_weight_t, int>> Graph;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_iterator EdgeIt;
typedef boost::graph_traits<Graph>::out_edge_iterator OutEdgeIt;
typedef boost::property_map<Graph, boost::edge_weight_t>::type WeightMap;

void preprocess(int from, Graph &G, int n, std::vector<std::vector<int>> &weights, std::vector<std::vector<int>> &preprocessed) {
    std::vector<bool> visited(n, false);
    std::stack<int> dfs;
    dfs.push(from);
    visited[from] = true;
    preprocessed[from][from] = 0;

    // dfs
    while(!dfs.empty()) {
        int curr = dfs.top();
        dfs.pop();
        OutEdgeIt oebeg, oeend;
        for (boost::tie(oebeg, oeend) = boost::out_edges(curr, G); oebeg != oeend; oebeg++) {
            Vertex u = boost::source(*oebeg, G);
            Vertex v = boost::target(*oebeg, G);
            if (visited[v]) continue;
            int w = weights[u][v];
            preprocessed[from][v] = std::max(w, preprocessed[from][curr]);
            visited[v] = true;
            dfs.push(v);
        }
    }
}

void testcase() {
    int n, root; std::cin >> n >> root;
    Graph G(n);
    WeightMap weightmap = boost::get(boost::edge_weight, G);
    std::vector<std::vector<int>> weights(n, std::vector<int>(n, -1));

    // Create graph from input
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            int c; std::cin >> c;
            Edge e; bool b;
            boost::tie(e, b) = boost::add_edge(i, j, G);
            weightmap[e] = c;
            weights[i][j] = weights[j][i] = c;
        }
    }

    // Prim MST from root
    std::vector<Vertex> prim_pred_map(n);
    boost::prim_minimum_spanning_tree(G,
                                      boost::make_iterator_property_map(prim_pred_map.begin(), boost::get(boost::vertex_index, G)),
                                      boost::root_vertex(root-1));

    Graph G2(n);
    std::vector<std::vector<bool>> added = std::vector<std::vector<bool>>(n, std::vector<bool>(n, false));
    std::vector<std::pair<int, int>> non_mst;

    int weight = 0;
    for (int i = 0; i < n; i++) {
        Vertex u = i;
        OutEdgeIt oebeg, oeend;
        for (boost::tie(oebeg, oeend) = boost::out_edges(u, G); oebeg != oeend; oebeg++) {
            Vertex v = boost::target(*oebeg, G);
            if (prim_pred_map[u] == v) {
                weight += weights[u][v];
                Edge e; bool s;
                boost::tie(e, s) = boost::add_edge(u, v, G2);
            } else if (prim_pred_map[v] != u) {
                if (!added[u][v] && !added[v][u]) {
                    added[u][v] = true;
                    non_mst.push_back(std::make_pair(u, v));
                }
            }
        }
    }

    // Find replacement of min cost
    std::vector<std::vector<int>> preprocessed(n, std::vector<int>(n, -1));

    int min = INT32_MAX;
    for (auto it = non_mst.begin(); it != non_mst.end(); it++) {
        int w = weights[it->first][it->second];
        if (preprocessed[it->first][0] == -1) {
            preprocess(it->first, G2, n, weights, preprocessed);
        }
        int w2 = w - preprocessed[it->first][it->second];
        if (w2 < min) min = w2;
    }

    std::cout << weight + min << std::endl;

}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t--) testcase();
    return 0;
}
