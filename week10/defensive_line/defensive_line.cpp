#include <iostream>
#include <vector>
#include <algorithm>

const int MEMO_INIT = -2;

int dp(int start, int attackers, const std::vector<int> & sw, std::vector<std::vector<int>> & memo) {
    if (start == sw.size()) return -1;

    if (memo[attackers][start] != MEMO_INIT) return memo[attackers][start];

    if (attackers == 1) {
        if (start == sw.size() - 1) return sw[start] == -1 ? -1 : 1;

        int val = sw[start] - start + 1;
        int rec = dp(start + 1, attackers, sw, memo);
        memo[attackers][start] = std::max(val, rec);
        return memo[attackers][start];
    }

    // Reccurence
    int best_strategy = -1;
    for (int i = start; i < sw.size(); i++) {
        if (sw[i] == -1) continue; // No window start here

        int val = sw[i] - i + 1;
        int rec = dp(sw[i] + 1, attackers - 1, sw, memo);
        if (rec > -1) best_strategy = std::max(best_strategy, val + rec);
    }

    memo[attackers][start] = best_strategy;
    return memo[attackers][start];
}

void testcase() {
    int n, m, k; std::cin >> n >> m >> k;

    std::vector<int> defense_values(n);
    for (int i = 0; i < n; i++) std::cin >> defense_values[i];

    // Precomputation step -> sliding window
    std::vector<int> sw(n, -1);
    int intervals = 0;
    int i = 0, j = 0, sum = defense_values[i];
    while (j < n) {
        // If interval is attackable, save it
        if (sum == k) {
            sw[i] = j;
            intervals++;
        }

        // Increment sliding window
        if (sum <= k) {
            sum += defense_values[++j]; // increment and add
        } else {
            sum -= defense_values[i++]; // remove and decrement
        }
    }

    if (intervals < m) {
        std::cout << "fail" << std::endl;
        return;
    }

    std::vector<std::vector<int>> memo(m+1, std::vector<int>(n, MEMO_INIT));
    int best_strategy = dp(0, m, sw, memo);
    if (best_strategy < 0) std::cout << "fail" << std::endl;
    else std::cout << best_strategy << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}



