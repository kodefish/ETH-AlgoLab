#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef K::Point_2 P;

// choose input type (input coefficients must fit)
typedef long IT;
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

void testcase() {
    int n, m, c; std::cin >> n >> m >> c;

    std::vector<P> t_loc; t_loc.reserve(n + m);
    std::vector<P> w_loc; w_loc.reserve(n);
    std::vector<int> w_supply; w_supply.reserve(n);
    std::vector<int> w_percentage; w_percentage.reserve(n);
    for (int i = 0; i < n; i++) {
        int x, y, s, a;
        std::cin >> x >> y >> s >> a;
        w_loc.push_back(P(x, y));
        w_supply.push_back(s);
        w_percentage.push_back(a);

        t_loc.push_back(P(x, y));
    }

    std::vector<P> s_loc; s_loc.reserve(m);
    std::vector<int> s_demand; s_demand.reserve(m);
    std::vector<int> s_upper; s_upper.reserve(m);
    for (int i = 0; i < m; i++) {
        int x, y, d, u;
        std::cin >> x >> y >> d >> u;
        s_loc.push_back(P(x, y));
        s_demand.push_back(d);
        s_upper.push_back(u);

        t_loc.push_back(P(x, y));
    }

    Triangulation t;
    t.insert(t_loc.begin(), t_loc.end());

    std::vector<std::vector<int>> revenue(n, std::vector<int>(m, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            int r; std::cin >> r;
            revenue[i][j] = r;
        }
    }

    std::vector<std::pair<P, K::FT>> contours;
    for (int i = 0; i < c; i++) {
        int x, y, r;
        std::cin >> x >> y >> r;

        // Check if contour line actually affects any points
        P center(x, y);
        K::FT r2 = K::FT(r) * K::FT(r);
        const auto nearest = t.nearest_vertex(center);
        if (CGAL::squared_distance(nearest->point(), center) <= r2) contours.push_back(std::make_pair(center, r2));
    }
 
    // Create an LP with Ax <= b, lower bound 0 and no upper bounds
    // The variales are x_ws -> amount (in liters) of beer sent from warehouse w to stadium s
    Program lp (CGAL::SMALLER, true, 0, false, 0);

    // Constraint that sum(x_ws) over s <= supply_w
    int c_count = 0;
    for (int w = 0; w < n; w++) {
        for (int s = 0; s < m; s++) {
            lp.set_a((w*n) + s, c_count, 1);
        }
        lp.set_b(c_count++, w_supply[w]);
    }

    // Constraint that sum(x_ws * w_percentage) over w <= upper_s
    for (int s = 0; s < m; s++) {
        for (int w = 0; w < n; w++) {
            lp.set_a((w*n) + s, c_count, w_percentage[w]);
        }
        lp.set_b(c_count++, s_upper[s] * 100);
    }

    // Constraint that sum(x_ws) over w = demand_s -> modelled by
    // sum(x_ws) over w <= demand_s
    // -sum(x_ws) over w <= -demand_s
    for (int s = 0; s < m; s++) {
        for (int w = 0; w < n; w++) {
            lp.set_a((w*n) + s, c_count, 1);
            lp.set_a((w*n) + s, c_count + 1, -1);
        }
        lp.set_b(c_count++, s_demand[s]);
        lp.set_b(c_count++, -s_demand[s]);
    }

    // Objective function -> maximise revenue
    for (int w = 0; w < n; w++) {
        for (int s = 0; s < m; s++) {
            int t = 0;
            for (auto c : contours) {
                bool in = false;
                if (CGAL::squared_distance(w_loc[w], c.first) <= c.second) in = true;
                if (CGAL::squared_distance(s_loc[s], c.first) <= c.second) in = !in;

                if (in) t++; // If one and only one point is in the circle 
            }

            lp.set_c((w*n) + s, -(100 * revenue[w][s] - t));
        }
    }

    Solution s = CGAL::solve_nonnegative_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));

    // output solution
    if (s.is_infeasible()) std::cout << "RIOT!" << std::endl;
    else std::cout << (long) floor_to_double(-s.objective_value() / 100) << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}

