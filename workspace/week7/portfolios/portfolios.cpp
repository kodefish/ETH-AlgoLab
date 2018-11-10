#include <iostream>

#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

// Exact rational type
typedef CGAL::Gmpq ET;

// Program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void testcase(int num_assets, int num_investors) {

  // Non-negative QP with Ax >= p
  Program qp(CGAL::LARGER, true, 0, false, 0);
  const int COST = 0;
  const int RETURN = 1;

  // Override defaults
  qp.set_r(COST, CGAL::SMALLER); // cost of portfolio <= capital of investor
  qp.set_r(RETURN, CGAL::LARGER); // return of portfolio >= min return for investor

  // Read in cost c_i and expected return r_i of each asset 
  for (int i = 0; i < num_assets; i++) {
    int c_i, r_i; std::cin >> c_i >> r_i;
    qp.set_a(i, COST, c_i);
    qp.set_a(i, RETURN, r_i);
  }

  // Read in covariance matrix D - works, because D is positive semi definite
  for (int i = 0; i < num_assets; i++) {
    for (int j = 0; j < num_assets; j++) {
      int v_i_j; std::cin >> v_i_j;
      qp.set_d(i, j, 2 * v_i_j);
    }
  }

  // Find portfolio for each investor
  for (int i = 0; i < num_investors; i++) {
    int C, R, V; std::cin >> C >> R >> V;

    // Set rhs of constraint equations
    qp.set_b(COST, C);
    qp.set_b(RETURN, R);

    // Solve
    Solution s = CGAL::solve_nonnegative_quadratic_program(qp, ET());
    if (s.is_optimal()) {
      ET variance = s.objective_value().numerator() / s.objective_value().denominator();
      if (variance <= V) {
	std::cout << "Yes." << std::endl;
      } else {
	std::cout << "No." << std::endl;
      }
    } else {
      std::cout << "No." << std::endl;
    }
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int n, m; std::cin >> n >> m;
  while (n != 0 && m != 0) {
    testcase(n, m);
    std::cin >> n >> m;
  }
  return 0;
}
