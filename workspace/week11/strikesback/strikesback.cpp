#include <iostream>
#include <cassert>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

// Delaunay
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Finite_faces_iterator  Face_iterator;

// choose exact integral type
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void testcase() {
    // Read input
    int A, S, B, E; std::cin >> A >> S >> B >> E;

    std::vector<std::pair<K::Point_2, int>> asteroids(A);
    for (int i = 0; i < A; i++) {
        K::Point_2 loc; int d; std::cin >> loc >> d;
        asteroids[i] = std::make_pair(loc, d);
    }

    std::vector<std::pair<K::Point_2, double>> shoot(S);
    for (int i = 0; i < S; i++) {
        K::Point_2 loc; std::cin >> loc;
        shoot[i] = std::make_pair(loc, 1048576.0);
    }

    std::vector<K::Point_2> bounty_hunters(B);
    for (int i = 0; i < B; i++) {
        std::cin >> bounty_hunters[i];
    }

    Program lp (CGAL::LARGER, true, 0, false, 0);

    // For every shooting point, find closest bounty hunter and save sqdist
    Triangulation t;
    t.insert(bounty_hunters.begin(), bounty_hunters.end());
    for (int s = 0; s < S; s++) {
        if (B > 0) {
            K::Point_2 s_i = shoot[s].first;
            K::Point_2 nearest_bounty = t.nearest_vertex(s_i)->point();
            shoot[s].second = CGAL::squared_distance(s_i, nearest_bounty);
        }
        // LP objective function
        lp.set_c(s, 1);
    }

    // For every asteroid i 
    //      b = d_i
    //      For every shooting point j
    //          If dist2(i, j) < r(j)
    //              add constraint e_j/dist2(i, j) 
    for (int i = 0; i < A; i++) {
        lp.set_b(i, asteroids[i].second);
        for (int j = 0; j < S; j++) {
            K::FT dist2 = CGAL::squared_distance(asteroids[i].first, shoot[j].first);
            if (dist2 < shoot[j].second) {
                lp.set_a(j, i, 1 / std::max(ET(1), ET(dist2)));
            }
        }
    }

    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert (s.solves_linear_program(lp));
    if (s.is_infeasible() || s.objective_value() > E) std::cout << "n" << std::endl;
    else std::cout << "y" << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}
