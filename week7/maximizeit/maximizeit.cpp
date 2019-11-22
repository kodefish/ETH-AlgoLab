// example: how to solve a simple explicit LP
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

double round_down(const CGAL::Quotient<ET> &s) {
    double a = std::floor(CGAL::to_double(s));
    while (a > s) a -= 1;
    while (a + 1 <= s) a += 1;
    return a;
}

void program_1() {

    IT a, b; std::cin >> a >> b;

    // create an LP with Ax <= b, lower bound 0 and no upper bounds
    Program lp (CGAL::SMALLER, true, 0, false, 0);

    // set the coefficients of A and b
    const int X = 0;
    const int Y = 1;
    lp.set_a(X, 0,  1); lp.set_a(Y, 0, 1); lp.set_b(0, 4);  //  x + y  <= 7
    lp.set_a(X, 1,  4); lp.set_a(Y, 1, 2); lp.set_b(1, a*b);  // -x + 2y <= 4
    lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, 1);  // -x + 2y <= 4

    // objective function
    lp.set_c(X,  a);                                       // ax
    lp.set_c(Y, -b);                                       // -bx

    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));

    if (s.is_infeasible()) {
        // check if solution exits
        std::cout << "no" << std::endl;
    } else if (s.is_unbounded()) {
        // check if solution is bounded
        std::cout << "unbounded" << std::endl;
    } else {
        // output solution, rounded up to next integer
        std::cout << round_down(-s.objective_value()) << std::endl;
    }
}

double round_up(const CGAL::Quotient<ET> &s) {
    double a = std::ceil(CGAL::to_double(s));
    while (a < s) a += 1;
    while (a - 1 >= s) a -= 1;
    return a;
}


void program_2() {

    IT a, b; std::cin >> a >> b;

    // create an LP with Ax >= b, no lower bound and upper bounds 0
    Program lp (CGAL::LARGER, false, 0, true, 0);

    // set the coefficients of A and b
    const int X = 0;
    const int Y = 1;
    const int Z = 2;
    lp.set_a(X, 0,  1); lp.set_a(Y, 0, 1); lp.set_a(Z, 0, 0); lp.set_b(0, -4);  //  x + y  >= -4
    lp.set_a(X, 1,  4); lp.set_a(Y, 1, 2); lp.set_a(Z, 1, 1); lp.set_b(1, -a*b);  //  4x + 2y + z >= -ab
    lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_a(Z, 2, 0); lp.set_b(2, -1);  //  -x + y >= -1

    // objective function ax + by + z
    lp.set_c(X, a);                                       // ax
    lp.set_c(Y, b);                                       // bx
    lp.set_c(Z, 1);                                       // z

    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));

    if (s.is_infeasible()) {
        // check if solution exits
        std::cout << "no" << std::endl;
    } else if (s.is_unbounded()) {
        // check if solution is bounded
        std::cout << "unbounded" << std::endl;
    } else {
        // output solution, rounded up to next integer
        std::cout << round_up(s.objective_value()) << std::endl;
    }
}

int main() {
    int p;
    do {
        std::cin >> p;
        switch (p) {
            case 1: program_1(); break;
            case 2: program_2(); break;
        }
    } while (p > 0);
    return 0;
}
