#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <iostream>
#include <stdexcept>

typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;
typedef CGAL::Exact_predicates_exact_constructions_kernel   EK;

int main()
{
  IK::Point_2 p(2,1), q(1,0), r(-1,-1);
  // do something that needs predicates only, e.g., ...
  std::cout << (CGAL::left_turn(p, q, r) ? "y" : "n") << "\n";
 
  // now we use non-trivial constructions...
  EK::Point_2 ep(p.x(), p.y()), eq(q.x(), q.y()), er(r.x(), r.y());
  EK::Circle_2 c(ep, eq, er);
  if (!c.has_on_boundary(ep))
    throw std::runtime_error("ep not on c");
}
