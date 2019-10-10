#include <iostream>
#include <vector>
#include <climits>

typedef std::vector<int> vec;
typedef std::vector<vec> vec2;
typedef std::vector<vec2> vec3;

vec2 stages;
vec cost;
vec3 memo;


const int MEMO_INIT = -1;

int f(int idx, bool repair, bool father_repaired) {
    // Base case (idx is a leaf in the tree)
    if (stages[idx].empty()) return father_repaired ? 0 : cost[idx];

    // Memo check
    if (memo[repair][father_repaired][idx] != MEMO_INIT) return memo[repair][father_repaired][idx];


    // Reccurence
    // Consider 3 cases
    int result = MEMO_INIT;
    if (repair) {
        result = cost[idx];
        for (int i = 0; i < stages[idx].size(); i++) {
            // For each child, compute the result of them doing whatever knowing that I am on
            result += std::min(f(stages[idx][i], false, true), f(stages[idx][i], true, true));
        }
    } else {
        if (father_repaired) {
            result = 0;
            for (int i = 0; i < stages[idx].size(); i++) {
                // For each child, compute the result of them doing whatever knowing that I am off
                result += std::min(f(stages[idx][i], false, false), f(stages[idx][i], true, false));
            }
        } else {
            // Compute values for all the kids (of turing them on, and letting them do whatever)
            vec on_result(stages[idx].size());
            vec whatever_result(stages[idx].size());
            int sum_of_whatever = 0;
            for (int i = 0; i < stages[idx].size(); i++) {
                on_result[i] = f(stages[idx][i], true, false);
                whatever_result[i] = std::min(f(stages[idx][i], false, false), f(stages[idx][i], true, false));
                sum_of_whatever += whatever_result[i];
            }

            // Find the combination that minimizes the overall result by replacing each whatever_sum by it's on sum
            // This guarantees we find a minimum result with at least one child that is repaired.
            result = INT_MAX;
            for (int i = 0; i < stages[idx].size(); i++) {
                int tmp_result = sum_of_whatever - whatever_result[i] + on_result[i];
                result = std::min(tmp_result, result);
            }
        }
    }

    memo[repair][father_repaired][idx] = result;
    return result;
}

void testcase() {
    int n; std::cin >> n;

    stages = vec2(n);
    for (int i = 0; i < n - 1; i++) {
        int u, v; std::cin >> u >> v;
        stages[u].push_back(v);
    }

    cost = vec(n);
    for (int i = 0; i < n; i++) {
        std::cin >> cost[i];
    }

    memo = vec3(2, vec2(2, vec(n, MEMO_INIT)));
    std::cout << std::min(f(0, true, false), f(0, false, false)) << std::endl;

}

int main() {
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}

