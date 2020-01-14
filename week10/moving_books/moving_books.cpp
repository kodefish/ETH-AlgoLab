#include <iostream>
#include <vector>
#include <map>
#include <climits>

void testcase() {
    // read in num friends and boxes
    std::size_t n, m; std::cin >> n >> m;

    // read in friend's strengths and keep track of strongest friend
    std::vector<int> strength(n);
    int max_strength = INT_MIN;
    for (std::size_t i = 0; i < n; i++) {
        std::cin >> strength[i];
        if (max_strength < strength[i]) max_strength = strength[i];
    }

    // count boxes by weight
    std::map<int, int> boxes;
    int max_weight = INT_MIN;
    for (std::size_t i = 0; i < m; i++) {
        int w; std::cin >> w;
        boxes[w]++;
        if (max_weight < w) max_weight = w;
    }

    // if no one is strong enough to lift the heaviest box -> impossible
    if (max_strength < max_weight) {
        std::cout << "impossible" << std::endl;
        return;
    }

    // distribute all the boxes among the friends by having each friend
    std::vector<int> distrib(n, 0);
    // keep track of friends that can lift the remaining boxes (everyone at the beginning)
    std::vector<int> lifter_indices;
    for (int i = 0; i < n; i++) lifter_indices.push_back(i);

    // pick the heaviest possible box available that they can lift
    for (int i = 0, l = 0; i < m; l = (l+1) % lifter_indices.size()) {
        // get index of available lifter
        int lifter_idx = lifter_indices[l];

        // figure out the max needed strength of lifter based on remaining boxes
        while (boxes[strength[lifter_idx]] == 0 && strength[lifter_idx] > 0) strength[lifter_idx]--;

        if (strength[lifter_idx] == 0) { // if lifter is not able to pick anything up
            lifter_indices.erase(lifter_indices.begin() + l); // remove friend from available lifters
            l--;
            continue;
        }

        boxes[strength[lifter_idx]]--; // pick up heaviest possible box
        distrib[lifter_idx]++; // count number of boxes carried
        i++;
    }

    // boxes distributed, total time just depends on the friend with most boxes
    int max_boxes_to_carry = INT_MIN;
    for (int i = 0; i < n; i++) {
        max_boxes_to_carry = std::max(max_boxes_to_carry, distrib[i]);
    }

    // total time is max_boxes_to_carry - 1 round trips + one trip down the stairs
    std::cout << (max_boxes_to_carry - 1) * 3 + 2 << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}
