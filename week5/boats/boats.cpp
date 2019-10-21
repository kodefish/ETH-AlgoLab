#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

struct boat {
    int length;
    int anchor;

    const bool operator<(const boat& b) const { return this->anchor < b.anchor; }
};

void testcase() {
    int n; std::cin >> n;
    std::vector<boat> boats;
    for (int i = 0; i < n; i++) {
        int length, anchor;
        std::cin >> length >> anchor;
        boats.push_back(boat{length, anchor});
    }

    // Sort in order of increasing anchor position
    std::sort(boats.begin(), boats.end());

    int current_end = INT_MIN;
    int previous_end;
    int num_boats = 0;
    for (int i = 0; i < n; i++) {
        if (current_end <= boats[i].anchor) {
            // We can dock boat i becasue it's anchor comes after the current end
            // So the current end becomes at best the anchor point (boat docked at right most corner),
            // or current end + length of the boat
            num_boats++;
            previous_end = current_end; // Save this, cuz we might replace this boat in the next iteration if it turns out that
                                        // it wasn't such a good idea after all
            current_end = std::max(boats[i].anchor, current_end + boats[i].length);
        } else {
            // Whether we take this boat or the previous doesn't change the number of boats, but may
            // lead to a closer end of boats allowing for more space down the line
            current_end = std::min(std::max(boats[i].anchor, previous_end + boats[i].length), current_end);
        }
    }

    std::cout << num_boats << std::endl;
}

int main() {
    std::ios::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}
