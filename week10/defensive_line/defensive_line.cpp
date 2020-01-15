#include <iostream>
#include <vector>

const double MEMO_INIT = -2;

double dp(int start, int attackers, const int num_defenders, const std::vector<int> & sw, std::vector<std::vector<double>> & memo) {
    if (attackers == 0) return 0; // No attackers left, legal strategy found
    if (start == num_defenders) return -1; // Attackers left, no more defenders -> illegal strategy

    if (memo[attackers][start] != MEMO_INIT) return memo[attackers][start];

    // If only one attacker left
    double best_strategy = -1;
    if (attackers == 1) {
        best_strategy = std::max((double) sw[start], dp(start + 1, attackers, num_defenders, sw, memo));
    } else {
        for (int i = start; i < num_defenders; i++) {
            if (sw[i] > 0) { // Only consider possible attack interval starts
                double rec = dp(i + sw[i], attackers - 1, num_defenders, sw, memo);
                if (rec < 0) continue; // Only consider legal strategies
                double strategy = sw[i] + rec;
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

    std::vector<std::vector<double>> memo(m+1, std::vector<double>(n, MEMO_INIT));
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


