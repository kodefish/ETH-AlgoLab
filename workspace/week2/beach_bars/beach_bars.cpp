#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <tuple>

void testcase() {

    // Read all the parasols
    int num_parasols; std::cin >> num_parasols;
    std::vector<int> parasol_position(num_parasols, INT_MIN);
    for(int i = 0; i < num_parasols; ++i) {
        std::cin >> parasol_position[i]; 
    }

    // Sort the positions
    std::sort(parasol_position.begin(), parasol_position.end());

    // Iterators over the positions vector
    std::vector<int>::iterator begin = parasol_position.begin();
    std::vector<int>::iterator end = begin;
    std::vector<int>::iterator inc_begin = begin; // used to see how far in the vector we are

    // Vector of tuples (location, num of reachable parasols, dist to furthest parasol)
    std::vector<std::tuple<int, int, int>> potential_locations;
    int max_reachable = INT_MIN;
    do{
        // Calculate number of reachable parasols for each location, and distance to the furthest one
        int num_reachable = end - begin + 1;
        // If the num reachable is smaller than the max, then skip
        if (num_reachable >= max_reachable) {
            if (num_reachable > max_reachable) {
                max_reachable = num_reachable;
                potential_locations.clear();
            }

            // Don't look at 2 iterators at the same parasol (there will always be a better position
            int tmpDist = *end + *begin;
            double precise_location = tmpDist / 2;
            // Since locations can only be integers, then we have to check the floor and the ceil if uneven
            std::vector<int> locations;
            if (tmpDist % 2 != 0) {
                if (precise_location < 0) {
                    locations.push_back(((int) precise_location) - 1);
                    locations.push_back((int) precise_location);
                } else {
                    locations.push_back((int) precise_location);
                    locations.push_back(((int) precise_location) + 1);
                }
            } else {
                locations.push_back((int) precise_location);
            }
    
            for(int i = 0; i < locations.size(); i++) {
                int a = *end - locations[i];
                int b = locations[i] - *begin;
                potential_locations.push_back(std::make_tuple(locations[i], num_reachable, ((a > b) ? a : b)));
            }
        }
        
        // If dist between the two would be larger than 100, have big boi catch up
        if(*(end + 1) - *begin > 2*100 || *end == parasol_position[num_parasols - 1]) {
            begin++;
        } else {
            end++;
        }
        
    } while (begin - inc_begin < num_parasols);


    // Out of the max reachables (potential locations is already filtered for max parasol), get the ones with min distance
    int min_dist = INT_MAX;
    std::vector<std::tuple<int, int, int>> final_locations; 
    for(int i = 0; i < potential_locations.size(); ++i) {
        int dist = std::get<2>(potential_locations[i]);

        // Reset min dist, and reject all previously added locations (because they won't satisfy the condition either) 
        if (dist < min_dist) {
            min_dist = dist;
            final_locations.clear();
        }

        if (dist == min_dist) {
            final_locations.push_back(potential_locations[i]);
        }
    }

    if (final_locations.size() > 0) {
        int max_parasol = std::get<1>(final_locations[0]);
        int dist = std::get<2>(final_locations[0]);

        std::cout << max_parasol << " " << dist << std::endl;

        bool first = true;
        for(int i = 0; i < final_locations.size(); ++i) {
            if(first) {
                first = false;
            } else {
                std::cout << " ";
            }
            std::cout << std::get<0>(final_locations[i]);
        }

        std::cout << std::endl;
    }
}

int main() {
    int t; std::cin >> t;

    for(int i = 0; i < t; ++i) {
        testcase();
    }

    return 0;

}
