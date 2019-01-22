// Disclaimer - this solution only gives 60/100 points, as the last testcase times out
#include <iostream>
#include <vector>

void testcase() {
    int N; std::cin >> N;
    const int NUM_SIDES = 4;

    std::vector<int> planks(N);
    int perim = 0;
    for (int i = 0; i < N; i++) {
        int l; std::cin >> l;
        planks[i] = l;
        perim += l;
    }

    // Compute the length of each side
    if (perim % 4 != 0) {
        std::cout << "0" << std::endl;
        return;
    }
    int side_length = perim / NUM_SIDES;

    // If there is an edge that is greater than the side, then there is no chance 
    // To include all the pieces
    for (int i = 0; i < N; i++) {
        if (planks[i] > side_length) {
            std::cout << "0" << std::endl;
            return;
        }
    }

    // Count the number of subsets that sum up to perim / 4
    std::vector<int> possible_subsets;
    for (int s = 0; s < 1 << N; s++) {
        int sum = 0; int num_edges_used = 0;
        for (int i = 0; i < N; i++) {
            if (s & 1 << i) {
                sum += planks[i];
                num_edges_used++;
            }
        }
        if (sum == side_length) possible_subsets.push_back(s);
    }

    // Count all the possible combinations
    int count = 0;
    if (possible_subsets.size() >= NUM_SIDES) {
        for (int i = 0; i < possible_subsets.size() - 3; i++) {
            int subset1 = possible_subsets[i];
            for (int j = i + 1; j < possible_subsets.size() - 2; j++) {
                int subset2 = possible_subsets[j];
                for (int k = j + 1; k < possible_subsets.size() - 1; k++) {
                    int subset3 = possible_subsets[k];
                    for (int l = k + 1; l < possible_subsets.size(); l++) {
                        int subset4 = possible_subsets[l];
                        // Check to see if the 4 are overlapping
                        if (subset1 + subset2 + subset3 + subset4 == (1 << N) - 1)
                            count++; 
                    }
                }
            }
        } 
    }
    std::cout << count << std::endl;
}

int main() {
    int t; std::cin >> t;
    while(t--) testcase();
    return 0;
}


