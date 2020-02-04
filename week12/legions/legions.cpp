#include <iostream>
#include <algorithm>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef long IT;
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void testcase() {
    IT x, y, n; std::cin >> x >> y >> n;

    // create an LP with Ax <= b, lower bound 0 and no upper bounds
    Program lp (CGAL::SMALLER, false, 0, false, 0);
    const int X = 0;
    const int Y = 1;
    const int T = 2;

    for (int i = 0; i < n; i++) {
        IT a, b, c, v; std::cin >> a >> b >> c >> v;

        IT norm = std::sqrt(a*a + b*b);
        IT sig = (x*a + y * b + c) >= 0 ? 1 : -1; // which side is the point on -> stay on that side
        lp.set_a(X, i, -a * sig);
        lp.set_a(Y, i, -b * sig);
        lp.set_a(T, i, norm * v);
        lp.set_b(i, c * sig); // ax + by + norm*v <= c
    }

    lp.set_c(T, -1);

    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));

    // output solution
    std::cout << (long)std::floor(CGAL::to_double(-s.objective_value())) << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}
