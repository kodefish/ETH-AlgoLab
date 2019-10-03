#include <iostream>
#include <vector>
#include <deque>
#include <climits>
#include <set>

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

     // Find min and max indices in the stacks
    int min_i = 0, min_dist = INT_MAX;
    std::set<std::pair<int, int>> words;
    for(int i = 0; i < n; i++) {
        words.insert(std::make_pair(p[i][0], i));
    }
    bool cond = true;
    while(cond) {
        auto max_p = *words.rbegin();
        auto min_p = *(--words.rend());


        int max = max_p.first;
        int min = min_p.first;

        // Compute distance containg all the words
        int dist = max - min + 1;

        // Check if it minimal
        if (dist < min_dist) min_dist = dist;

        // Increment word with smallest indice (to make the interval smaller)
        int min_i = min_p.second;
        p[min_i].pop_front();
        // replace old <min_i, min> with new one
        // If a stack is empty, word will no longer appear -> shortest interval has been found by now
        cond = !p[min_i].empty();
        if (cond) {
            words.erase(min_p);
            words.insert(std::make_pair(p[min_i][0], min_i));
        }
    }

    std::cout << min_dist << std::endl;
}

int main() {
    std::ios::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t--) testcase();
    return 0;
}
