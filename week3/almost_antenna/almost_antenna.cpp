#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <iostream>
#include <vector>
#include <climits>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;
typedef K::Point_2 P;

double ceil_to_double(const K::FT& x) {
    double a = std::ceil(CGAL::to_double(x));
    while (a < x) a += 1;
    while (a - 1 >= x) a -= 1;
    return a;
}

bool testcase() {
    long n; std::cin >> n;
    if (n == 0) return true;

    std::vector<P> homes(n);
    for (int i = 0; i < n; i++) {
        long x, y; std::cin >> x >> y;
        homes[i] = P(x, y);
    }

    Min_circle min_circle(homes.begin(), homes.end(), true);
    K::FT min_radius = min_circle.circle().squared_radius();

    for (auto support_point = min_circle.support_points_begin(); support_point != min_circle.support_points_end(); support_point++) {
        std::vector<P> homes_minus;
        for (auto home = homes.begin(); home != homes.end(); home++) {
            if (*home != *support_point) {
                homes_minus.push_back(*home);
            }
        }

        Min_circle min_circle_minus(homes_minus.begin(), homes_minus.end(), true);
        K::FT radius = min_circle_minus.circle().squared_radius();
        if (radius < min_radius) min_radius = radius;
    }

    std::cout << std::setprecision(0) << std::fixed
        << ceil_to_double(CGAL::sqrt(min_radius)) << std::endl;

    return false;
}

int main() {
    std::ios::sync_with_stdio(false);
    bool testcase_done = false;
    while(!testcase_done) testcase_done = testcase();
    return 0;
}
