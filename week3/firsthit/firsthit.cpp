#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <iostream>
#include <vector>
#include <algorithm>

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

P compute_intersection(R ray, S segment, P origin) {
    auto o = CGAL::intersection(ray, segment);
    if (const P* op = boost::get<P>(&*o)) {
        return *op;
    } else if (const S* os = boost::get<S>(&*o)) {
        return CGAL::has_smaller_distance_to_point(origin, os->source(), os->target()) ? os->source() : os->target();
    }else // how could this be? -> error
        throw std::runtime_error("strange segment intersection");
}

bool testcase() {
  // Num segments
  long n; std::cin >> n;

  // Check if done
  if (n == 0) return true;

  // Defenition of ray
  long x, y, a, b; std::cin >> x >> y >> a >> b;
  P origin(x, y);
  R ray(origin, P(a, b));

  std::vector<S> segments;
  for (int i = 0; i < n; i++) {
    long r, s, t, u; std::cin >> r >> s >> t >> u;
    segments.push_back(S(P(r, s), P(t, u)));
  }

  std::random_shuffle(segments.begin(), segments.end());

  // Read segments
  P min_intersection;
  S min_segment;
  bool init = false;

  for (int i = 0; i < n; i++) {
    S segment = segments[i];
    if (init) {
        if (CGAL::do_intersect(min_segment, segment)) {
            // Compute intersection
            P intersection = compute_intersection(ray, segment, origin);
            min_intersection = CGAL::has_smaller_distance_to_point(origin, intersection, min_intersection) ? intersection : min_intersection;
            min_segment = S(origin, min_intersection);
        }
    } else {
        if (CGAL::do_intersect(ray, segment)) {
            init = true;
            min_intersection = compute_intersection(ray, segment, origin);;
            min_segment = S(origin, min_intersection);
        }
    }

  }

  if (init) std::cout << std::fixed << std::setprecision(0)
                      << floor_to_double(min_intersection.x()) << " " << floor_to_double(min_intersection.y()) << std::endl;
  else std::cout << "no" << std::endl;

  return false;

}

int main() {
  std::ios::sync_with_stdio(true);
  bool testcase_done = false;
  while(!testcase_done) testcase_done = testcase();
  return 0;
}



