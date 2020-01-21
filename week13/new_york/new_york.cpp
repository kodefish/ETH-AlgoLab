#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>

void testcase() {
    int n, m, k; std::cin >> n >> m >> k;
    std::vector<int> temp(n);
    for (int i = 0; i < n; i++) std::cin >> temp[i];

    std::vector<int> next(n, n);
    for (int i = 0; i < n - 1; i++) {
        int u, v; std::cin >> u >> v;
        next[u] = v;
    }

    // Check for feasibility starting at i that have outgoing routes
    std::vector<int> starting_points;
    int u = 0, v = u;
    int min_i = u, max_i = u;
    int j;
    for (j = 0; j < m - 1 && next[v] < n; j++) {
        v = next[v];
        if (temp[v] < temp[min_i]) min_i = v;
        if (temp[v] > temp[min_i]) max_i = v;
    }
    if (j == m - 1) {
        while (v < n) {
            int risk = temp[max_i] - temp[min_i];
            if (risk <= k) starting_points.push_back(u); // Feasible trip of length m starting at i

            v = next[v];
            if (temp[v] < temp[min_i]) min_i = v;
            if (temp[v] > temp[min_i]) max_i = v;


            u = next[u];
            if (min_i == u || max_i == u) {
                for (int i = u; i != next[v]; i = next[i]) {
                    if (temp[i] < temp[min_i]) min_i = v;
                    if (temp[i] > temp[min_i]) max_i = v;
                }
            }
        }
    }

    if (starting_points.empty()) std::cout << "Abort mission" << std::endl;
    else {
        std::sort(starting_points.begin(), starting_points.end());
        for (auto i : starting_points) std::cout << i << " ";
        std::cout << std::endl;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}
