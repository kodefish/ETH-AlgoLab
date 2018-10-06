#include <iostream>
#include <vector>

#define MAX(a, b) ((a > b) ? a : b)
#define MIN(a, b) ((a < b) ? a : b)

typedef std::vector<int> ivector;
typedef std::vector<std::vector<std::vector<int>>> ivector3d;

/**
 * Computes the max sum one can get based on the turn number, and the available coins (begin_idx and 
 * end_idx). Using memoization, it avoids recomputing values
 */
int max_sum(const ivector &coins, int begin_idx, int end_idx, int turn, ivector3d &memo) {

    // If value already computed, return it, else compute and save it
    int memo_val = memo[turn % 2][begin_idx][end_idx];
    if (memo_val != -1) {
        return memo_val;
    }

    // Check if only one coin is left - bottom of recursion
    if (begin_idx == end_idx) {
        return ((turn % 2 == 0) ? coins[begin_idx] : 0);
    }

    if (turn % 2 == 0) {
        // If my turn, then we count the taken coin
        int sum_begin = coins[begin_idx] + max_sum(coins, begin_idx + 1, end_idx, turn + 1, memo);
        int sum_end = coins[end_idx] + max_sum(coins, begin_idx, end_idx - 1, turn + 1, memo);
        int max_sum = MAX(sum_begin, sum_end);
        memo[turn % 2][begin_idx][end_idx] = max_sum;
        return max_sum;
    } else {
        // If not my turn, then we don't count the taken coin
        int sum_begin = max_sum(coins, begin_idx + 1, end_idx, turn + 1, memo);
        int sum_end = max_sum(coins, begin_idx, end_idx - 1, turn + 1, memo);
        int min_sum = MIN(sum_begin, sum_end);
        memo[turn % 2][begin_idx][end_idx] = min_sum;
        return min_sum;
    }
}

void testcase() {

    // Read num coins
    int num_coins; std::cin >> num_coins;

    // Read coins
    ivector coins(num_coins, 0);
    for(int i = 0; i < num_coins; ++i) {
            std::cin >> coins[i];
    }

    // Memoization
    ivector3d memo(2, std::vector<std::vector<int>>(num_coins, std::vector<int>(num_coins, -1)));
    std::cout << max_sum(coins, 0, num_coins - 1, 0, memo) << std::endl;

}

int main() {
    int t; std::cin >> t;

    for(int i = 0; i < t; ++i) {
        testcase();
    }
}
