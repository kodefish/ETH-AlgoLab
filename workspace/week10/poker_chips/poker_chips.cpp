#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <boost/dynamic_bitset.hpp>

int max_points_rec(std::vector<std::vector<int>> chips, std::vector<int> idx) {
    std::map<int, std::vector<int>> same_col_loc;
    for (int i = 0; i < chips.size(); i++) {
        if (idx[i] < 0) continue;
        same_col_loc[chips[i][idx[i]]].push_back(i); 
    }

    // For all possible colors
    int max_points = 0;
    for (auto it = same_col_loc.begin(); it != same_col_loc.end(); it++) {
        int col = it->first; 
        std::vector<int> possible_locations = it->second;
        // Try all possible subsets
        for (unsigned long mask = 0; mask < (1 << possible_locations.size()); mask++) {
            boost::dynamic_bitset<> bit_mask(possible_locations.size(), mask);
            std::vector<int> new_idx;
            new_idx = idx;
            for (int stack = 0; stack < possible_locations.size(); stack++) {
                int stack_idx = possible_locations[stack];
                if (bit_mask[stack]) {
                    // Take chip off stack i 
                    new_idx[stack_idx] = idx[stack_idx] - 1;
                } else {
                    new_idx[stack_idx] = idx[stack_idx];
                }
            }

            int k = bit_mask.count();
            int points = k > 1 ? std::pow(2, k - 2):0;
            if (k > 0) points += max_points_rec(chips, new_idx);
            max_points = std::max(max_points, points);
        }
    }
    return max_points;
}

void testcase() {
    int N; std::cin >> N;

    std::vector<int> num_chips(N);
    for (int i = 0; i < N; i++) {
        std::cin >> num_chips[i];
        num_chips[i] -= 1;
    }

    std::vector<std::vector<int>> chips(N);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < num_chips[i] + 1; j++) {
            int c_i_j; std::cin >> c_i_j;
            chips[i].push_back(c_i_j); 
        }
    }

    std::cout << "read input" << std::endl;

    std::cout << max_points_rec(chips, num_chips) << std::endl;
}

int main() {
    int t; std::cin >> t;
    while(t--) testcase();
    return 0;
}
