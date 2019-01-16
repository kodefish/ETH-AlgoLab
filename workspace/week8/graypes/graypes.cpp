#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

int main()
{
    while(true) {
        // read number of points
        std::size_t n;
        std::cin >> n;
        if (n == 0) break;

        // read points
        std::vector<K::Point_2> pts;
        pts.reserve(n);
        for (std::size_t i = 0; i < n; ++i) {
            K::Point_2 p;
            std::cin >> p;
            pts.push_back(p);
        }
        // construct triangulation
        Triangulation t;
        t.insert(pts.begin(), pts.end());
        // output all edges
        K::FT min_length = INT_MAX;
        for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e)
            min_length = std::min(min_length, t.segment(e).squared_length());
        
        std::cout << std::ceil(std::sqrt(min_length) * 50) << std::endl;
        }
  }
