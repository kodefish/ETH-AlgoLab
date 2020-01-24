#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

void testcase() {
    int n, k; std::cin >> n >> k;
    std::vector<std::pair<int, int>> drinks;
    int v_max = INT_MIN;
    for (int i = 0; i < n; i++) {
        int c, v; std::cin >> c >> v;
        drinks.push_back(std::make_pair(c, v));
        v_max = std::max(v_max, v);
    }

    int q_max = k + v_max;

    std::vector<std::vector<std::pair<int, int>>> memo(n, std::vector<std::pair<int, int>>(q_max, std::make_pair(0, 0)));

    for (int j = 1; j < q_max; j++) {
        int num_bottles_for_vol_j = ((j - 1)/drinks[0].second) + 1; // How many bottle to buy to get volume at least j
        memo[0][j] = std::make_pair(num_bottles_for_vol_j * drinks[0].first, 1);
    }

    for (int i = 1; i < n; i++) { // for every drink
        memo[i][0] = memo[i - 1][0]; // When adding this drink but no volume, take value from previous drink and no volume

        int ci = drinks[i].first; // cost of drink i
        int vi = drinks[i].second; // volume of drink i

        for (int j = 1; j < q_max; j++) { // for every quantity
            std::pair<int, int> with = memo[i][j - vi]; // taking a bottle of this drink
            std::pair<int, int> without = memo[i - 1][j]; // don't take this drink

            int c_min;
            int d_max;

            // considering taking a bottle of this drink
            if (j < vi) { // volume of current bottle is sufficient, so we can forget about all other bottles
                c_min = ci;
                d_max = 1;
            } else {
                c_min = ci + with.first;
                d_max = with.second;
                if (with == memo[i - 1][j - vi]) {
                    // if this is a different drink from before, we increment the counter
                    d_max++;
                }
            }

            // considering not taking a bottle of this drink
            if (without.first < c_min) {
                // cheaper cost -> better
                c_min = without.first;
                d_max = without.second;
            } else if (without.first == c_min && without.second > d_max) {
                d_max = without.second; // same price, but more drinks -> better
            }

            memo[i][j] = std::make_pair(c_min, d_max);
        }
    }

    std::cout << memo[n - 1][k].first << " " << memo[n - 1][k].second << std::endl;

}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}
