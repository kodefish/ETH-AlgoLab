#include <iostream>
#include <cmath>
#include <vector>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;
typedef K::Point_2 Point;

int main() {
     std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
    int n; std::cin >> n;
    while (n != 0) {
        // Read input points
        std::vector<Point> P(n);
        for(int i = 0; i < n; ++i) {
            double x, y;
            std::cin >> x >> y;
            P[i] = Point(x, y);
        }

        // Compute min bounding circle around all the points
        Min_circle mc1(P.begin(), P.end(), true);
        K::FT minRadius = mc1.circle().squared_radius();
        // Try removing every support point and see which result gives the smallest radius
        for(auto support_point = mc1.support_points_begin(); support_point < mc1.support_points_end(); ++support_point) {

            int i = 0; 
            while (i < n) {
                if (P[i] == *support_point) {
                    // "Delete" a support point
                    P[i] =  P[i + (i == 0 ? 1 : -1)];
                    Min_circle maybe(P.begin(), P.end(), true);
                    K::FT maybeRadius = maybe.circle().squared_radius();
                    if (minRadius > maybeRadius) {
                        minRadius = maybeRadius;
                    }
                    P[i] =  *support_point;
                    break;
                }
                i++;
            }
        }


        std::cout << ceil(sqrt(minRadius)) << std::endl;

        // Read next number of points (next testcase);
        std::cin >> n;
    }
}
