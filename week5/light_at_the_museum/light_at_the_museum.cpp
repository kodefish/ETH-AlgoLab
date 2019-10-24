#include <iostream>
#include <vector>
#include <climits>

void testcase() {
    int n, m; std::cin >> n >> m;

    std::vector<int> target_brightness(m);
    for (int i = 0; i < m; i++) {
        std::cin >> target_brightness[i];
    }

    // for every switch, for every room, record amount of on and off switches
    std::vector<std::vector<std::pair<int, int>>> lights(n, std::vector<std::pair<int, int>>(m));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            int on, off; std::cin >> on >> off;
            lights[i][j] = std::make_pair(on, off);
        }
    }

    // bruteforce, i.e try all possible subsets of lights
    int min_num_flicks = INT_MAX;
    for (int s = 0; s < 1<<n; ++s) {
        // Achieved brightness in each room
        std::vector<int> achieved_brightness(m, 0);
        int num_flicks = 0;
        for (int i = 0; i < n; i++) {
            // Count the number of lights we turn on
            if (s & 1<<i) num_flicks++;
            for (int j = 0; j < m; j++) {
                if (s & 1<<i) {
                    // light i is switched on, add brightness of off to each room
                    achieved_brightness[j] += lights[i][j].second;
                } else {
                    // light i is switched off, add brightness of on to each room
                    achieved_brightness[j] += lights[i][j].first;
                }
            }
        }

        bool satisfied = true;
        for (int i = 0; i < m; i++) {
            satisfied = satisfied && (target_brightness[i] == achieved_brightness[i]);
        }

        if (satisfied && num_flicks < min_num_flicks) min_num_flicks = num_flicks;
    }

    if (min_num_flicks < INT_MAX) std::cout << min_num_flicks << std::endl;
    else std::cout << "impossible" << std::endl;
}

int main() {
    std::ios::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}
