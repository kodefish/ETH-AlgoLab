#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K>                  VB;
typedef CGAL::Triangulation_face_base_with_info_2<int, K>     FB;
typedef CGAL::Triangulation_data_structure_2<VB, FB>        TDS;
typedef CGAL::Delaunay_triangulation_2<K, TDS>              DT;
typedef DT::Edge_iterator                                   Edge_iterator;
typedef DT::All_faces_iterator                              Face_iterator;

bool escape(DT::Face_handle start, const DT &t, K::FT d, int iter) {
    // Mark faced as visited
    start->info() = iter;


    // Check if managed to escape
    if (t.is_infinite(start)) 
        return true;

    // Check if we can escape through one of our 3 neighbors
    for (int i = 0; i < 3; i++) {
        K::Segment_2 seg = t.segment(start, i);    
        // Check to see if we can visit the neighboring face without getting infected
        if (seg.squared_length() >= 4 * d) {
            DT::Face_handle neighbor = start->neighbor(i);
            // If neighbor hasn't been visited yet, see if we can escape
            if (neighbor->info() < iter && escape(neighbor, t, d, iter))
                return true;
        }
    }

    return false;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    while(true) {
        // read number of points
        int n;
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
        DT t;
        t.insert(pts.begin(), pts.end());

        // face info starts with 0 for every face (track if face has been visited for current 
        // query
        for (Face_iterator f = t.all_faces_begin(); f != t.all_faces_end(); f++) {
            f->info() = 0;
        }

        // process every query
        int m; std::cin >> m;
        for (int i = 1; i <= m; ++i) {
            K::Point_2 c; K::FT d;
            std::cin >> c >> d;

            // Check if not already infected
            K::Point_2 nearest = t.nearest_vertex(c)->point();
            if (CGAL::squared_distance(c, nearest) < d) {
                std::cout << "n";
            } else {
                DT::Face_handle f = t.locate(c);
                if (escape(f, t, d, i)) 
                    std::cout << "y";
                else
                    std::cout << "n";
            }
        }
        std::cout << std::endl;
    }
}
