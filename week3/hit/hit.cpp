#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <iostream>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;
typedef K::Ray_2 R;

bool testcase() {
  // Num segments
  long n; std::cin >> n;

  // Check if done
  if (n == 0) return true;

  // Defenition of ray
  long x, y, a, b; std::cin >> x >> y >> a >> b;
  R ray(P(x, y), P(a, b));

  // Read segments
  bool intersect = false;
  for (int i = 0; i < n; i++) {
    long r, s, t, u; std::cin >> r >> s >> t >> u;
    S segment(P(r, s), P(t, u));

    if (!intersect) intersect = CGAL::do_intersect(ray, segment);
  }

  std::cout << (intersect?"yes":"no") << std::endl;
  return false;

}

int main() {
  std::ios::sync_with_stdio(false);
  bool testcase_done = false;
  while(!testcase_done) testcase_done = testcase();
  return 0;
}
