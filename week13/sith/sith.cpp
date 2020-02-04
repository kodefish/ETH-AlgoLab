#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Triangulation;
typedef Triangulation::Vertex_iterator  Vertex_iterator;
typedef Triangulation::Vertex_handle  Vertex_handle;
typedef Triangulation::Edge_iterator  Edge_iterator;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS> graph;
typedef boost::graph_traits<graph>::edge_iterator                         edge_it;

void testcase() {
    // read number of points and operating range of vessel
    size_t n; cin >> n;
    K::FT r; cin >> r;
    K::FT r2 = r*r;

    // read points, store index as info
    vector<pair<K::Point_2, int>> pts;
    pts.reserve(n);
    for (std::size_t i = 0; i < n; ++i) {
        int x, y; cin >> x >> y;
        pts.emplace_back(make_pair(K::Point_2(x, y), i));
    }

    // Binary search for k such that there exists a subset of planets
    // of size at least k among non-conquered planets. A subset of planets
    // is a strongly connected component, as they are all reachable from one another
    // in at most k steps, and therefore we can conquer them all since we
    // decide where we start.
    size_t lower = 1, upper = n;
    while (lower <= upper) {
        size_t k = (lower + upper) / 2;

        // construct triangulation
        Triangulation t;
        t.insert(pts.begin() + k, pts.end());

        // Graph with all planets (triangulation will only contain edges of conquerable planets anyway,
        // the overhead is not that big here to have to remove the conquered planets from the graph)
        graph G(n);
        for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
                int i1 = e->first->vertex((e->second+1)%3)->info();
                int i2 = e->first->vertex((e->second+2)%3)->info();
                // if traversable edge, then add it to the graph;
                if (t.segment(e).squared_length() <= r2) {
                    boost::add_edge(i1, i2, G);
                    boost::add_edge(i2, i1, G);
                }
        }

        // Compute strongly connnected components. Each component is a possible subset of conquerable planets
        vector<int> scc_map(n);  // exterior property map// nscc: total number of SCCs
        int nscc = boost::strong_components(G, boost::make_iterator_property_map(scc_map.begin(), boost::get(boost::vertex_index, G)));
        vector<size_t> scc_size(nscc, 0);
        for (int i = 0; i < n; i++) {
            scc_size[scc_map[i]]++;
        }

        // Figure out max size of strongly conncected component, as this will be the max amount of planets
        // that can join the rebel alliance
        size_t max_size = 0;
        for (auto s : scc_size) max_size = s > max_size ? s : max_size;

        // If less than k planets join the Rebel Alliance, Palpatine must've conquered a vital planet
        // Otherwise, we can try to go one more day and see what happens.
        if (max_size < k) {
            upper = k - 1;
        } else {
            lower = k + 1;
        }
    }

    // lower represnts a lower bound of unsatisfiable k -> k - 1 is max satisfiable
    cout << lower - 1 << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) testcase();
    return 0;
}
