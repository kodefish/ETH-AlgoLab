// CGAL includes
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>

// STL includes
#include <vector>
#include <iostream>

// Epic kernel is enough, no constructions needed, provided the squared distance
// fits into a double (!)
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// we want to store an index with each vertex
typedef CGAL::Triangulation_vertex_base_with_info_2<int ,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;

typedef std::vector<int> vec_i;
typedef std::vector<vec_i> vec_ii;

bool dfs(int start, const vec_ii &graph, std::vector<int> &coloring, bool color) {
    if (coloring[start] > -1) std::cerr << "yall go f yourself!" << std::endl;
    coloring[start] = color;
    bool diff = true;
    for (int i = 0; i < graph[start].size() && diff; i++) {
        int v = graph[start][i];
        if (coloring[v] < 0) { // if neighbor not visited, recurse
            diff = diff && dfs(v, graph, coloring, !color); // if DFS coloring ok of neighbor
        } else { // if neighbor already visited
            diff = diff && (coloring[start] != coloring[v]); // and coloring is ok with us
        }
    }
    return diff;
}

bool two_color(const vec_ii &graph) {
    // Color graph via dfs by coloring the neighbor of each vertex a different color than the vertex
    std::vector<int> coloring(graph.size(), -1);
    bool ok = true;
    for (int i = 0; i < graph.size(); i++) {
        if (coloring[i] < 0) {
            ok = ok && dfs(i, graph, coloring, 0);
        }
    }
    return ok;
}

void testcase() {
    int n, m, r;
    std::cin >> n >> m >> r;

    std::vector<std::pair<K::Point_2, int>> points;
    points.reserve(n);
    for (int i = 0; i < n; ++i) {
        int x, y;
        std::cin >> x >> y;
        points.emplace_back(K::Point_2(x, y), i);
    }

    Delaunay t;
    t.insert(points.begin(), points.end());

    // Build graph of network by adding edges of length <= r
    //  UF data structure to keep track of components
    vec_ii graph(n);
    boost::disjoint_sets_with_storage<> uf(n);
    for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
        if (t.segment(e).squared_length() <= r*r) {
            int i1 = e->first->vertex((e->second+1)%3)->info();
            int i2 = e->first->vertex((e->second+2)%3)->info();

            graph[i1].push_back(i2);
            graph[i2].push_back(i1);

            int c1 = uf.find_set(i1);
            int c2 = uf.find_set(i2);
            if (c1 != c2) {
                uf.link(c1, c2);
            }
        }
    }

    bool no_interference = two_color(graph);
    std::cerr << (no_interference ? "no interference" : "interference") << std::endl;

    for (int i = 0; i < m; i++) {
        K::Point_2 s, w; std::cin >> s >> w;
        if (!no_interference) {
            std::cout << "n"; // Interference network cannot be built
        } else if (CGAL::squared_distance(s, w) <= r*r) {
            std::cout << "y"; // Radios connect directly to each other
        } else {
            auto nearest_s = t.nearest_vertex(s);
            auto nearest_w = t.nearest_vertex(w);

            if (CGAL::squared_distance(nearest_s->point(), s) > r*r || CGAL::squared_distance(nearest_w->point(), w) > r*r) {
                std::cout << "n"; // One of the radios is out of range
            } else if (uf.find_set(nearest_s->info()) == uf.find_set(nearest_w->info())) {
                std::cout << "y"; // Radios connect to radio stations that can talk to each other (part of same component)
            } else {
                std::cout << "n"; // Radios connect to radio stations that are in different components
            }
        }
    }
    std::cout << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}
