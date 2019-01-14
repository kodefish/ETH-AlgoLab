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

double floor_to_double (const CGAL::Quotient<ET> &x) {
    double a = std::floor(CGAL::to_double(x));
    /*
    while (a > x) a--;
    while (a - 1 <= x) a++;
    */
    return a;
}

int main() {
    while (true) {
        int n, m; std::cin >> n >> m;
        if (n == 0 && m == 0) break;

        Program lp(CGAL::LARGER, true, 0, false, 0);
        // min max of nutrient i
        for(int i = 0; i < n; i++) {
            int min_i, max_i; std::cin >> min_i >> max_i;
            lp.set_b(i, min_i);
            lp.set_b(i + n, -max_i);
        }

        // price and nutrient amounts for each food
        std::vector<std::pair<int, std::vector<int>>> foods(m);
        for(int i = 0; i < m; i++) {
            int p_i; std::cin >> p_i;
            lp.set_c(i, p_i); 

            for(int j = 0; j < n; j++) {
                int amount_i_j; std::cin >> amount_i_j;
                // min - invert to make < into >
                lp.set_a(i, j, amount_i_j);
                lp.set_a(i, j + n, -amount_i_j);
            }
        }

        Solution s = CGAL::solve_linear_program(lp, ET());
        assert(s.solves_linear_program());
        if (s.is_infeasible()) {
            std::cout << "No such diet." << std::endl; 
        } else {
            std::cout << floor_to_double(s.objective_value()) << std::endl; 
        }
    }
}
