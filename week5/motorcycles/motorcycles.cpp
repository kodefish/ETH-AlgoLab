#include <iostream>
#include <vector>
#include <algorithm>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Line_2 Line;
typedef K::Point_2 Point;
typedef K::Ray_2 Ray;

struct BikerTrajectory {
    long source;
    K::FT slope;
    int index;
    Ray trajectory;

    //    Order in descending order of y0 (highest point first)
    bool operator<(const BikerTrajectory& t) const { return this->source < t.source;  }
};

bool sort_by_index(const BikerTrajectory& t1, const BikerTrajectory& t2) { return t1.index < t2.index;  }

void testcase() {
    // Read input
    long n; std::cin >> n;

    // Store index of bikers
    std::vector<BikerTrajectory> biker_trajectories;
    for (int i = 0; i < n; i++) {
        long y0, x1, y1; std::cin >> y0 >> x1 >> y1;
        Line l(Point(0, y0), Point(x1, y1));

        // Create ray
        biker_trajectories.push_back(BikerTrajectory{y0, -l.a()/l.b(), i, Ray(Point(0, y0), Point(x1, y1))});
    }

    // Sort bikers in ascending order of y0
    std::sort(biker_trajectories.begin(), biker_trajectories.end());

    // Start at bottom, and work our way up by checking for intersections with bikers beneath us
    // Keep track of bikers that are riding in a stack (the higher the source, the further up in the stack)
    std::vector<BikerTrajectory> biker_rides_off;
    biker_rides_off.push_back(biker_trajectories[0]);
    int i = 1;
    while (i < n) {
        BikerTrajectory bi = biker_trajectories[i];
        bool keeps_riding = true;
        while(!biker_rides_off.empty() && keeps_riding) {
            BikerTrajectory bj = biker_rides_off.back();
            if (CGAL::do_intersect(bi.trajectory, bj.trajectory)) {
                if (bi.slope + bj.slope > 0) {
                    // Biker i has smaller slope than biker j -> biker i is closer to straight line and therefore was
                    // at the intersection first
                    biker_rides_off.pop_back();
                } else {
                    // If slopes are equal -> biker i is coming from the top therefore he yields
                    // If slope of biker i is larger, then he got to the intersectoin second and stops
                    keeps_riding = false;
                }
            } else {
                break;
            }
        }
        if (keeps_riding) {
            biker_rides_off.push_back(bi);
        }
        i++;
    }

    // Sort riding bikers by index
    std::sort(biker_rides_off.begin(), biker_rides_off.end(), sort_by_index);

    for (auto biker = biker_rides_off.begin(); biker != biker_rides_off.end(); biker++) {
        std::cout << biker->index << " ";
    }
    std::cout << std::endl;
}

int main() {
    std::ios::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}

