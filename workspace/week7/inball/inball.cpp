#include <iostream>
#include <vector>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int main() {
    int i = 0; 
    while (true) {
        int n, d; std::cin >> n >> d;
        if (n == 0) break;

        Program lp(CGAL::SMALLER, false, 0, false, 0);

        // Variables go from 0 to d.
        // x0 ... x_d-1 represent a point
        // x_d is the squared radius of the circle, which we want to maximize
        for(int i = 0; i < n; i++) {
            int norm2 = 0;
            for(int j = 0; j < d; j++) {
                int a_i_j; std::cin >> a_i_j;
                lp.set_a(j, i, a_i_j);
                norm2 += a_i_j * a_i_j;
            }

            int norm = std::floor(std::sqrt(norm2)); // compute norm of a_i
            assert(norm * norm == norm2);
            lp.set_a(d, i, norm);

            int b_i; std::cin >> b_i;
            lp.set_b(i, b_i);
        }
        
        // r^2 >= 0;
        lp.set_l(d, true, 0);

        lp.set_c(d, -1); // Maximize r^2

        Solution s = CGAL::solve_linear_program(lp, ET());
        assert(s.solves_linear_program());
        if (s.is_infeasible())
            std::cout << "none" << std::endl;
        else if (s.is_unbounded())
            std::cout << "inf" << std::endl;
        else
            std::cout << -(s.objective_value().numerator() / s.objective_value().denominator()) << std::endl;
    }
}
