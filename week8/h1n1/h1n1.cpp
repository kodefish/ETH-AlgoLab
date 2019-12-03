#include <iostream>
#include <vector>
#include <stack>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> VB;
typedef CGAL::Triangulation_face_base_with_info_2<int, K> FB;
typedef CGAL::Triangulation_data_structure_2<VB, FB> TDS;
typedef CGAL::Delaunay_triangulation_2<K, TDS>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Face_handle Face_handle;
typedef Triangulation::All_faces_iterator Face_itr;
typedef K::Segment_2 Segment_2;
typedef K::Ray_2 Ray_2;

void dfs(const Triangulation &t, Face_handle &v, K::FT d, int iter) {
    // Iterative DFS starting from v
    // There is an edge in the graph if the length of the edge is longer
    // than d^2
    std::stack<Face_handle> stack;
    stack.push(v);
    while (!stack.empty()) {
        v = stack.top(); stack.pop();
        // If face is not visited
        if (v->info() < iter) {
            // Set face to visited
            v->info() = iter;// Check if actally an exit

            if (t.is_infinite(v)) {
                // If it's a ray, that means it's a way out
                // So we print y and return
                std::cout << "y";
                return;
            }

            // For every edge of the face
            for (int e = 0; e < 3; e++) {

                // If not an exit
                // Get the edge
                Segment_2 seg = t.segment(std::make_pair(v, e));
                // If distance is great enough
                if (seg.squared_length() >= 4 * d) {
                    // Push corresponding neighbor to the stack to check as well
                    stack.push(v->neighbor(e));
                }
            }
        }
    }

    // No exit was found
    std::cout << "n";
    return;
}

void testcase(int n){
    // Read points
    std::vector<K::Point_2> pts;
    for (std::size_t i = 0; i < n; ++i) {
        K::FT x, y; std::cin >> x >> y;
        pts.push_back(K::Point_2(x, y));
    }
    // Construct triangulation
    Triangulation t;
    t.insert(pts.begin(), pts.end());

    // Set all face information
    for (Face_itr face_itr = t.all_faces_begin(); face_itr != t.all_faces_end(); face_itr++) {
        face_itr->info() = -1;
    }

    // Process the queries
    int m; std::cin >> m;
    for (int i = 0; i < m; i++) {
        // Read query
        K::FT x, y, d; std::cin >> x >> y >> d;
        K::Point_2 start(x, y);
        if (CGAL::squared_distance(start, t.nearest_vertex(start)->point()) < d) {
            std::cout << "n";
        } else {
            Face_handle v = t.locate(start);
            dfs(t, v, d, i);
        }

    }
    std::cout << std::endl;
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
