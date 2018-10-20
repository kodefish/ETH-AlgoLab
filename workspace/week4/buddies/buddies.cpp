#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <algorithm>

void testcase() {

    // Read inputs (num_students, num_characteristics, and Dr. Fuzzman's solution
    int num_s, num_c, f; std::cin >> num_s >> num_c >> f;

    // Store student characteristics
    std::vector<std::set<std::string>> student_cs(num_s);
    for(int i = 0; i < num_s; i++) {
        std::set<std::string>> c;
        for(int j = 0; j < num_c; j++) {
            std::string characteristic; std::cin >> characteristic;
            c.insert(characteristic);
        }
        student_cs[i] = c;
    }

    // Build the possible pairings graph 
    for(int i = 0; i < num_s; i++) {
        for(int j = i; j < num_s; j++) {
            std::set<std::string>> intersect;
            std::set_intersection(student_cs[i].begin(), student_cs[i].end()
                student_cs[j].begin(), student_cs[j].end(),
                std::inserter(intersect, intersect.begin()));
            if (intersect.size() > f) {
                // Insert edge (i, j) into graph
            }
        }
    }

    // Graph is built, use maximum cardinality matching to find the number of pairings
    // If it's smaller than n/2, then f is optimal, else f+1 is a new optimum and f no longer is

}

int main() {
    int t; std::cin >> t;
    while(t--) testcase();
    return 0;
}
