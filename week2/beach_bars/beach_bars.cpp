#include <iostream>
#include <vector>
#include <climits>
#include <cmath>
#include <algorithm>

void testcase() {
    int n; std::cin >> n;
    std::vector<double> p(n);
    for (int i = 0; i < n; i++) {
        std::cin >> p[i];
    }

    std::sort(p.begin(), p.end());

    // Sliding window pointers
    int i = 0, j = 0;
    double i_loc = p[i], j_loc = p[j];
    // Window variables
    int num_parasols = 1;
    // Global variables
    int max_num_parasols = INT_MIN;
    double min_l_dist = INT_MAX;
    std::vector<int> optimal_positions;

    // Sliding window
    while (j < n) {
        // Analyze current window if window is valid. Window is valid if it spans a distance of 200 at most
        if ((j_loc - i_loc) <= 200.0) {
            if (num_parasols > max_num_parasols) {
                max_num_parasols = num_parasols;
                min_l_dist = std::abs(std::ceil((j_loc - i_loc) / 2.0));
                optimal_positions.clear();
            }

            // If the number of parsols is the same, but the min_l_dist is smaller, then it is also better
            if (std::abs(std::ceil((j_loc - i_loc) / 2.0)) < min_l_dist && num_parasols == max_num_parasols) {
                min_l_dist = std::abs(std::ceil((j_loc - i_loc) / 2.0));
                optimal_positions.clear();
            }

            // If optimal case, just different position, also save that location
            if (num_parasols == max_num_parasols
                && std::abs(std::ceil((j_loc - i_loc) / 2.0)) == min_l_dist) {
                if (std::fmod(i_loc + j_loc, 2.0) == 0) {
                    optimal_positions.push_back((i_loc + j_loc) / 2);
                } else {
                    optimal_positions.push_back(std::floor((i_loc + j_loc) / 2));
                    optimal_positions.push_back(std::ceil((i_loc + j_loc) / 2));
                }
            }
        }
        // Slide the window
        if (j_loc - i_loc < 200) {
            j++;
            j_loc = p[j];
            num_parasols++;
        } else {
            i++;
            i_loc = p[i];
            num_parasols--;
        }
    }

    std::cout << max_num_parasols << " " << min_l_dist << std::endl;
    for(int i = 0; i < optimal_positions.size(); i++) {
        std::cout << optimal_positions[i] << " ";
    }
    std::cout << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t--) testcase();
    return 0;
}
