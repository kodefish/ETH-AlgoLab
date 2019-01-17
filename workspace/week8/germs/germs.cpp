#include <iostream>
#include <vector>
#include <algorithm>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_data_structure_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt       EK;
typedef CGAL::Exact_predicates_inexact_constructions_kernel     K;
typedef CGAL::Triangulation_vertex_base_with_info_2<K::FT, K>   VB;
typedef CGAL::Triangulation_face_base_2<K>                      FB;
typedef CGAL::Triangulation_data_structure_2<VB, FB>            TDS;
typedef CGAL::Delaunay_triangulation_2<K, TDS>                  DT;

double ceil_to_double(const EK::FT &a) {
    double x = std::ceil(CGAL::to_double(a));
    while (x < a) x++;
    while (x - 1 >= a) x--;
    return x;
}

EK::FT sq_rad_to_t(EK::FT r2) {
    return CGAL::sqrt((CGAL::sqrt(r2) - 1) / 2);
}

int main()
{
    while (true) {
        // read number of points
        int n; std::cin >> n;
        if (n == 0) return 0;

        // Read boundaries (y tho?)
        int left, bottom, right, top;
        std::cin >> left >> bottom >> right >> top;

        // read points
        std::vector<K::Point_2> pts(n);
        for (int i = 0; i < n; ++i) {
            std::cin >> pts[i];
        }

        // construct triangulation
        DT t;
        t.insert(pts.begin(), pts.end());
 
        // Compute min squared distance to boundary 
        for (auto v = t.finite_vertices_begin(); v != t.finite_vertices_end(); v++) {
            K::Point_2 p = v->point();
            K::FT min_h = std::min(p.x() - left, right - p.x());
            K::FT min_v = std::min(p.y() - bottom, top - p.y());
            K::FT min = std::min(min_h, min_v);
            v->info() = 4 * min * min;
        }

        // Compute min squared distance to nearest neighbor
        for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); e++) {
            auto v_i = e->first->vertex(t.cw(e->second));
            auto v_j = e->first->vertex(t.ccw(e->second));
            K::FT dist = CGAL::squared_distance(v_i->point(), v_j->point());

            v_i->info() = std::min(v_i->info(), dist);
            v_j->info() = std::min(v_j->info(), dist);
        }

        // Create array of min squared distances for all vertices
        std::vector<K::FT> min_squared_dist;
        for (auto v = t.finite_vertices_begin(); v != t.finite_vertices_end(); v++) 
            min_squared_dist.push_back(v->info());

        // Sort the squared distances
        std::sort(min_squared_dist.begin(), min_squared_dist.end());

        EK::FT f = sq_rad_to_t(EK::FT(min_squared_dist[0]));
        EK::FT m = sq_rad_to_t(EK::FT(min_squared_dist[n / 2]));
        EK::FT l = sq_rad_to_t(EK::FT(min_squared_dist[n - 1]));

        //std::cout << std::setprecision(0)
        std::cout 
                << ceil_to_double(f) << " " 
                << ceil_to_double(m) << " " 
                << ceil_to_double(l) << std::endl;

    }
}

