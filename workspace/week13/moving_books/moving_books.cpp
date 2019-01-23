// Disclaimer: master solution
#include <iostream>
#include <vector>
#include <set>
#include <climits>
#include <algorithm>

void testcase() {
    int N, M; std::cin >> N >> M;
    std::vector<int> strength(N);
    for (int i = 0; i < N; i++) {
        std::cin >> strength[i];
    }

    std::vector<int> boxes(M);
    std::multiset<int, std::greater<int>> ws;
    for (int i = 0; i < M; i++) {
        std::cin >> boxes[i];
        ws.insert(boxes[i]);
    }

    int maxw = -1;
    int maxs = -1;
    for (int i = 0; i < N; ++i)
            maxs = std::max(maxs, strength[i]);
    for (int i = 0; i < M; ++i)
        maxw = std::max(maxw, boxes[i]);
    if (maxw > maxs) {
        std::cout << "impossible" << std::endl;
        return; // Go to next test case.
    }

    std::sort(strength.begin(), strength.end(), std::greater<int>());

    int r = 0;
    while (!ws.empty()) {
        r++;
        for (int i = 0; i < N; i++) {
            auto b = ws.lower_bound(strength[i]);
            if (b != ws.end()) {
                ws.erase(b);
            } else {
                break;
            }
        }
    }

    std::cout << 3* r - 1 << std::endl;

}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t--) testcase();
    return 0;
}
