// Master solution
#include <iostream>
#include <vector>
#include <algorithm>

typedef std::vector<int>    VI;
typedef std::vector<VI>     VVI;

void back_track(int id, int ubound, VVI &F, VVI &assignment, const VI &planks) {
    if (id >= ubound) {
        VI tuple(4, 0);
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < assignment[i].size(); j++) {
                tuple[i] += planks[assignment[i][j]]; 
            }
        }
        F.push_back(tuple);
        return;
    }

    for (int i = 0; i < 4; i++) {
        assignment[i].push_back(id); // Try to put id-th plank to i-th set
        back_track(id + 1, ubound, F, assignment, planks); // Recurse
        assignment[i].pop_back(); //Remove id-th plank from i-th set
    }
}

void testcase() {
    int N; std::cin >> N;
    VI planks(N);
    int sum = 0;
    for (int i = 0; i < N; i++) {
        std::cin >> planks[i]; 
        sum += planks[i];
    }

    if (sum % 4 != 0) {
        std::cout << 0 << std::endl;
        return;
    }

    VVI F1, assignment(4);
    // Generate all 4-tuple for the first half of the set
    back_track(0, N/2, F1, assignment, planks);

    VVI F2, assignment2(4);
    // Generate all 4-tuple for the first half of the set
    back_track(N/2, N, F2, assignment2, planks);
    std::sort(F2.begin(), F2.end());

    long long result = 0;
    for (int idx = 0; idx < F1.size(); idx++) {
        VI member = F1[idx];
        for (int i = 0; i < 4; i++) {
            member[i] = sum / 4 - member[i];
        }

        std::pair<VVI::iterator, VVI::iterator> bounds;
        bounds = std::equal_range(F2.begin(), F2.end(), member);
        long long counter = std::distance(bounds.first, bounds.second);
        result += counter;
    }

    std::cout << result / 24 << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}
