#include <iostream>
#include <vector>
#include <algorithm>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Line_2 Line;
typedef K::Point_2 Point;

struct BikerTrajectory {
    long source;
    K::FT slope;
    int index;
    
    //    Order in descending order of y0 (highest point first)
    bool operator<(const BikerTrajectory& t) const { return this->source < t.source;  }
};

void testcase() {
    // Read input
    long n; std::cin >> n;

    // Store index of bikers
    std::vector<BikerTrajectory> biker_trajectories;
    for (int i = 0; i < n; i++) {
        long y0, x1, y1; std::cin >> y0 >> x1 >> y1;
        Line l(Point(0, y0), Point(x1, y1));

        // Create ray
        biker_trajectories.push_back(BikerTrajectory{y0, -l.a()/l.b(), i});
    }

    // Sort bikers in descending order of y0
    std::sort(biker_trajectories.begin(), biker_trajectories.end());

    // Choose a random biker and follow him until we hit an intersection
    // Stay on the biker that got to the intersection first, delete the one that got there second
    // Keep track of who rides off, they all start with hearts full of hope
    std::vector<bool> biker_rides_off(n, true);
    int last_collision = 0;
    for (int i = 0; i < n; i++) {
        K::FT si = biker_trajectories[i].slope;
        for (int j = 0; j < i; j++) {
            if (biker_rides_off[biker_trajectories[j].index]) {
                K::FT sj = biker_trajectories[j].slope;
                biker_rides_off[biker_trajectories[j].index] = biker_rides_off[biker_trajectories[j].index] && si >= sj;
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

