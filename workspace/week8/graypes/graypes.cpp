#include <iostream>
#include <vector>
#include <climits>
#include <cmath>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/squared_distance_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;

void testcase(int num_apes) {
  // Read in points
  std::vector<K::Point_2> pts;
  for(int i = 0; i < num_apes; i++) {
    int x, y; std::cin >> x >> y;
    pts.push_back(K::Point_2(x, y));
  }

  

  // Construct triangulation
  Triangulation t;
  t.insert(pts.begin(), pts.end());

  double min_time = INT_MAX;
  for (Triangulation::Edge_iterator e_itr = t.finite_edges_begin(); e_itr != t.finite_edges_end(); e_itr++){
    Triangulation::Vertex_handle v1 = e_itr->first->vertex((e_itr->second + 1) % 3);
    Triangulation::Vertex_handle v2 = e_itr->first->vertex((e_itr->second + 2) % 3);

    double time = CGAL::squared_distance(v1->point(), v2->point());
    if (time < min_time) {
      min_time = time;
    }
  }

  // Divide time by 2 and convert to hundreth of a second -> time/(2 * 0.01) = time * 50
  std::cout << std::ceil(std::sqrt(min_time) * 50) << std::endl;
}

int main() {
  int n; std::cin >> n;
  while (n != 0) {
    testcase(n);
    std::cin >> n;
  }
  return 0;
}
