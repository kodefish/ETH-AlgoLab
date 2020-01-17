#include <iostream>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Vertex_handle Vertex_handle;
typedef K::Point_2 Point_2;

// choose input type (input coefficients must fit)
typedef double IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

double floor_to_double(const K::FT& x) {
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return a;
}

void testcase() {
    int a, s, b, e;
    std::cin >> a >> s >> b >> e;

    std::vector<Point_2> asteroids_loc;
    std::vector<IT> asteroids_density;
    for (int i = 0; i < a; i++) {
        Point_2 pos; IT d; std::cin >> pos >> d;
        asteroids_loc.push_back(pos);
        asteroids_density.push_back(d);
    }

    std::vector<Point_2> laser_targets;
    for (int i = 0; i < s; i++) {
        IT x, y; std::cin >> x >> y;
        laser_targets.push_back(Point_2(x, y));
    }

    std::vector<Point_2> bounty_hunters;
    for (int i = 0; i < b; i++) {
        IT x, y; std::cin >> x >> y;
        bounty_hunters.push_back(Point_2(x, y));
    }

    Triangulation t;
    t.insert(bounty_hunters.begin(), bounty_hunters.end());

    // create an LP with Ax >= b, lower bound 0 and upper bound e (since we have at most energy e)
    Program lp (CGAL::LARGER, true, 0, true, e);

    // One constraint per asteroid, one variable per laser shot, set lhs of constraint
    // Add e_i / dist2 if dist2 < upper_bound_laser_radius. The upper bound on the laser radius is set to be the
    // distance to the closest bounty hunter
    for (int i = 0; i < s; i++) {
        Vertex_handle nearest_bounty_hunter = t.nearest_vertex(laser_targets[i]);
        K::FT laser_radius;
        if (nearest_bounty_hunter == NULL) laser_radius = INT_MAX;
        else laser_radius = CGAL::squared_distance(nearest_bounty_hunter->point(), laser_targets[i]);

        for (int j = 0; j < a; j++) {
            Point_2 asteroid_loc = asteroids_loc[j];
            // Compute distance from laser shot to asteroid
            IT dist2 = floor_to_double(CGAL::squared_distance(asteroid_loc, laser_targets[i]));
            if (dist2 < laser_radius) lp.set_a(i, j, 1 / (dist2 > 1 ? dist2 : 1)); // e_i / max(1, dist2)
        }
    }

    // Set rhs of contraint
    for (int i = 0; i < a; i++) {
        lp.set_b(i, asteroids_density[i]); // >= density of asteroid
    }

    // Set objective function
    for (int i = 0; i < s; i++) {
        lp.set_c(i, 1); // sum of energy used for laser shots
    }

    // solve the program, using ET as the exact type
    Solution solution = CGAL::solve_linear_program(lp, ET());
    assert(solution.solves_linear_program(lp));

    // output solution
    if (solution.is_infeasible() || CGAL::to_double(solution.objective_value()) > e) std::cout << "n" << std::endl;
    else std::cout << "y" << std::endl;
}


int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}
