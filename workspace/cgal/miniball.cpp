#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <iostream>

// typedefs
typedef  CGAL::Exact_predicates_exact_constructions_kernel K;
typedef  CGAL::Min_circle_2_traits_2<K>  Traits;
typedef  CGAL::Min_circle_2<Traits>      Min_circle;

int main()
{
  const int n = 100;
  K::Point_2 P[n];
  
  for (int i = 0; i < n; ++i)
    P[i] = K::Point_2((i % 2 == 0 ? i : -i), 0);
  // (0,0), (-1,0), (2,0), (-3,0), ...
  
  Min_circle mc(P, P+n, true);
  Traits::Circle c = mc.circle();
  std::cout << c.center() << " " << c.squared_radius() << std::endl;
}
