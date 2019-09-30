#include <iostream>
#include <vector>

void testcase() {
    int n; std::cin >> n;

    std::vector<int> h;
    for (int i = 0; i < n; i++){
        int h_i; std::cin >> h_i;
        h.push_back(h_i);
    }

    int limit = h[0];
    for (int i = 1; i < limit; i++) {
        int topple = i + h[i];
        if (topple > limit) limit = topple;
        if (limit > n) limit = n;
    }

    std::cout << limit << std::endl;
}

int main() {
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}
