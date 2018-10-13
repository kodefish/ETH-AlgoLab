#include <iostream>
#include <vector>
#include <climits>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;
typedef K::Ray_2 R;

double floor_to_double(const K::FT& x) {
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return a;
}

void replace_min_intersection(boost::optional<P> &curr_min_inter, const P &maybe, const P &origin) {
    // Compute the distance to the intersection
    // If it's smaller than the current min dist, then reset the new min_pt
    if (curr_min_inter) {
        if (CGAL::has_smaller_distance_to_point(origin, maybe, *curr_min_inter)){
            curr_min_inter = maybe;
        }
    } else {
        curr_min_inter = maybe;
    }
}

void testcase(int num_segments) {
    // Read source and point through which ray passes
    long x, y, a, b;
    std::cin >> x >> y >> a >> b;

    // Define the ray
    P origin(x, y);
    R ppRay(origin, P(a, b));

    // Read segments, test for intersection
    std::vector<S> obstacles;
    for (int i = 0; i < num_segments; ++i) {
        std::cin >> x >> y >> a >> b;
        obstacles.push_back(S(P(x, y), P(a, b)));
    }

    // Because why not?
    std::random_shuffle(obstacles.begin(), obstacles.end());
    boost::optional<P> min_inter;
    for(auto obstacle: obstacles) {
        // If the ray intersects the obstacle
        if (CGAL::do_intersect(ppRay, obstacle)) {
            auto o = CGAL::intersection(ppRay, obstacle);
            if (const P* op = boost::get<P>(&*o)){
                replace_min_intersection(min_inter, *op, origin);
            } else if (const S* op = boost::get<S>(&*o)){
                replace_min_intersection(min_inter, op->source(), origin);
                replace_min_intersection(min_inter, op->target(), origin);
            }
        }
    }
    
    // Print result
    if (!min_inter) {
        std::cout << "no" << std::endl;
    }  else {
        P& p = min_inter.get();
       std::cout << floor_to_double(p.x()) << " " << floor_to_double(p.y()) << std::endl; 
    }
    return;

}

int main() {
    std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
    std::ios_base::sync_with_stdio(false);
    int n; std::cin >> n;
    while (n != 0) {
        testcase(n);
        std::cin >> n;
    }
    return 0;
}
