#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <algorithm>
#include <chrono>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>      Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor           Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor             Edge;

using namespace std::chrono;

void testcase() {

    // Read inputs (num_students, num_characteristics, and Dr. Fuzzman's solution
    int num_s, num_c, f; std::cin >> num_s >> num_c >> f;

    int hobby_hash = 0;
    std::map<std::string, int> hashed_hobbies;

    // Store student characteristics
    Graph possible_buddies(num_s);
    std::vector<std::set<int>> student_cs(num_s);
    for(int i = 0; i < num_s; i++) {
        std::set<int> c;
        for(int j = 0; j < num_c; j++) {
            std::string hobby; std::cin >> hobby;
            if(hashed_hobbies.find(hobby) == hashed_hobbies.end()){
                hashed_hobbies[hobby] = hobby_hash++;
            }
            c.insert(hashed_hobbies[hobby]);
        }

        student_cs[i] = c;

        for(int j = 0; j < i; j++) {
            std::set<int> intersect;
            std::set_intersection(student_cs[i].begin(), student_cs[i].end(),
                student_cs[j].begin(), student_cs[j].end(),
                std::inserter(intersect, intersect.begin()));
            if (intersect.size() > f) {
                // Insert edge (i, j) into graph
                Edge e; bool s;
                std::tie(e, s) = boost::add_edge(i, j, possible_buddies);
            }
        }
    }

    // Graph is built, use maximum cardinality matching to find the number of pairings
    // If it's smaller than n/2, then f is optimal, else f+1 is a new optimum and f no longer is
    std::vector<Vertex> mate_map(num_s);
    boost::edmonds_maximum_cardinality_matching(possible_buddies, 
        boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, possible_buddies)));

    int matchingsize = matching_size(possible_buddies, boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, possible_buddies)));

    if (2 * matchingsize < num_s) {
        // Not all vertices got matched -> f is optimal solution
        std::cout << "optimal" << std::endl;        
    } else {
        // All students got matched -> f is not optimal, f + 1 is better
        std::cout << "not optimal" << std::endl;        
    }
    return;

}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t--) testcase();
    return 0;
}
