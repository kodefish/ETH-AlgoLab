#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>

void testcase() {
    int num_elems; std::cin >> num_elems;
    // Create map
    std::map<std::string, std::vector<int>> elems;
    for(int i = 0; i < num_elems; i++) {
        int value; std::cin >> value;
        std::string key; std::cin >> key;

        if(value) {
            // add value to the vector associated to the key
            elems[key].push_back(value);
        } else {
            // erase everything assosiated to the key
            auto search = elems.find(key);
            if (search != elems.end()){
                elems.erase(search);
            }
        }
    }

    // Print the elements associated to the key
    std::string key; std::cin >> key;
    std::vector<int> ints = elems[key];
    if (ints.size() > 0) {
        // Sort the array
        std::sort(ints.begin(), ints.end());
        for(int i = 0; i < ints.size(); i++) {
            std::cout << ints[i] << " ";
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
