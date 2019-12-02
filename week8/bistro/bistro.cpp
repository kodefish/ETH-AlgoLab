#include <iostream>
#include <climits>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/squared_distance_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

void testcase(int n) {
    // Read points
    std::vector<K::Point_2> pts;
    pts.reserve(n);
    for (std::size_t i = 0; i < n; ++i) {
        int x, y;
        std::cin >> x >> y;
        pts.push_back(K::Point_2(x, y));
    }

    // construct triangulation
    Triangulation t;
    t.insert(pts.begin(), pts.end());

    // Read in possible locations
    int m; std::cin >> m;
    for (int i = 0; i < m; i++) {
        int x, y;
        std::cin >> x >> y;
        K::Point_2 possible_location(x, y);
        K::FT dist_to_nearest = CGAL::squared_distance(possible_location, t.nearest_vertex(possible_location)->point());
        std::cout << std::setprecision(0) << std::fixed << dist_to_nearest << std::endl;
    }

}

int main() {
    std::ios_base::sync_with_stdio(false);
    int n; std::cin >> n;
    while (n > 0) {
        testcase(n);
        std::cin >> n;
    }
    return 0;
}
