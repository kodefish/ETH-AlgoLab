#include <iostream>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

typedef CGAL::Gmpq ET;

typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

double ceil_to_double (const CGAL::Quotient<ET> &x) {
    double a = std::ceil(CGAL::to_double(x));
    while (a < x) a++;
    while (a - 1 >= x) a--;
    return a;
}

int main() {
    while (true) {
        int p, a, b; std::cin >> p >> a >> b;
        if (p == 0) return 0;

        // Construct the problem, depending on p
        Program lp;
        int neg = 1;
        if (p == 1) {
            lp = Program(CGAL::SMALLER, true, 0, false, 0);

            // Variables x and y
            const int X = 0; 
            const int Y = 1; 

            // Setup constraints
            // -x - y <= 0
            lp.set_a(X, 0, 1); lp.set_a(Y, 0, 1); lp.set_b(0, 4); 
            // 4x + 2y <= ab
            lp.set_a(X, 1, 4); lp.set_a(Y, 1, 2); lp.set_b(1, a*b); 
            // -x + y <= 1
            lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, 1); 
            
            // Setup objective function
            lp.set_d(X, X, 2*a); 
            lp.set_c(Y, -b);
            neg = -1; // used later because maximization
        } else if (p == 2) {
            lp = Program(CGAL::LARGER, false, 0, false, 0);

            // Variables
            const int X = 0; 
            const int Y = 1; 
            const int Z = 2; 

            // Constraints
            // x + y >= -4
            lp.set_a(X, 0, 1); lp.set_a(Y, 0, 1); lp.set_b(0, -4);
            // 4x + 2y + z^2 >= -ab
            lp.set_a(X, 1, 4); lp.set_a(Y, 1, 2); lp.set_a(Z, 1, 1); lp.set_b(1, -a*b);
            // -x + y >= -1
            lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, -1);
            // x, y <= 0
            lp.set_u(X, true, 0); lp.set_u(Y, true, 0);

            // Objective function
            lp.set_d(X, X, 2 * a);
            lp.set_d(Z, Z, 2);
            lp.set_c(Y, b);
        }

        // Print solution
        Solution s = CGAL::solve_quadratic_program(lp, ET());
        assert(s.solves_quadratic_program());
        if (!s.is_optimal()) {
            std::cout << "no" << std::endl; 
        } else if (s.is_unbounded()) {
            std::cout << "unbounded" << std::endl; 
        } else {
            std::cout << std::setprecision(12) << neg * ceil_to_double(s.objective_value()) << std::endl; 
        }

    }
}
