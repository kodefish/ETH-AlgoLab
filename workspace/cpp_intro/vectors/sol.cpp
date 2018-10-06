#include <iostream>
#include <vector>

void testcase() {
    // Create initial vector
    int num_elem; std::cin >> num_elem;
    std::vector<int> nums;   

    // Fill vector
    for(int i = 0; i < num_elem; i++) {
        int tmp; std::cin >> tmp;
        nums.push_back(tmp);
    }

    // Remove element at given index
    int rem_idx; std::cin >> rem_idx;
    nums.erase(nums.begin() + rem_idx);

    // Remove elements in range
    std::cin >> rem_idx;
    int rem_idx_end;
    std::cin >> rem_idx_end;

    nums.erase(nums.begin() + rem_idx, nums.begin() + rem_idx_end + 1);

    if(nums.size() > 0) {
        for(int i = 0; i < nums.size(); i++) {
            std::cout << nums[i] << " ";
        }
    } else {
        std::cout << "Empty";
    }

    std::cout << std::endl;
}

int main() {
    int t; std::cin >> t;
    for(int i = 0; i < t; i++) {
        testcase();
    }
}
