#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/basic.h>
#include <CGAL/squared_distance_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Point_set_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef K::Point_2 Point;
typedef K::Circle_2 Circle;

typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_data_structure_2<Vb> Tds;
typedef CGAL::Point_set_2<K, Tds> PSet;
typedef CGAL::Point_set_2<K, Tds>::Vertex_handle Vertex_handle;

void testcase() {
    int m, n;
    std::cin >> m >> n;

    std::vector<K::Point_2> players(m);
    std::vector<int> radius(m);
    for(int i = 0; i < m; i++) {
        std::cin >> players[i] >> radius[i];
    }

    // read in lamp information
    int h;
    std::cin >> h;

    std::vector<std::pair<K::Point_2, int> > lamps(n);
    for(int i = 0; i < n; i++) {
        K::Point_2 lamp;
        std::cin >> lamp;
        lamps[i] = std::make_pair(lamp, i);
    }

    PSet t(lamps.begin(), lamps.end());
    std::vector<int> first_hit(m, INT_MAX);
    int last_light = -1;

    for(int i = 0; i < m; i++) {
      K::Point_2 player = players[i];
      Vertex_handle nearest = t.nearest_neighbor(player);

      int dist_to_kill = pow(radius[i] + h, 2);

      if(dist_to_kill <= CGAL::squared_distance(nearest->point(), player)) {
        continue;
      }

      for(int lamp_idx = 0; lamp_idx < n; lamp_idx++) {
        if(dist_to_kill > CGAL::squared_distance(lamps[lamp_idx].first, player)) {
          first_hit[i]  = lamps[lamp_idx].second;
          last_light = std::max(last_light, lamps[lamp_idx].second);
          break;
        }
      }
    }

    std::vector<int> winners;
    for(int i = 0; i < m; i++) {
      if(first_hit[i] == INT_MAX) {
        winners.push_back(i);
      }
    }

    if(winners.size() <= 0) {
      for(int i = 0; i < m; i++) {
        if(first_hit[i] == last_light) {
          winners.push_back(i);
        }
      }
    }

    std::sort(winners.begin(), winners.end());
    for(int i = 0; i < winners.size(); i++) {
        std::cout << winners[i] << " ";
    }
    std::cout << std::endl;
}

int main() {
    int t; std::cin >> t;
    while(t--) testcase();
    return 0;
}
