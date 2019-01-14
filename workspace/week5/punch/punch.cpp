#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <map>
#include <set>

void testcase() {
    int n, k; std::cin >> n >> k;

    std::map<double, std::vector<int>> drinks; // (price per litre, list of volumes sold)
    for (int i = 0; i < n; i++) {
        double cost; int volume; std::cin >> cost >> volume;
        drinks[cost/volume].push_back(volume);
    }

    int punch_volume = 0;
    double price = 0; 
    auto itr = drinks.begin();
    std::set<std::pair<int, int>> selected_drinks;
    while (punch_volume < k && itr != drinks.end()) {
        // Sort from small to large volumes
        std::sort(itr->second.begin(), itr->second.end());
        
        int i = 0;
        while(punch_volume != k) {
            // If we can fit this bottle in the punch, then buy it
            if (punch_volume + itr->second[i] <= k) {
                // Include a bottle of beverage of volume i
                punch_volume = punch_volume + itr->second[i];
                price += itr->first * itr->second[i];
                selected_drinks.insert(std::make_pair(itr->first, i));
                i = (i + 1) % itr->second.size();
            } else if (i > 0) {
                // If not, try smaller bottle of same price
                i--;
            } else {
                // If all fails, try a pricier bottles
                itr++;
                break;
            }
        }   
    }

    std::cout << price << " " << selected_drinks.size() << std::endl;

}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t--) testcase();
    return 0;
}
