#include <iostream>
#include <vector>
#include <algorithm>

bool sort_non_decreasing(int i, int j) {
    return j < i;
}

void testcase() {
    int num_elems; std::cin >> num_elems;
    std::vector<int> ints;
    for(int i = 0; i < num_elems; i++) {
        int tmp; std::cin >> tmp;
        ints.push_back(tmp);
    }

    int sort; std::cin >> sort;

    if (sort != 0) {
        std::sort(ints.begin(), ints.end(), sort_non_decreasing);
    } else {
        std::sort(ints.begin(), ints.end());
    }

    for (std::vector<int>::iterator it=ints.begin(); it!=ints.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << '\n';
    
}

int main() {
    int t; std::cin >> t;
    for(int i = 0; i < t; i++) {
        testcase();
    }
}
