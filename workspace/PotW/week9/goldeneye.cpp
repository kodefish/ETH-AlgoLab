#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <vector>
#include <tuple>
#include <boost/pending/disjoint_sets.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> TDS;
typedef CGAL::Delaunay_triangulation_2<K, TDS> Triangulation;
typedef Triangulation::Finite_edges_iterator Edge_itr;
typedef boost::disjoint_sets_with_storage<> UnionFind;

void testcase() {
  int n, m; double p; std::cin >> n >> m >> p;
  // Read in jammers
  std::vector<std::pair<K::Point_2, int>> pts;
  pts.reserve(n);
  for (int i = 0; i < n; i++) {
    K::Point_2 p; std::cin >> p;
    pts.push_back(std::make_pair(p, i));
  }
  // Construct triangulation
  Triangulation t;
  t.insert(pts.begin(), pts.end());

  // Read over all edges of triangulation, if it's squared length < p
  // Union the two vertices
  UnionFind uf(n);
  std::vector<std::tuple<int, int, K::FT>> edges;
  for(Edge_itr ei = t.finite_edges_begin(); ei != t.finite_edges_end(); ++ei) {
    int v0 = ei->first->vertex((ei->second + 1) % 3)->info();
    int v1 = ei->first->vertex((ei->second + 2) % 3)->info();
    K::FT squared_l = t.segment(ei).squared_length();
    // Union edges that are reachable under p-umbrella
    if (squared_l <= p) {
      uf.union_set(v0, v1);
    }
    // Save edge with power consumpiton
    edges.push_back(std::make_tuple(v0, v1, squared_l));
  }

  // Sort edges by power consumption
  std::sort(edges.begin(), edges.end(), [](std::tuple<int, int, K::FT> a, std::tuple<int, int, K::FT> b){
					  return std::get<2>(a) < std::get<2>(b);
  });

  // For every mission
  K::FT a = 0, b = 0; 
  UnionFind uf_a(n);
  UnionFind uf_b(n);
  std::vector<std::tuple<int, int, K::FT>>::iterator a_itr = edges.begin();
  std::vector<std::tuple<int, int, K::FT>>::iterator b_itr = edges.begin();
  for(int i = 0; i < m; i++) {
    K::Point_2 p_s, p_t; std::cin >> p_s >> p_t;
    Triangulation::Vertex_handle near_s = t.nearest_vertex(p_s);
    Triangulation::Vertex_handle near_t = t.nearest_vertex(p_t);
    K::FT max_dist = std::max(CGAL::squared_distance(p_s, near_s->point()),
			      CGAL::squared_distance(p_t, near_t->point()));
    //std::cout << "mission " << i << ", max dist: " << max_dist << std::endl;
    if (4*max_dist <= p && uf.find_set(near_s->info()) == uf.find_set(near_t->info())) {
      // Mission possible!
      std::cout << "y";
      if (b < max_dist * 4)
	b = max_dist * 4;
      // Add edges as necessary to reach t from s
      for(; b_itr != edges.end()
	    && uf_b.find_set(near_s->info()) != uf_b.find_set(near_t->info());
	  b_itr++) {
	uf_b.union_set(std::get<0>(*b_itr), std::get<1>(*b_itr));
      }
    } else {
      // Mission impossible
      std::cout << "n";
    }

    // Get minimal power consumption for all missions
    if (a < max_dist * 4)
      a = max_dist * 4;
    // Add edges until t can be safely reached from t
    for(; a_itr != edges.end()
	&& uf_a.find_set(near_s->info()) != uf_a.find_set(near_t->info());
	a_itr++) {
      uf_a.union_set(std::get<0>(*a_itr), std::get<1>(*a_itr));
    }
  }
  std::cout << std::endl;
  // Get the last (and since it's sorted the largest) squared distance -> minimal power consumption
  if (a_itr != edges.begin() && std::get<2>(*(a_itr - 1)) > a) {
    a = std::get<2>(*(a_itr - 1));
  }
  if (b_itr != edges.begin() && std::get<2>(*(b_itr - 1)) > b) {
    b = std::get<2>(*(b_itr - 1));
  }
  std::cout << a << std::endl;
  std::cout << b << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  int t; std::cin >> t;
  while(t--) testcase();
  return 0;
}
