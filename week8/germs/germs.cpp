#include <iostream>
#include <vector>
#include <algorithm>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_data_structure_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<K::FT, K> VB;
typedef CGAL::Triangulation_face_base_2<K> FB;
typedef CGAL::Triangulation_data_structure_2<VB, FB> TDS;
typedef CGAL::Delaunay_triangulation_2<K, TDS>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

double floor_to_double(const K::FT& x) {
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return a;
}

K::FT sq_dist_to_t(K::FT sq_dist) {
     return CGAL::sqrt((CGAL::sqrt(sq_dist) - 1) / 2);
}

void testcase(int n) {

    // Read in boundaries and construct the lines
    K::FT l, b, r, t; std::cin >> l >> b >> r >> t;

    // Read points
    std::vector<K::Point_2> pts;
    for (std::size_t i = 0; i < n; ++i) {
        int x, y;
        std::cin >> x >> y;
        pts.push_back(K::Point_2(x, y));
    }

    // Construct triangulation
    Triangulation triang;
    triang.insert(pts.begin(), pts.end());

    // Compute squared distances from points to bounds of dish
    // In order to simulate the death by hitting the border, we imagine
    // there's another bacteria mirroring it from the other side
    // For that we must use min_dist*2 squared
    for (auto v = triang.finite_vertices_begin(); v != triang.finite_vertices_end(); v++) {
        K::Point_2 p = v->point();
        K::FT min_h = std::min(p.x() - l, r - p.x());
        K::FT min_v = std::min(p.y() - b, t - p.y());
        K::FT min = std::min(min_h, min_v);
        v->info() = 4*min*min; // (2min)^2
    }

    // Compute shortest ttl of each bacteira based on it's nearest neighbor
    for (auto e = triang.finite_edges_begin(); e != triang.finite_edges_end(); e++) {
        // Get the two vertices of the edge
        auto v_i = e->first->vertex(triang.cw(e->second));
        auto v_j = e->first->vertex(triang.ccw(e->second));

        // Compute lenght of the edge
        K::FT dist = CGAL::squared_distance(v_i->point(), v_j->point());

        // Update shortest ttl in vertice info if needed
        // i.e. bacteria dies before it hits the end of the dish
        v_i->info() = std::min(v_i->info(), dist);
        v_j->info() = std::min(v_j->info(), dist);
    }

    std::vector<K::FT> sq_dists;
    for (auto v = triang.finite_vertices_begin(); v != triang.finite_vertices_end(); v++) {
        sq_dists.push_back(v->info());
    }

    std::sort(sq_dists.begin(), sq_dists.end());

    K::FT first = sq_dist_to_t(sq_dists[0]);
    K::FT middle = sq_dist_to_t(sq_dists[n / 2]);
    K::FT last = sq_dist_to_t(sq_dists[n - 1]);

    std::cout
        << -floor_to_double(-first) << " "
        << -floor_to_double(-middle) << " "
        << -floor_to_double(-last) << std::endl;

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
