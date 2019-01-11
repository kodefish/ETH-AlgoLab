#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <climits>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;

void testcase() {
    int m, n; std::cin >> m >> n;

    // Read in points
    std::vector<P> path_points(m);
    for(int i = 0; i < m; i++) {
        int x, y; std::cin >> x >> y;
        path_points[i] = P(x, y);
    }

    // Construct hiking path
    std::vector<S> path(m);
    for (int i = 0; i < m - 1; i++) {
        path[i] = S(path_points[i], path_points[i + 1]);
    }

    // Read in map parts - for each find all the path legs it covers
    std::map<int, std::vector<int>> map_coverage;
    for (int i = 0; i < n; i++) {
        // Read in the map delimiting lines
        std::vector<std::pair<P, P>> points(3);
        for(int tri_pt = 0; tri_pt < 3; tri_pt++) {
            int x1, y1, x2, y2; std::cin >> x1 >> y1 >> x2 >> y2;
            points[tri_pt] = std::make_pair(P(x1, y1), P(x2, y2));
        }

        // Make sure the order of the points is okay, correct if needed.
        for(int tri_del = 0; tri_del < 3; tri_del++) {
            P p1, p2, p3; 
            p1 = points[tri_del].first;
            p2 = points[tri_del].second;
            p3 = points[(tri_del + 1) % 3].first;
            // Swap p1 and p2 if p1 p2 p3 don't form a left turn
            if (CGAL::right_turn(p1, p2, p3)) {
                points[tri_del].first = p2; 
                points[tri_del].second = p1; 
            }
        }

        // Compute which legs are contained by this map part
        for(int p = 0; p < m - 1; p++) {
            bool covered = true;
            // Check if path p is covered by map part i
            for(int tri_del = 0; tri_del < 3; tri_del++) {
                P p1, p2; 
                p1 = points[tri_del].first;
                p2 = points[tri_del].second;
                covered &= !(CGAL::right_turn(p1, p2, path[p].source()) ||
                      CGAL::right_turn(p1, p2, path[p].target()));
            }
            if (covered) {
               map_coverage[i].push_back(p);
            }
        }
    }

    // Taken and adapted from master solution
    std::vector<int> covered(m - 1, 0);
    std::size_t uncovered = m - 1;
    std::size_t best = n;
    for (std::size_t tb = 0, te = 0; tb != map_coverage.size();) {
        // Get coverage by incrementing te
        for (; uncovered > 0 && te != map_coverage.size(); te++) {
            for (int j = 0; j < map_coverage[te].size(); j++) {
                if (++covered[map_coverage[te][j]] == 1) --uncovered;
            } 
        } 
        if (uncovered != 0) break;

        do {
            for (int j = 0; j < map_coverage[tb].size(); j++) {
                if (--covered[map_coverage[tb][j]] == 0) ++uncovered;
            }
        } while (++tb != te && uncovered == 0);
        best = std::min(best, te-tb+1);
    }
    
    std::cout << best << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t--) testcase();
    return 0;
}
