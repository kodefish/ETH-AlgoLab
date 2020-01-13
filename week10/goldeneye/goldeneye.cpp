// STL includes
#include <iostream>
#include <vector>

// CGAL includes
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

// BGL includes
#include <boost/pending/disjoint_sets.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Vertex_handle Vertex_handle;
typedef boost::disjoint_sets_with_storage<> UnionFind;


void testcase() {
    // read number of jammers and missions
    std::size_t n, m;
    std::cin >> n >> m;

    // read initial power consumption
    double p;
    std::cin >> p;

    // read points
    std::vector<std::pair<K::Point_2, int>> pts;
    pts.reserve(n);
    for (std::size_t i = 0; i < n; ++i) {
        int x, y;
        std::cin >> x >> y;
        pts.push_back(std::make_pair(K::Point_2(x, y), i));
    }

    // construct triangulation
    Triangulation t;
    t.insert(pts.begin(), pts.end());

    // Read over edges in triangulation. If the squared distance is
    // smaller than p, then union the two vertices
    UnionFind uf_n(n);
    std::vector<std::tuple<int, int, K::FT>> edges;
    for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
        int v1 = e->first->vertex((e->second + 1) % 3)->info();
        int v2 = e->first->vertex((e->second + 2) % 3)->info();
        K::FT squared_length = t.segment(e).squared_length();
        if (squared_length <= p) {
            uf_n.union_set(v1, v2);
        }
        edges.push_back(std::make_tuple(v1, v2, squared_length));
    }

    // Sort edges by their length
    std::sort(edges.begin(), edges.end(), [](std::tuple<int, int, K::FT>a , std::tuple<int, int, K::FT> b){
        return std::get<2>(a) < std::get<2>(b);
    });

    for (std::size_t i = 0; i < m; i++) {
        K::Point_2 start, end; std::cin >> start >> end;
        Vertex_handle u = t.nearest_vertex(start);
        Vertex_handle v = t.nearest_vertex(end);

        K::FT max_dist = std::max(
            CGAL::squared_distance(start, u->point()),
            CGAL::squared_distance(end, v->point())
        );

        if (4*max_dist <= p && uf_n.find_set(u->info()) == uf_n.find_set(v->info())) {
            // Mission possible!
            std::cout << "y";
        } else {
            std::cout << "n";
        }
    }
    std::cout << std::endl;
    std::cout << 4*p << std::endl << p << std::endl;

}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}
