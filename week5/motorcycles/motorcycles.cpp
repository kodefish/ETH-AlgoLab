#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;
typedef K::Segment_2 Segment;
typedef K::Ray_2 Ray;
typedef K::Direction_2 Direction;
typedef K::Line_2 Line;


void testcase() {
    // Read input
    long n; std::cin >> n;

    std::vector<Ray> biker_trajectories;
    for (int i = 0; i < n; i++) {
        long y0, x1, y1; std::cin >> y0 >> x1 >> y1;

        // Create ray
        biker_trajectories.push_back(Ray(Point(0, y0), Point(x1, y1)));
    }

    // Slow solution (n2) check every biker against every other biker
    std::vector<bool> biker_rides_off(n, true);
    for (int i = 0; i < n; i++) {
        if (true || biker_rides_off[i]) {
            for (int j = i + 1; j < n; j++) {
                if (true || biker_rides_off[j]) {
                    if (CGAL::do_intersect(biker_trajectories[i], biker_trajectories[j])) {
                        // Smallest angle is the one that is closest
                        Line li, lj;
                        li = biker_trajectories[i].supporting_line();
                        lj = biker_trajectories[j].supporting_line();

                        // Compute slopes
                        K::FT slope_i = -li.a()/li.b();
                        K::FT slope_j = -lj.a()/lj.b();

                        // Take absolute value (cuz no idea how to work with K::FT)
                        K::FT abs_slope_i = (slope_i < 0) ? -slope_i:slope_i;
                        K::FT abs_slope_j = (slope_j < 0) ? -slope_j:slope_j;

                        if (abs_slope_i == abs_slope_j) {
                            // Slopes are same but opporsite -> rider on the right has priority (i.e. the one with the positive slope)
                            if (slope_i > 0) biker_rides_off[j] = biker_rides_off[j] && false;
                            else biker_rides_off[i] = biker_rides_off[i] && false;
                        } else if (abs_slope_i < abs_slope_j) {
                            biker_rides_off[j] = biker_rides_off[j] && false;
                        } else if (abs_slope_j < abs_slope_i) {
                            biker_rides_off[i] = biker_rides_off[i] && false;
                        }
                    }
                }
            }
        }
    }

    for (int i = 0; i < n; i++) {
        if (biker_rides_off[i]) std::cout << i << " ";
    }
    std::cout << std::endl;
}

int main() {
    std::ios::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}
