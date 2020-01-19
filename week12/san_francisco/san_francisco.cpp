#include <iostream>
#include <vector>
#include <algorithm>

typedef std::vector<long> vec_l;
typedef std::vector<vec_l> vec_ll;
typedef std::vector<int> vec_i;
typedef std::vector<vec_i> vec_ii;

const long MEMO_INIT = -1;

long best_score(const int u, const int moves_left, const vec_ii &adj_list, const vec_ll &edge_weights, vec_ll &memo) {
    if (moves_left == 0) return 0; // base case

    if (memo[u][moves_left] != MEMO_INIT) return memo[u][moves_left];

    long best = 0;
    for (int i = 0; i < adj_list[u].size(); i++) {
        int v = adj_list[u][i];

        long points = edge_weights[u][v];
        long rec = best_score(v, moves_left - 1, adj_list, edge_weights, memo);
        best = std::max(best, rec + points);
    }

    if (adj_list[u].empty()) { // If no out edges, restart from beginning
        best = best_score(0, moves_left, adj_list, edge_weights, memo);
    }

    memo[u][moves_left] = best;
    return best;
}

void testcase() {
    long n, m, x, k; std::cin >> n >> m >> x >> k;

    // Read in edges
    vec_ll edge_weights(n, vec_l(n, MEMO_INIT));
    for (int i = 0; i < m; i++) {
        int u, v; long p; std::cin >> u >> v >> p;
        edge_weights[u][v] = std::max(edge_weights[u][v], p); // only keep highest scoring edge
    }

    // Build adjacency list
    vec_ii adj_list(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (edge_weights[i][j] > MEMO_INIT) adj_list[i].push_back(j);
        }
    }

    if (adj_list[0].size() == 0) { // No vertices reacheable from 0 :(
        std::cout << "Impossible" << std::endl;
        return;
    }

    long best = -1;
    vec_ll memo(n + 1, vec_l(k + 1, MEMO_INIT));
    for (int i = k; i >= 0; i--) {
        // Compute best score possible using i moves starting from 0
        long res = best_score(0, i, adj_list, edge_weights, memo);
        if (res < x) break; // score is smaller than chief's -> best is already found or impossible
        else best = i;
    }

    if (best > -1) std::cout << best << std::endl;
    else std::cout << "Impossible" << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t--) testcase();
    return 0;
}
