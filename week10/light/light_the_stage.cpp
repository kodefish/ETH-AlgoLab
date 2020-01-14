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
    std::cerr << "new testcase, " << m << " players" << std::endl;

    // read points
    std::vector<std::pair<K::Point_2, int>> players;
    std::vector<K::FT> radii;
    players.reserve(m);
    for (std::size_t i = 0; i < m; ++i) {
        int x, y; K::FT r;
        std::cin >> x >> y >> r;
        players.push_back(std::make_pair(K::Point_2(x, y), i));
        radii.push_back(r);
    }

    // construct triangulation
    Triangulation t;
    t.insert(players.begin(), players.end());

    // read in lamp_raidus (since we have a 45° angle, and tan 45° = 1, so it's the same as the height, yay!)
    K::FT lamp_radius; std::cin >> lamp_radius;

    // read in each lamp
    std::vector<K::Point_2> lamps;
    for (std::size_t i = 0; i < n; i++) {
      int x, y; std::cin >> x >> y;
      lamps.push_back(K::Point_2(x, y));
    }

    // simulate the game basically
    std::vector<int> eliminated_players;
    for (std::size_t i = 0; i < n && t.number_of_vertices() > 0;) {
      Vertex_handle nearest_player = t.nearest_vertex(lamps[i]);
      std::cerr << "light: " << i << "\tnearest player " << nearest_player->info();
      K::FT min_dist = (lamp_radius + radii[nearest_player->info()]);
      std::cerr << "\tmindist2: " << min_dist*min_dist << ", sq dist: " << CGAL::squared_distance(lamps[i], nearest_player->point()) << std::endl;
      if (min_dist * min_dist > CGAL::squared_distance(lamps[i], nearest_player->point())) {
        // Player is eliminated
        eliminated_players.push_back(nearest_player->info());
        t.remove(nearest_player);
      } else {
        // No overlap with nearest player -> go to next lamp/round
        // If there are still players left, then players eliminated in this round are not winners
        std::cerr << "eliminated players in round " << i << ": ";
        for (std::size_t u = 0; u < eliminated_players.size(); u++) std::cerr << eliminated_players[u] << " ";
        std::cerr << std::endl;
        if (t.number_of_vertices() > 0) eliminated_players.clear();
        i++;
      }
    }

    // Print winners
    // Winners are players left in the game or if none are left it's the players eliminated in the last round
    std::vector<int> winners;
    if (t.number_of_vertices() > 0) {
      std::cerr << "players still left!" << std::endl;
      for (Vertex_iterator v = t.finite_vertices_begin(); v != t.finite_vertices_end(); v++) {
        winners.push_back(v->info());
      }
    } else {
      std::cerr << "winners died in last round" << std::endl;
      winners = eliminated_players;
    }

    std::sort(winners.begin(), winners.end());
    for (std::size_t i = 0; i < winners.size(); i++) std::cout << winners[i] << " ";
    std::cout << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
    int t; std::cin >> t;
    t =8;
    while (t--) testcase();
    return 0;
}
