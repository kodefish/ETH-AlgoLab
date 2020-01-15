#include <iostream>
#include <vector>

const int MEMO_INIT = -2;

int dp(int start, int attackers, const int num_defenders, const std::vector<int> & sw, std::vector<std::vector<int>> & memo) {
    if (start == num_defenders) return -1; // Attackers left, no more defenders -> illegal strategy

    //if (memo[attackers][start] != MEMO_INIT) return memo[attackers][start];

    // If only one attacker left
    int best_strategy = -1;
    if (attackers == 1) {
        if (start == num_defenders - 1) return sw[start] > 0 ? sw[start] : -1;

        int strategy = sw[start] > 0 ? sw[start] : -1;
        int next = dp(start + 1, attackers, num_defenders, sw, memo);
        best_strategy = std::max(strategy, next);
    } else {
        for (int i = start; i < num_defenders; i++) {
            if (sw[i] > 0) { // Only consider possible attack interval starts
                int rec = dp(i + sw[i], attackers - 1, num_defenders, sw, memo);
                if (rec < 0) continue; // Only consider legal strategies
                int strategy = sw[i] + rec;
                if (strategy > best_strategy) best_strategy = strategy;
            }
        }
    }

    memo[attackers][start] = best_strategy;

    return best_strategy;
}

void testcase() {
    int n, m, k; std::cin >> n >> m >> k;

    std::vector<int> defense_values(n);
    for (int i = 0; i < n; i++) std::cin >> defense_values[i];

    // Precomputation step -> sliding window
    std::vector<int> sw(n, 0);
    int i = 0, j = 0, sum = defense_values[i];
    while (j < n || sum >= k) {
        // If interval is attackable, save it
        if (sum == k) {
            sw[i] = j - i + 1;
        }

        // Increment sliding window
        if (sum < k && j < n) {
            sum += defense_values[++j]; // increment and add
        } else {
            sum -= defense_values[i++]; // remove and decrement
        }
    }

    std::vector<std::vector<int>> memo(m+1, std::vector<int>(n, MEMO_INIT));
    int best_strategy = dp(0, m, n, sw, memo);
    if (best_strategy < 0) std::cout << "fail" << std::endl;
    else std::cout << best_strategy << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}


