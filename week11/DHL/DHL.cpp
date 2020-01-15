#include <iostream>
#include <vector>
#include <climits>

void testcase() {
    int n; std::cin >> n;

    std::vector<int> stack_a(n), stack_b(n);
    for (int i = 0; i < n; i++) std::cin >> stack_a[i];
    for (int i = 0; i < n; i++) std::cin >> stack_b[i];

    // Precompute the sums
    // Precomputing the sums
    std::vector<int>partial_sums_a(n + 1, 0);
    std::vector<int>partial_sums_b(n + 1, 0);
    for (int i = n - 1 ; i >= 0 ; i--) {
        partial_sums_a[i] = partial_sums_a[i + 1] + stack_a[i];
        partial_sums_b[i] = partial_sums_b[i + 1] + stack_b[i];
    }

    std::vector<std::vector<int>> bu (n+1, std::vector<int>(n+1, -1));
    // Base case, both piles are emtpy
    bu[0][0] = 0;
    for (int i = 1; i < n + 1; i++) {
        for (int j = 1; j < n + 1; j++) {
            int min_cost = INT_MAX;

            // Take one package from a, and try taking all possible packages from b
            int cost_a = partial_sums_a[i - 1] - partial_sums_a[i] - 1;

            for (int k = 1; k <= j; k++) {
                int previous_best = bu[i - 1][j - k];
                if (previous_best > -1) {
                    int cost_b = partial_sums_b[j - k] - partial_sums_b[j] - k;
                    int cost = (cost_a * cost_b) + previous_best;
                    if (cost < min_cost) min_cost = cost;
                }
            }

            // Do the same, but for b
            int cost_b = partial_sums_b[j - 1] - partial_sums_b[j] - 1;

            for (int k = 1; k <= i; k++) {
                int previous_best = bu[i - k][j - 1];
                if (previous_best > -1) {
                    int cost_a = partial_sums_a[i - k] - partial_sums_a[i] - k;
                    int cost = (cost_a * cost_b) + previous_best;
                    if (cost < min_cost) min_cost = cost;
                }
            }

            bu[i][j] = min_cost;
        }
    }

    std::cout << bu[n][n] << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}
