#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <iostream>
#include <stdexcept>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;

int main()
{
  P p[] = { P(0,0), P(2,0), P(1,0), P(3,0), P(.5,1), P(.5,-1) };
  S s[] = { S(p[0],p[1]), S(p[2],p[3]), S(p[4],p[5]) };
  for (int i = 0; i < 3; ++i)
    for (int j = i+1; j < 3; ++j)
      if (CGAL::do_intersect(s[i],s[j])) {
	auto o = CGAL::intersection(s[i],s[j]);
        if (const P* op = boost::get<P>(&*o))
	  std::cout << "point: " << *op << "\n";
        else if (const S* os = boost::get<S>(&*o))
	  std::cout << "segment: " << os->source() << " " 
                    << os->target() << "\n";
        else // how could this be? -> error
          throw std::runtime_error("strange segment intersection");
      } else
	std::cout << "no intersection\n";
}
