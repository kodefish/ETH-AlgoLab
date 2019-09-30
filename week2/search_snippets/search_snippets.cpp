#include <iostream>
#include <vector>
#include <deque>
#include <climits>

void testcase() {
    int n; std::cin >> n;
    std::vector<int> m(n, 0);
    for (int i = 0; i < n; i++) {
        std::cin >> m[i];
    }

    // Read in input into stacks
    std::vector<std::deque<int>> p;
    for (int i = 0; i < n; i++) {
        p.push_back(std::deque<int>());
        for (int j = 0; j < m[i]; j++) {
            int p_i_j; std::cin >> p_i_j;
            p[i].push_back(p_i_j);
        }
    }

    int min_i = 0, min_dist = INT_MAX;
    do {
        // Find min and max indices in the stacks
        int min = INT_MAX, max = INT_MIN;
        for(int i = 0; i < n; i++) {
            if (p[i][0] < min) {
                min = p[i][0];
                min_i = i;
            }
            if (p[i][0] > max) {
                max = p[i][0];
            }
        }

        // Compute distance containg all the words
        int dist = max - min + 1;

        // Check if it minimal
        if (dist < min_dist) min_dist = dist;

        // Increment word with smallest indice (to make the interval smaller)
        p[min_i].pop_front();
    } while(!p[min_i].empty()); // If a stack is empty, word will no longer appear -> shortest interval has been found by now

    std::cout << min_dist << std::endl;
}

int main() {
    std::ios::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t--) testcase();
    return 0;
}
