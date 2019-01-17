#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;

int main()
{
    while (true) {
        std::ios_base::sync_with_stdio(false);
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
    
        // read in potential locations
        int m; std::cin >> m;
        for (int i = 0; i < m; i++) {
            K::Point_2 p; 
            std::cin >> p; 
            std::cout << std::setprecision(24) << CGAL::squared_distance(t.nearest_vertex(p)->point(), p) << std::endl;
        }
    }
}

