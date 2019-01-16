#include <iostream>
#include <vector>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

typedef CGAL::Gmpq ET;
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

double ceil_to_double(CGAL::Quotient<ET> const &x) {
    double a = std::ceil(CGAL::to_double(x));
    while (a < x) a++;
    while (a - 1 >= x) a--;
    return a;
}

void testcase() {
    // Read in
    // n - num new posters
    // m - num old posters
    // h - default poster height
    // w - default poster width
    int n, m, h, w; std::cin >> n >> m >> h >> w;

    std::vector<std::pair<int, int>> new_posters(n);
    for(int i = 0; i < n; i++) {
        int x, y; std::cin >> x >> y;      
        new_posters[i] = std::make_pair(x, y);
    }

    std::vector<std::pair<int, int>> old_posters(m);
    for(int i = 0; i < m; i++) {
        int x, y; std::cin >> x >> y;      
        old_posters[i] = std::make_pair(x, y);
    }

    Program lp(CGAL::SMALLER, true, 1, false, 0);

    // Objective function
    // -2(w + h) sum(si)
    for (int i = 0; i < n; i++) {
        lp.set_c(i, -2*(w+h));
    }
    
    int constraint = 0;
    // Setup red-red constraints
    // si + sj <= 2*max(dx/w, dy/w)
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            ET dx = std::abs(new_posters[i].first - new_posters[j].first);
            ET dy = std::abs(new_posters[i].second - new_posters[j].second);
            int c = constraint++;
            lp.set_a(i, c, 1); lp.set_a(j, c, 1);
            lp.set_b(c, 2 * std::max(dx/w, dy/h));
        }
    }

    // Setup red-blue constraints
    // si <= 2*min(max(dx/2, dy/h)) - 1
    for (int i = 0; i < n; i++) {
        ET limit = 33554432;
        for (int j = 0; j < m; j++) {
            ET dx = std::abs(new_posters[i].first - old_posters[j].first);
            ET dy = std::abs(new_posters[i].second - old_posters[j].second);
            ET curr_limit = 2 * std::max(dx/w, dy/h) - 1; 
            limit = std::min(limit, curr_limit);
        }    
        int c = constraint++;
        lp.set_a(i, c, 1);
        lp.set_b(c, limit); 
    }

    Solution s = CGAL::solve_linear_program(lp, ET());
    std::cout << std::setprecision(0) << std::fixed 
        << ceil_to_double(-s.objective_value()) << std::endl;
}

int main() {
    std::ios::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t--) testcase();
    return 0;
}
