// example: decide whether there exists a disk that covers a given set
// of points R in the plane and is disjoint from another set of points B
#include <iostream>
#include <cassert>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

// choose exact integral type
#include <CGAL/Gmpz.h>
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int main() {
  // by default, we have an LP with Ax <= b and no bounds for 
  // the four variables alpha, beta, gamma, delta
  Program lp (CGAL::SMALLER, false, 0, false, 0); 
  const int alpha = 0;
  const int beta  = 1;
  const int gamma = 2;
  const int delta = 3;
  
  // number of red and blue points
  int m; std::cin >> m;
  int n; std::cin >> n;

  // read the red points (cancer cells)
  for (int i=0; i<m; ++i) {
    int x; std::cin >> x;
    int y; std::cin >> y;
    // set up <= constraint for point inside/on circle: 
    //   -alpha x - beta y - gamma <= -x^2 - y^2
    lp.set_a (alpha, i, -x); 
    lp.set_a (beta,  i, -y);
    lp.set_a (gamma, i, -1);
    lp.set_b (       i, -x*x - y*y);
  }
  // read the blue points (healthy cells)
  for (int j=0; j<n; ++j) {
    int x; std::cin >> x;
    int y; std::cin >> y;
    // set up <= constraint for point outside circle: 
    //   alpha x + beta y + gamma + delta <= x^2 + y^2
    lp.set_a (alpha, m+j, x); 
    lp.set_a (beta,  m+j, y);
    lp.set_a (gamma, m+j, 1);
    lp.set_a (delta, m+j, 1);
    lp.set_b (       m+j, x*x + y*y);
  }

  // objective function: -delta (the solver minimizes)
  lp.set_c(delta, -1);

  // enforce a bounded problem: 
  lp.set_u (delta, true, 1);

  // solve the program, using ET as the exact type
  Solution s = CGAL::solve_linear_program(lp, ET());
  assert (s.solves_linear_program(lp));

  // output exposure center and radius, if they exist
  if (s.is_optimal() && (s.objective_value() < 0)) {
    // *opt := alpha, *(opt+1) := beta, *(opt+2) := gamma
    CGAL::Quadratic_program_solution<ET>::Variable_value_iterator 
      opt = s.variable_values_begin(); 
    CGAL::Quotient<ET> alpha = *opt;
    CGAL::Quotient<ET> beta = *(opt+1);
    CGAL::Quotient<ET> gamma = *(opt+2);
    std::cout << "There is a valid exposure:\n";
    std::cout << " Center = ("        // (alpha/2, beta/2)
	      << alpha/2 << ", " <<  beta/2 
	      << ")\n"; 
    std::cout << " Squared Radius = " // gamma + alpha^2/4 + beta^2/4
	      << gamma + alpha*alpha/4 + beta*beta/4 << "\n";
  } else
    std::cout << "There is no valid exposure.";
  std::cout << "\n";
  return 0;
}
