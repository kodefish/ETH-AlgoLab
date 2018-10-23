#include <iostream>
#include <vector>
#include <algorithm>

bool sort_boat_pairs(std::pair<int, int> b1, std::pair<int, int> b2) {
    return b1.second < b2.second;
}

void testcase() {
    // Parse the input
    int n; std::cin >> n;
    std::vector<std::pair<int, int>> boats(n);
    for (int i = 0; i < n; i++) {
        int l_i, p_i; std::cin >> l_i >> p_i;
        boats[i] = std::make_pair(l_i, p_i);
    }

    // Sort by ascending ring position
    std::sort(boats.begin(), boats.end(), sort_boat_pairs);

    int end_of_line = boats[0].second;
    int prev_eol = end_of_line;
    int num_boats = 1;
    // For every anchor point
    for(int i = 1; i < n; i++) {
        int l_i = boats[i].first;
        int p_i = boats[i].second;

        // If the anchor point is after the current last boat
        if (end_of_line <= p_i) {
            // Put the boat such that it is as close to the school as possible
            prev_eol = end_of_line;
            end_of_line = std::max(end_of_line + l_i, p_i);
            num_boats++;
        } else { // If the anchor point is before the current end of line
            // Check if maybe taking this boat instead yields a closer end of line
            // If they are the same or smaller, take the new boat, so update the end of line, 
            // but the number of boats stays the same
            int potential_end_of_line = std::max(p_i, prev_eol + l_i);
            if (potential_end_of_line < end_of_line) {
                end_of_line = potential_end_of_line;
            }
        }
    }

    std::cout << num_boats << std::endl;
}

int main() {
    int t; std::cin >> t;
    while(t--) testcase();
    return 0;
}
