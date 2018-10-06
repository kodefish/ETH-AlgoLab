#include <iostream>
#include <set>

void testcase() {
    int num_elem; std::cin >> num_elem;
    std::set<int> nums;

    for(int i = 0; i < num_elem; i++) {
        int insert; std::cin >> insert;
        int tmp; std::cin >> tmp;

        if(insert == 0){
            nums.insert(tmp);
        } else {
            auto search = nums.find(tmp);
            if (search != nums.end()) {
                nums.erase(search);
            }
        }
    }

    if (nums.size() > 0) {
        for(auto& i: nums) std::cout << i << ' ';
    } else {
        std::cout << "Empty";
    }
    std::cout << '\n';

    
}

int main() {
    int t; std::cin >> t;
    for(int i = 0; i < t; i++) {
        testcase();
    }
}
