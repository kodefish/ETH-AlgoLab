#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

typedef std::pair<int, int> pair_i;

bool sort_by_detonation_time(const pair_i& p1, const pair_i& p2) {
    return p1.first < p2.first;
}

void testcase() {
    int n; std::cin >> n;
    std::vector<int> balls(n);
    std::cin >> balls[0];
    for (int i = 1; i < n; i++) {
        int det; std::cin >> det;

        // Detonation time is at most the input, but at least the parent's detonation time - 1
        // This makes sure we diffuse the bombs in such a way that a early detonation time isn't
        // masked by high detonation times on children
        int parent_det_i = balls[(i - 1)/ 2];
        balls[i] = std::min(parent_det_i - 1, det);
    }

    // Keep track of balls (and their index) we can difuse without setting off the ones underneath
    std::priority_queue<pair_i, std::vector<pair_i>, std::greater<pair_i>> can_diffuse;
    // Add balls initally on the floor
    for (int i = (n - 1)/2; i < n; i++) {
        can_diffuse.push(std::make_pair(balls[i], i));
    }

    // Keeps track of how many childs of bomb have been diffused (0 or 1)
    std::vector<bool> other_kid_diffused(n, false);

    // Keep diffusing bombs while it is still possible
    int time = 0;
    while (!can_diffuse.empty()) {
        pair_i bomb = can_diffuse.top();
        if (bomb.first - time < 1) {
            // boom.
            std::cout << "no" << std::endl;
            return;
        } else {
            // If we can diffuse the bomb
            can_diffuse.pop(); // diffuse the bomb

            // If not root node
            if (bomb.second > 0) {
                // Compute index of ball on top of diffused bomb
                int parent_idx = (bomb.second - 1)/ 2;

                // Check if the parent should be added to the diffusable bombs list (i.e. all his kids have been diffused)
                if (other_kid_diffused[parent_idx]) {
                    can_diffuse.push(std::make_pair(balls[parent_idx], parent_idx));
                } else {
                    other_kid_diffused[parent_idx] = true;
                }
            }
        }
        time++;
    }
    std::cout << "yes" << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t--) testcase();
    return 0;
}
