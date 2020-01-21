// Credit goes to hlycklama
// https://github.com/hlycklama/eth-algolab/blob/master/week12/potw/main.cpp
#include <iostream>
#include <queue>
#include <set>
#include <algorithm>
#include <stack>

void dfs(int v, std::multiset<int> &window, int s, int m, int k, std::deque<int> &hist,
         const std::vector<std::deque<int>> &adj_list, const std::vector<int> &temp, std::set<int> &starting_points) {
    hist.push_back(v);
    int val = 0;

    // Keep track of temps in the path, via a sorted deque
    if (s >= m) {
        window.erase(window.find(temp[*hist.begin()]));
        val = hist.front();
        hist.pop_front();
    }
    window.insert(temp[v]);

    // If start of fesible journey, add as starting point
    if (window.size() == m && *window.rbegin() - *window.begin() <= k) {
        starting_points.insert(hist.front());
    }

    // go over reachable points in classic dfs manner
    for (auto i : adj_list[v]) {
        dfs(i, window, s + 1, m, k, hist, adj_list, temp, starting_points);
    }

    if (s >= m) {
        hist.push_front(val);
        window.insert(temp[val]);
    }

    window.erase(window.find(temp[v]));
    hist.pop_back();
}

void testcase() {
    int n, m, k; std::cin >> n >> m >> k;

    std::vector<int> temp(n);
    for (int i = 0; i < n; i++) std::cin >> temp[i];

    std::vector<std::deque<int>> adj_list(n);
    for (int i = 0; i < n - 1; i++) {
        int u, v; std::cin >> u >> v;
        adj_list[u].push_back(v);
    }

    std::set<int> starting_points;
    std::multiset<int> window;
    std::deque<int> history;
    dfs(0, window, 0, m, k, history, adj_list, temp, starting_points);

    if (starting_points.empty()) std::cout << "Abort mission" << std::endl;
    else {
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
