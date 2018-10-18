#include <iostream>
#include <vector>
#include <climits>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Triangle_2   Triangle;
typedef K::Segment_2    Segment;
typedef K::Point_2      Point;
typedef K::Line_2       Line;
typedef std::vector<Point>          pt_vec;
typedef std::pair<Point, Point>     pt_pair;
typedef std::vector<pt_pair>        pt_pair_vec;
typedef std::vector<Triangle>       tri_vec;

bool point_in_triangle(const Point &point, const pt_pair_vec triangle) {
    bool inside = true;
    for(int i = 0; i < triangle.size(); i++) {
        inside = inside && !(CGAL::right_turn(triangle[i].first, triangle[i].second, point));
    }
    return inside;
}

void testcase(){
    int m, n; std::cin >> m >> n;

    pt_pair_vec paths(m-1);
    int p_x, p_y; std::cin >> p_x >> p_y;
    Point p_start(p_x, p_y);
    for(int i = 0; i < m-1; i++) {
        std::cin >> p_x >> p_y;
        Point p_end(p_x, p_y);
        paths[i] = std::make_pair(p_start, p_end);
        p_start = p_end;
    }

    // If for a pair of points, both lie in the triangle, save i
    std::vector<std::vector<int> > path_to_tri(m-1, std::vector<int>(0));
    for(int triangle_idx = 0; triangle_idx < n; ++triangle_idx){
        // Read the 6 points that describe the lines that form a triangle
        pt_pair_vec lines(3);
        for (int j = 0; j < 3; ++j) {
            int x1, y1, x2, y2; std::cin >> x1 >> y1 >> x2 >> y2;
            lines[j] = std::make_pair(Point(x1, y1), Point(x2, y2));
        }

        // Order the points in pairs s.t. (p, p'), (q, q'), (r, r') performs a counter-clockwise
        // walk around the triangle formed by the lines
        // So if p, p', q form a right turn, swap p and p'
        // If q, q', r form a right turn, swap q and q'
        // If r, r', p form a right turn, swap r and r'
        for(int j = 0; j < 3; ++j) {
            pt_pair l1 = lines[j];
            pt_pair l2 = lines[(j + 1) % 3];
            if (CGAL::right_turn(l1.first, l1.second, l2.first)) {
                lines[j] = std::make_pair(l1.second, l1.first);
            } else if (CGAL::right_turn(l1.first, l1.second, l2.second)) {
                lines[j] = std::make_pair(l1.second, l1.first);
            }
        }

        // Compute which paths are fully covered by the triangle
        for(int path_idx = 0; path_idx < m-1; ++path_idx) {
            bool source_in = point_in_triangle(paths[path_idx].first, lines);
            bool target_in = point_in_triangle(paths[path_idx].second, lines);
            if(source_in && target_in) {
                path_to_tri[path_idx].push_back(triangle_idx);
            }
        }
    }

    // Sliding window
    std::vector<int> index(m-1, 0);
    bool end_not_reached = true;
    int min_tri_cost = INT_MAX;
    while(end_not_reached) {
        int min_tri = INT_MAX, max_tri = INT_MIN;
        int min_path_idx = INT_MAX;
        for (int i = 0; i < m - 1; i++) {
            int i_path_tri = path_to_tri[i][index[i]];
            if (i_path_tri < min_tri) {
                min_tri = i_path_tri;
                min_path_idx = i; 
            }
            if (max_tri < i_path_tri) {
                max_tri = i_path_tri;
            }
        }

        int curr_cost = max_tri - min_tri + 1;
        if (curr_cost < min_tri_cost) {
            min_tri_cost = curr_cost;
        }

        if (++index[min_path_idx] == path_to_tri[min_path_idx].size()) {
            end_not_reached = false;
        }
    }

    std::cout << min_tri_cost << std::endl;

}

int main(){
    int t; std::cin >> t;
    while(t--) testcase();
    return 0;
}
