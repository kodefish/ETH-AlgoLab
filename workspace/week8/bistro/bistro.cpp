#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/squared_distance_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;

void testcase(int num_existing) {
  std::vector<K::Point_2> pts;
  pts.reserve(num_existing);
  for(int i = 0; i < num_existing; i++) {
    int x, y; std::cin >> x >> y;
    pts.push_back(K::Point_2(x, y));
  }

  Triangulation t;
  t.insert(pts.begin(), pts.end());

  int num_locations; std::cin >> num_locations;
  for(int i = 0; i < num_locations; i++) {
    K::Point_2 pt, nearest; std::cin >> pt;
    nearest = t.nearest_vertex(pt)->point();
    std::cout << std::setprecision(24) << CGAL::squared_distance(nearest, pt) << std::endl;
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int num_existing; std::cin >> num_existing;
  while(num_existing != 0) {
    testcase(num_existing);
    std::cin >> num_existing;
  }
}
