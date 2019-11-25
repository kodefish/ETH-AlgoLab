#include <iostream>
#include <vector>
#include <climits>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

double floor_to_double(const K::FT& x) {
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return a;
}

void testcase(int n) {
    // Read graype positions
    std::vector<K::Point_2> pts(n);
    for (std::size_t i = 0; i < n; ++i) {
        std::cin >> pts[i];
    }

    // Construct triangulation
    Triangulation t;
    t.insert(pts.begin(), pts.end());

    // output all edges
    K::FT min_dist = INT_MAX;
    for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
        K::FT dist = t.segment(e).squared_length();
        min_dist = min_dist < dist ? min_dist : dist;
    }

    // Get distance fastet graype covers
    min_dist = std::sqrt(min_dist) / 2;

    // Convert to hundreth of a second
    K::FT time = min_dist * 100;

    // Quick tip: you can do ceil_to_double by just using floor to double and negating twice :thumbs_up:
    std::cout << -floor_to_double(-time) << std::endl;
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
