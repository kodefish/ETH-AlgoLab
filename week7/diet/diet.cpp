#include <iostream>

//  example: how to solve a simple explicit LP
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

double floor_to_double(const CGAL::Quotient<ET>& x) {
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

void testcase(int n, int m) {

    // create an LP with Ax <= b, lower bound 0 and no upper bounds
    Program lp (CGAL::SMALLER, true, 0, false, 0);

    // Read in nutrients
    std::vector<int> min_nutrient(n);
    std::vector<int> max_nutrient(n);
    for (int i = 0; i < n; i++) {
        IT min, max; std::cin >> min >> max;
        // Set nutrient constraint
        // Negate min as we want to be greater then min and lp is setup as SMALLER
        lp.set_b(2*i, -min);
        lp.set_b((2*i) + 1, max);
    }

    // Read in foods
    for (int i = 0; i < m; i++) {
        IT price; std::cin >> price;

        // Add food to objective function
        lp.set_c(i, price);

        for (int j = 0; j < n; j++) {
            IT nutrient; std::cin >> nutrient;

            // Add food's nutrient value to nutrient constraint
            // Negate nutrient for min as we want to be greater then min and lp is setup as SMALLER
            lp.set_a(i, 2*j, -nutrient);
            lp.set_a(i, (2*j) + 1, nutrient);
        }
    }

    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));

    // Check if there exists a diet
    if (s.is_infeasible()) std::cout << "No such diet." << std::endl;
    else std::cout << floor_to_double(s.objective_value()) << std::endl;

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
