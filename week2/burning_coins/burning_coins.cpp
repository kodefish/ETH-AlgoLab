#include <iostream>
#include <vector>
#include <climits>

typedef std::vector<int> vec;
typedef std::vector<vec> vec2;
typedef std::vector<vec2> vec3;

const int MEMO_INIT = -1;

/**
 * Recursive formulation of the burning coins problem
 * i, j are the indices in which we want to find an optimal pick
 * t is the turn counter (if it is pair, then maximise gains, otherwise minimise)
 */
int f(int i, int j, int t, vec const& coins, vec3 &memo){
    // Base case, i = j
    if (i == j) {
        if (t % 2 == 0) return coins[i];
        else  return 0;
    }

    // Check memo
    if (memo[t % 2][i][j] != MEMO_INIT) return memo[t % 2][i][j];

    // Reccurence
    int result = -1;
    if (t % 2 == 0) {
        result = std::max(coins[i] + f(i + 1, j, t + 1, coins, memo), coins[j] + f(i, j - 1, t + 1, coins, memo));
    } else {
        result = std::min(f(i + 1, j, t + 1, coins, memo), f(i, j - 1, t + 1, coins, memo));
    }
    memo[t % 2][i][j] = result;
    return result;
}

void testcase() {
    int n; std::cin >> n;
    vec coins(n);
    vec3 memo(2, vec2(n, vec(n, MEMO_INIT)));
    for (int i = 0; i < n; i++) {
        std::cin >> coins[i];
    }

    std::cout << f(0, n - 1, 0, coins, memo) << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}

