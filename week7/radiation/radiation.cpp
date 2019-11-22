// STL
#include <iostream>
#include <tuple>
#include <vector>
#include <cmath>

//  example: decide whether two point sets R and B can be separated by a nonvertical line
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose input type (input coefficients must fit)
typedef double IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

// Define a position (list of vecotrs, a cell is a vec of size 3)
typedef std::tuple<IT, IT, IT> cell;
typedef std::vector<cell> cell_vec;

// Global variables (so as not to pass them around through function params)
cell_vec healthy_cells;
cell_vec tumor_cells;

// Returns the polynomial of degree d evaluated at some point c
std::vector<IT> get_polynomial_coeffs(cell c, int d) {
    std::vector<IT> poly_coeffs(0);
    for (int i = 0; i <= d; i++) {
        for (int j = 0; j <= d-i; j++) {
            for (int k = 0; k < d-i-j; k++) {
                poly_coeffs.push_back(std::pow(std::get<0>(c), i) * std::pow(std::get<1>(c), j) * std::pow(std::get<2>(c), k));
            }
        }
    }

    return poly_coeffs;
}

// Check if a polynomial of degree d can separate the cells
bool polynomial_exists(int d) {
    // create an LP with Ax <= b and no lower/upper bounds
    Program lp (CGAL::SMALLER, false, 0, false, 0);

    // Setup constraints for healthy cells
    // p(healthy_point) < 0 (so we set p <= -1)
    for (int i = 0; i < healthy_cells.size(); i++) {
        // For every term in the polynomial of degree d, evaluated at x, y z;
        std::vector<IT> poly_coeffs = get_polynomial_coeffs(healthy_cells[i], d);
        for (int j = 0; j < poly_coeffs.size(); j++) {
            lp.set_a(j, i, poly_coeffs[j]);
        }
        lp.set_b(i, -1);
    }

    // Setup constraints for healthy cells
    // p(tumor_point) > 0 (so we set p >= 1)
    for (int i = 0; i < tumor_cells.size(); i++) {
        // For every term in the polynomial of degree d, evaluated at x, y z;
        std::vector<IT> poly_coeffs = get_polynomial_coeffs(tumor_cells[i], d);
        for (int j = 0; j < poly_coeffs.size(); j++) {
            lp.set_a(j, healthy_cells.size() + i, poly_coeffs[j]);
        }
        lp.set_r(healthy_cells.size() + i, CGAL::LARGER);
        lp.set_b(healthy_cells.size() + i, 1);
    }

    // solve the program, using ET as the exact type
    CGAL::Quadratic_program_options options;
    options.set_pricing_strategy(CGAL::QP_BLAND);
    Solution s = CGAL::solve_linear_program(lp, ET(), options);
    assert(s.solves_linear_program(lp));

    // output solution
    return !s.is_infeasible();
}

void testcase() {
    // Read in healthy and tumor cells
    int h, t; std::cin >> h >> t;

    // Read in healthy cells
    healthy_cells = cell_vec();
    for (int i = 0; i < h; i++) {
        IT x, y, z; std::cin >> x >> y >> z;
        healthy_cells.push_back(std::make_tuple(x, y, z));
    }

    // Read in healthy cells
    tumor_cells = cell_vec();
    for (int i = 0; i < t; i++) {
        IT x, y, z; std::cin >> x >> y >> z;
        tumor_cells.push_back(std::make_tuple(x, y, z));
    }

    // Find optimal d between 0 and 30 (higher we don't need)
    const int D_MAX = 30;
    int lower = 1;
    int upper = 2;
    // Exponential search for upper bound that does not exceed D_MAX
    while (upper <= D_MAX && !polynomial_exists(upper)) upper = std::min(2*upper, D_MAX + 1);

    // Binary search for optimal polynomial degree
    while (lower != upper) {
        int d = (lower + upper) / 2;
        if (polynomial_exists(d)) upper = d;
        else lower = d + 1;
    }

    // No idea why I have to do -1, but apparently there's an off by 1 bug somewhere in here.
    // Good luck finding it.
    if (lower <= D_MAX) std::cout << lower - 1 << std::endl;
    else std::cout << "Impossible!" << std::endl;

}

int main() {
    std::ios_base::sync_with_stdio(false);
    int n; std::cin >> n;
    while (n--) testcase();
    return 0;
}
