#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

typedef std::vector<int> vec_i;
typedef std::vector<double> vec_d;
typedef std::vector<vec_d> vec_dd;

const double MEMO_INIT = -1.0;

double dp(int d, int w, const vec_d &probabilities, const int days, const int m, vec_dd &memo) {
    if (w >= m) return 1; // reached goal, stop betting
    if (w == 0 || d == days) return 0; // no more money, or magician left

    if (memo[w][d] != MEMO_INIT) return memo[w][d];

    double prob = 0;
    for (int j = 0; j <= w; j++) {
        double prob_by_winning = probabilities[d] * dp(d + 1, w + j, probabilities, days, m, memo);
        double prob_by_losing = (1 - probabilities[d]) * dp(d + 1, w - j, probabilities, days, m, memo);
        prob = std::max(prob, prob_by_winning + prob_by_losing);
    }

    memo[w][d] = prob;
    return prob;
}

void testcase() {
    int n, k, m; std::cin >> n >> k >> m;
    vec_d probabilities(n);
    for (int i = 0; i < n; i++) std::cin >> probabilities[i];


    vec_dd memo(m, vec_d(n, MEMO_INIT));
    double prob = dp(0, k, probabilities, n, m, memo);

    std::cout << prob << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cout << std::fixed << std::setprecision(5);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}
