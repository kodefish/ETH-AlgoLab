// STL includes
#include <iostream>
#include <vector>

// CGAL includes
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>  Triangulation;
typedef Triangulation::Vertex_iterator  Vertex_iterator;
typedef Triangulation::Vertex_handle Vertex_handle;

void testcase() {
    // read number of jammers and missions
    std::size_t m, n;
    std::cin >> m >> n;

    // read points
    std::vector<std::pair<K::Point_2, K::FT>> players;
    for (std::size_t i = 0; i < m; ++i) {
        int x, y; K::FT r;
        std::cin >> x >> y >> r;
        players.push_back(std::make_pair(K::Point_2(x, y), r));
    }

    // read in lamp_raidus (since we have a 45° angle, and tan 45° = 1, so it's the same as the height, yay!)
    K::FT lamp_radius; std::cin >> lamp_radius;

    // read in each lamp
    std::vector<std::pair<K::Point_2, int>> lamps;
    for (std::size_t i = 0; i < n; i++) {
      int x, y; std::cin >> x >> y;
      lamps.push_back(std::make_pair(K::Point_2(x, y), i));
    }

    // construct triangulation
    Triangulation t;
    t.insert(lamps.begin(), lamps.end());

    // check for each player if they get hit
    // keep track of which lamp eliminates the player i
    std::vector<int> eliminated_by(m, INT_MAX);
    for (std::size_t i = 0; i < m; i++) {
      Vertex_handle nearest_lamp = t.nearest_vertex(players[i].first);
      K::FT min_dist = (lamp_radius + players[i].second);

      K::FT dist2 = CGAL::squared_distance(nearest_lamp->point(), players[i].first);
      if (min_dist * min_dist > dist2) {
        eliminated_by[i] = nearest_lamp->info();
        // overlap! check if player got hit by a lamp before this one
        for (int l = 0; l < eliminated_by[i]; l++) {
          if (min_dist * min_dist > CGAL::squared_distance(lamps[l].first, players[i].first)) {
            eliminated_by[i] = l;
          }
        }
      }
    }

    // find players eliminated by largest lamp idx
    int max_lamp_idx = INT_MIN;
    for (std::size_t i = 0; i < m; i++) {
      max_lamp_idx = std::max(max_lamp_idx, eliminated_by[i]);
    }

    for (std::size_t i = 0; i < m; i++) {
      if (eliminated_by[i] == max_lamp_idx) std::cout << i << " ";
    }
    std::cout << std::endl;

}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}
