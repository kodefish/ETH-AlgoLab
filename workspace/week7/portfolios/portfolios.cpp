#include <iostream>
#include <vector>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

typedef CGAL::Gmpq ET;

typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int main() {
    while (true) {
        int n, m; std::cin >> n >> m;
        if (n == 0 && m == 0) break;

        Program lp(CGAL::SMALLER, true, 0, false, 0);

        const int COST = 0;
        const int RETURN = 1;
        for (int i = 0; i < n; i++) {
            int c, r; std::cin >> c >> r; 
            lp.set_a(i, COST, c);
            lp.set_a(i, RETURN, -r);
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                int c_ij; std::cin >> c_ij; 
                if (j <= i)
                    lp.set_d(i, j, 2*c_ij);
            }
        }

        for(int i = 0; i < m; i++) {
            int C, R, V; std::cin >> C >> R >> V;
            
            lp.set_b(COST, C);
            lp.set_b(RETURN, -R);

            Solution s = CGAL::solve_quadratic_program(lp, ET());
            assert(s.solves_quadratic_program);

            if (s.is_optimal()) {
                std::cout << 
                        ((s.objective_value().numerator() /  s.objective_value().denominator()) <= V ? "Yes." : "No.")  << std::endl;
            } else {
                std::cout << "No." << std::endl; 
            }

        }
    }
}
