#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <climits>
#include <cassert>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
// CGAL includes
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
		boost::no_property,
		boost::property<boost::edge_weight_t, int>>	            Graph;
typedef boost::graph_traits<Graph>::edge_descriptor		        Edge;		
typedef boost::graph_traits<Graph>::vertex_descriptor		    Vertex;			
typedef boost::graph_traits<Graph>::vertex_iterator		        VertexIt;		
typedef boost::graph_traits<Graph>::out_edge_iterator		    OutEdgeIt;	
typedef CGAL::Exact_predicates_inexact_constructions_kernel 	K;
typedef CGAL::Delaunay_triangulation_2<K>  			            DT;

void testcase() {
    int N, M, R; std::cin >> N >> M >> R;

    std::vector<K::Point_2> stations(N);
    std::map<K::Point_2, Vertex> station_to_vertex;
    for (int i = 0; i < N; i++) {
        std::cin >> stations[i]; 
        station_to_vertex[stations[i]] = i;
    }

    Graph network(N);
    DT triangulation; triangulation.insert(stations.begin(), stations.end());
    for (DT::Edge_iterator e = triangulation.finite_edges_begin(); e != triangulation.finite_edges_end(); ++e) {
        K::Segment_2 seg = triangulation.segment(e); 
        if (seg.squared_length() <= R * R) {
            // Add edge to graph
            Vertex u = station_to_vertex[seg.source()];
            Vertex v = station_to_vertex[seg.target()];
            Edge e; bool s;
            boost::tie(e, s) = boost::add_edge(u, v, network);
        }
    }

    // Perform BFS to find out if there is a network that satisfies the frequency critiria
    // This is essentially vertex coloring with 2 colors
    std::vector<std::pair<bool, int>> vis(N); // visited flags initially false, 0
    std::vector<int> component(N);
    int comp = -1;
    VertexIt vbeg, vend;
    // Satisfiable indicates if a pair-wise disjoint vertex coloring is possible
    bool satisfiable = true;
    for(boost::tie(vbeg, vend) = boost::vertices(network); vbeg != vend && satisfiable; vbeg++) {
        if (vis[*vbeg].first) continue;         // Vertex already visited

        comp++;
	    std::queue<int> Q;                      // BFS queue 
	    vis[*vbeg] = std::make_pair(true, 0);   // Mark the source as visited
        component[*vbeg] = comp;
	    Q.push(*vbeg);
	    while (!Q.empty() && satisfiable) {
		    const int u = Q.front();
		    Q.pop();
		    OutEdgeIt ebeg, eend;
		    for (boost::tie(ebeg, eend) = boost::out_edges(u, network); ebeg != eend; ++ebeg) {
			    const int v = boost::target(*ebeg, network);
			    if (vis[v].first) {
                    if (vis[v].second == vis[u].second) { satisfiable = false; break; } 
                    else continue;
                }
			    vis[v] = std::make_pair(true, (vis[u].second + 1) % 2);
                component[v] = comp;
			    Q.push(v);
		    }
	    }
    }

    // Process all the clues
    for (int i = 0; i < M; i++) {
        K::Point_2 a_i, b_i; std::cin >> a_i >> b_i; 
        if (!satisfiable) { 
                std::cout << "n" << std::flush; 
                continue; 
        } 
        // If the network exists
        // Check if b_i is closer than t1 -> then the radios connect directly
        if (CGAL::squared_distance(a_i, b_i) <= R * R) {
            std::cout << "y"; continue;
        }

        // Find nearest station to a_i and b_i
        K::Point_2 t1 = triangulation.nearest_vertex(a_i)->point();
        K::Point_2 tk = triangulation.nearest_vertex(b_i)->point();

        // Check that both are in range of the nearest station
        if (std::max(CGAL::squared_distance(a_i, t1), CGAL::squared_distance(b_i, tk)) > R * R) {
            std::cout << "n"; continue;
        }

        if (component[station_to_vertex[t1]] == component[station_to_vertex[tk]]) 
            std::cout << "y";
        else 
            std::cout << "n";
    }
    std::cout << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t--) testcase();
    return 0;
}
