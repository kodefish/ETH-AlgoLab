#include <iostream>
#include <cmath>
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
        Point P[n];
        for(int i = 0; i < n; ++i) {
            double x, y;
            std::cin >> x >> y;
            P[i] = Point(x, y);
        }

        Min_circle mc1(P, P+n, true);
        std::cout << ceil(sqrt(mc1.circle().squared_radius())) << std::endl;

        // Read next number of points (next testcase);
        std::cin >> n;
    }
}
