// STL includes
#include <iostream>
#include <vector>

//  CGAL includes
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

// Input and output types
typedef double IT;
typedef CGAL::Gmpq ET;

// Program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef std::pair<IT, IT> position;

double floor_to_double(const CGAL::Quotient<ET> &x) {
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

void testcase() {
    // Read in number of free and occupied nails
    IT n, m;
    std::cin >> n >> m;

    // Read in standard size of poster
    IT h, w;
    std::cin >> h >> w;

    // Read in free positions
    std::vector<position> free_positions;
    for (int i = 0; i < n; i++) {
        IT x, y; std::cin >> x >> y;
        free_positions.push_back(std::make_pair(x, y));
    }

    // Read in occupied positions
    std::vector<position> occupied_positions;
    for (int i = 0; i < m; i++) {
        IT x, y; std::cin >> x >> y;
        occupied_positions.push_back(std::make_pair(x, y));
    }

    // create an LP with Ax <= b, lower bound 1 (scaling factor cannot shrink poster)
    // and no upper bounds
    Program lp (CGAL::SMALLER, true, 1, false, 0);

    // Constraint counter
    int c_num = 0;
    // Set constraints for each new poster
    for (int i = 0; i < n; i++) {
        // Set constraints w.r.t new posters
        for (int j = i + 1; j < n; j++) {

            // Compute horizontal and vertical distances between posters
            IT horizontal_dist = std::abs(free_positions[j].first - free_positions[i].first);
            IT vertical_dist = std::abs(free_positions[j].second - free_positions[i].second);

            // Set constraint depending on the direction of biggest "variance" between the two positions
            if (horizontal_dist / w > vertical_dist / h) {
                // Set horizontal constraint: si * w/2 + sj * w/2 <= |xj - xi|
                // <==> si * w + sj * w <= 2 * |xj - xi|
                lp.set_a(i, c_num, w); lp.set_a(j, c_num, w);
                lp.set_b(c_num++, 2 * horizontal_dist);
            } else {
                // Set vertical constraint: si * h/2 + sj * h/2 <= |yj - yi|
                // <==> si * h + sj * h <= 2*|yj - yi|
                lp.set_a(i, c_num, h); lp.set_a(j, c_num, h);
                lp.set_b(c_num++, 2 * vertical_dist);
            }
        }

        // If there are old posters, setup constraints for them
        if (m > 0) {
            // Set constraints w.r.t closest old poster (same constraints, just with a scaling factor of 1)
            // First, find closest existing poster
            IT min_horizontal_dist = INT_MAX;
            IT min_vertical_dist = INT_MAX;
            for (int j = 0; j < m; j++) {
                // Compute horizontal and vertical distances between posters
                IT horizontal_dist = 2 * std::abs(occupied_positions[j].first - free_positions[i].first) - w;
                IT vertical_dist = 2 * std::abs(occupied_positions[j].second - free_positions[i].second) - h;

                if (horizontal_dist / w > vertical_dist / h) {
                    min_horizontal_dist = std::min(min_horizontal_dist, horizontal_dist);
                } else {
                    min_vertical_dist = std::min(min_vertical_dist, vertical_dist);
                }

            }

            // Set constraint for closest existing nail
            if (min_horizontal_dist / w < min_vertical_dist / h) {
                // Set horizontal constraint:
                // si * w/2 + 1 * w/2 <= |xj - xi|
                // <==> si * w/2 <= |xj - xi| - w/2
                // <==> si * w <= 2*|xj - xi| - w
                lp.set_a(i, c_num, w);
                lp.set_b(c_num++, min_horizontal_dist);
            } else {
                // Set vertical constraint:
                // si * h/2 + 1 * h/2 <= |yj - yi|
                // <==> si * h/2 <= |yj - yi| - h/2
                // <==> si * h <= 2*|yj - yi| - h
                lp.set_a(i, c_num, h);
                lp.set_b(c_num++, min_vertical_dist);
            }
        }

        // Set objective function: Sum(si*2*(h + w))
        // Negated since we want to maximize the perimiter
        lp.set_c(i, -2*(h + w));
    }

    // Solve the program, using ET as the exact type
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));

    // Output solution
    std::cout << std::setprecision(0) << std::fixed << -floor_to_double(s.objective_value()) << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}
