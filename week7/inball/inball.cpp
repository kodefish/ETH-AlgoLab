// STL
#include <cmath>

// CGAL LP
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

double floor_to_double(const CGAL::Quotient<ET>& x) {
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return a;
}

void testcase(int n, int d) {
    // create an LP with Ax <= b, lower bound 0 and no upper bounds
    Program lp (CGAL::SMALLER, false, 0, false, 0);

    // Read in cave constraints
    // 2 constraints per inequality
    // Guarantee that sphere fits into the cave 2*i: -aTx + b*norm(a) <= -b
    for(int i = 0; i < n; i++) {
        IT norm = 0;
        for (int j = 0; j < d; j++) {
            // Set coefficient in constraint i (representing the cave)
            IT a; std::cin >> a;
            lp.set_a(j, i, a);

            // add coeff^2
            norm += a * a;
        }

        // Compute norm of a
        int norm_sqrt = std::sqrt(norm);
        // Set constraint on radius
        lp.set_a(d, i, norm_sqrt);

        // Set rhs of constraint
        IT b; std::cin >> b;
        lp.set_b(i, b);
    }
    // Radius cannot be negative
    lp.set_l(d, true, 0);

    // objective function -> maximise r -> minimize -r
    lp.set_c(d, -1);


    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));

    // Check if solution is feasible or infinite before printing the objective value if it exists
    if (s.is_infeasible()) std::cout << "none" << std::endl;
    else if (s.is_unbounded()) std::cout << "inf" << std::endl;
    else std::cout << floor_to_double(-s.objective_value()) << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int n, d; std::cin >> n >> d;
    while (n > 0) {
        testcase(n, d);
        std::cin >> n >> d;
    }

    return 0;
}
