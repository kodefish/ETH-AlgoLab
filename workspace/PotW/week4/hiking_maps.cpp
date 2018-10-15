#include <iostream>
#include <vector>
#include <climits>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
//#include <CGAL/Triangle_2.h>
//#include <CGAL/Segment_2.h>
//#include <CGAL/Point_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Triangle_2    Triangle;
typedef K::Segment_2    Segment;
typedef K::Point_2      Point;
typedef K::Line_2       Line;
typedef std::vector<Point>      pt_vec;
typedef std::vector<Segment>    seg_vec;
typedef std::vector<Triangle>   tri_vec;

void testcase(){
    std::cout << "testcase" << std::endl;
    int m, n; std::cin >> m >> n;

    std::cout << "read input" << std::endl;

    seg_vec paths(m-1);
    int p_x, p_y; std::cin >> p_x >> p_y;
    Point p_start(p_x, p_y);
    for(int i = 0; i < m-1; i++) {
        std::cin >> p_x >> p_y;
        Point p_end(p_x, p_y);
        paths[i] = Segment(p_start, p_end);
        p_start = p_end;
    }

    tri_vec map_parts(n);
    for(int i = 0; i < n; ++i){
        std::vector<Line> lines(3);
        for (int j = 0; j < 3; ++j) {
            int x1, y1, x2, y2; std::cin >> x1 >> y1 >> x2 >> y2;
            lines[j] = Line(Point(x1, y1), Point(x2, y2));
        }

        pt_vec tri_pts(3);
        for (int j = 0; j < 3; j++) {
            Line l1 = lines[j];
            Line l2 = lines[(j + 1) % 3];
            if (CGAL::do_intersect(l1, l2)) {
                auto o = CGAL::intersection(l1, l2);
                if (const Point* op = boost::get<Point>(&*o)) {
                    tri_pts[j] = *op;
                }
            }
        }
        map_parts[i] = Triangle(tri_pts[0], tri_pts[1], tri_pts[2]);
    }

    std::vector<std::vector<int> > path_to_tri(m-1, std::vector<int>(0));
    int min_tri = INT_MAX;
    int max_tri = INT_MIN;
    for(int i = 0; i < m-1; i++) {
        // Find all triangles that contain the path i
        for(int j = 0; j < n; j++) {
            std::cout << paths[i] << " - " << map_parts[j] << std::endl;
            if (CGAL::do_intersect(paths[i], map_parts[j])) {
                auto o = CGAL::intersection(paths[i], map_parts[j]);
                if (const Segment* op = boost::get<Segment>(&*o)){
                    std::cout << "\t" << *op << " - " << paths[i];
                    if (*op == paths[i]){
                        std::cout << " pushing back " << j;
                        path_to_tri[i].push_back(j);
                    }
                    std::cout << std::endl;
                }
            }
        }
    }


    for(int i = 0; i < m-1; i++) {
        std::cout << i << ": ";
        for(int j = 0; j < path_to_tri[i].size(); j++) {
            std::cout << path_to_tri[i][j] << " ";
        }
        std::cout << std::endl;
    }

}

int main(){
    int t; std::cin >> t;
    while(t--) testcase();
    return 0;
}
