#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>

using namespace std;

const double MEMO_INIT = -1;

double dp(int day, int wealth, int num_days, int min_wealth, const vector<double> &prob, vector<vector<double>> &memo) {
    if (wealth >= min_wealth) {
        return 1; // we done
    }
    if (day == num_days) {
        return 0; // magician is gone and we don't have enough
    }

    if (memo[day][wealth] != MEMO_INIT) return memo[day][wealth];

    double p = 0;
    for (int b = 0; b <= wealth; b++) {
        double p_win = dp(day + 1, wealth + b, num_days, min_wealth, prob, memo); // prob if we win today and bet b
        double p_lose = dp(day + 1, wealth - b, num_days, min_wealth, prob, memo); // prob if we lose today and bet b
        p = std::max(p, (prob[day] * p_win) + ((1 - prob[day]) * p_lose));
    }

    memo[day][wealth] = p;
    return p;
}

void testcase() {
    int n, k, m; cin >> n >> k >> m;
    vector<double> prob(n);
    for (int i = 0; i < n; i++) cin >> prob[i];

    vector<vector<double>> memo(n, vector<double>(m, MEMO_INIT));
    cout << dp(0, k, n, m, prob, memo) << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(5);
    int t; cin >> t;
    while (t--) testcase();
    return 0;
}

