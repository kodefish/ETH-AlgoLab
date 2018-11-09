#include <iostream>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

// Choose exact integral type
typedef CGAL::Gmpq ET;

// Program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef CGAL::Quotient<ET> SolT;

double ceil_to_double (const SolT &x) {
    double a = std::ceil(CGAL::to_double(x));
    while (a < x) a++;
    while (a - 1 >= x) a--;
    return a;
}

int main() {
  int p; std::cin >> p;
  while(p != 0) {

    int a, b; std::cin >> a >> b;
    const int X = 0;
    const int Y = 1;
    const int Z = 2; // Rememeber to take sqrt of Z!!!
    Program qp;
    int mult = 1;

    if (p == 1) {
      // First case
      qp = Program(CGAL::SMALLER, true, 0, false, 0); // x, y >= 0
      qp.set_a(X, 0, 1); qp.set_a(Y, 0, 1); qp.set_b(0, 4); // x + y <= 4
      qp.set_a(X, 1, 4); qp.set_a(Y, 1, 2); qp.set_b(1, a * b); // 4x + 2y <= ab
      qp.set_a(X, 2, -1); qp.set_a(Y, 2, 1); qp.set_b(2, 1); // -x + y <= 1
      qp.set_c(Y, -b); qp.set_d(X, X, 2 * a); // min(-by + ax^2) - DON'T FORGET TO MULTIPLY BY -1 AFTER!

      mult = -1;
    } else {
      // Second case
      qp = Program(CGAL::LARGER, false, 0, false, 0); // x, y >= 0
      qp.set_a(X, 0, 1); qp.set_a(Y, 0, 1); qp.set_b(0, -4); // x + y >= -4
      qp.set_a(X, 1, 4); qp.set_a(Y, 1, 2); qp.set_a(Z, 1, 1); qp.set_b(1, - (a * b)); // 4x + 2y + z^2 >= -ab
      qp.set_a(X, 2, -1); qp.set_a(Y, 2, 1); qp.set_b(2, -1); // -x + y >= -1
      qp.set_d(X, X, 2 * a); qp.set_c(Y, b); qp.set_d(Z, Z, 2); // min(ax^2 + by + z^4)

      qp.set_u(X, true, 0); // x <= 0
      qp.set_u(Y, true, 0); // y <= 0
    }

    Solution s = CGAL::solve_quadratic_program(qp, ET());
    assert(s.solves_quadratic_program(qp));
    if (!s.is_optimal()) {
	std::cout << "no" << std::endl;
    } else if (s.is_unbounded()) {
	std::cout << "unbounded" << std::endl;
    } else {
      std::cout << std::setprecision(12) << mult * ceil_to_double(s.objective_value()) << std::endl;
    }

    std::cin >> p;

  }
  return 0;
}
