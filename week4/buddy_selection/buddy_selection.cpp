// STL includes
#include <iostream>
#include <vector>
#include <set>
#include <string>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;

void testcase() {
    // Read in number of students, number of characteristics per student and min number of common of characteristics per student
    int n, c, f; std::cin >> n >> c >> f;

    // Read in students characteristics
    std::vector<std::set<std::string>> students;
    for (int i = 0; i < n; i++) {
        students.push_back(std::set<std::string>());
        for (int j = 0; j < c; j++) {
            std::string characteristic; std::cin >> characteristic;
            students[i].insert(characteristic);
        }
    }

    // Create graph, 2 students are linked only if they share more than c characteristics (i.e. they should be buddies)
    graph g(n);
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            // Compute set intersection of 2 students (only one way and not with one-self)
            std::vector<std::string> intersect;
            std::set_intersection(students[i].begin(), students[i].end(), students[j].begin(), students[j].end(),
                                  std::back_inserter(intersect));
            if (intersect.size() > f) {
                boost::add_edge(i, j, g);
            }
        }
    }

    // Compute maximum cardinality matching (essentially try to match up maximum amount of students with more than
    // f corresponding caracteristics)
    std::vector<vertex_desc> mate_map(n);
    boost::edmonds_maximum_cardinality_matching(g,
                boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, g)));
    int matching_size = boost::matching_size(g,
                boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, g)));

    std::cout << (matching_size < n/2 ? "optimal":"not optimal") << std::endl;

}

int main() {
    std::ios::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}
