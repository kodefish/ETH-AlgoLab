#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Finite_faces_iterator  Face_iterator;

int main()
{
  // read number of points
  std::size_t n;
  std::cin >> n;
  // construct triangulation
  Triangulation t;
  for (std::size_t i = 0; i < n; ++i) {
    Triangulation::Point p;
    std::cin >> p;
    t.insert(p);
  }
  // output all triangles
  for (Face_iterator f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f)
    std::cout << t.triangle(f) << "\n";
}
