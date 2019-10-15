#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 Point;
typedef K::Segment_2 Segment;
typedef K::Ray_2 Ray;


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
        if (biker_rides_off[i]) {
            for (int j = i + 1; j < n; j++) {
                if (biker_rides_off[j]) {
                    if (CGAL::do_intersect(biker_trajectories[i], biker_trajectories[j])) {
                        auto o = CGAL::intersection(biker_trajectories[i], biker_trajectories[j]);
                        if (const Point* op = boost::get<Point>(&*o)) {
                            // Compute distance to each biker's origin, closest one got here first so we keep him
                            if (CGAL::has_smaller_distance_to_point(*op, biker_trajectories[i].source(), biker_trajectories[j].source())) {
                                // Biker j comes after -> stops
                                biker_rides_off[j] = biker_rides_off[j] && false;
                            } else {
                                // Biker i comes after -> stops
                                biker_rides_off[i] = biker_rides_off[i] && false;
                            }
                        } else throw std::runtime_error("strange segment intersection");
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
