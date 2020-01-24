#include <iostream>
#include <vector>
#include <algorithm>

typedef std::vector<int> vec_i;
typedef std::vector<vec_i> vec_ii;

void back_track(int id, int ubound, vec_ii &f, vec_ii &assignment, const vec_i &planks) {
    if (id >= ubound) {
        vec_i tuple(4, 0);
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < assignment[i].size(); j++) {
                tuple[i] += planks[assignment[i][j]];
            }
        }
        f.push_back(tuple);
        return;
    }

    for (int i = 0; i < 4; i++) {
        // Try to put id-ith plank in i-th set
        assignment[i].push_back(id);
        // Recurse
        back_track(id+1, ubound, f, assignment, planks);
        // Remove id-th plank from i-th set
        assignment[i].pop_back();
    }

}

void testcase() {
    int n; std::cin >> n;
    vec_i planks(n);
    int sum = 0;
    for (int i = 0; i < n; i++) {
        std::cin >> planks[i];
        sum += planks[i];
    }

    // If total sum isn't divisible by 4, then it's impossible to create a square
    if (sum % 4 != 0) {
        std::cout << 0 << std::endl;
        return;
    }

    // Generate all 4-tuples for the first half of the set
    vec_ii f1, assignment(4);
    back_track(0, n/2, f1, assignment, planks);

    // Generate all 4-tuples for the first half of the set
    vec_ii f2, assignment2(4);
    back_track(n/2, n, f2, assignment2, planks);
    std::sort(f2.begin(), f2.end());

    long long result = 0;
    for (int i = 0; i < f1.size(); i++) {
        vec_i member = f1[i];
        for (int i = 0; i < 4; i++) {
            member[i] = sum/4 - member[i];
        }

        std::pair<vec_ii::iterator, vec_ii::iterator> bounds;
        bounds = std::equal_range(f2.begin(), f2.end(), member);
        result += std::distance(bounds.first, bounds.second);
    }

    std::cout << result / 24 << std::endl;

}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}
