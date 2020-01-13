// STL includes
#include <iostream>
#include <vector>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

void testcase() {
    // Read in info
    int n, i;
    std::cin >> n >> i;

    // compute original graph
    weighted_graph G(n);
    weight_map weights = boost::get(boost::edge_weight, G);

    std::vector<int> costs;

    for (int j = 1; j < n; j++) {
        for (int k = 1; k < n - j + 1; k++) {
            int c; std::cin >> c;
            costs.push_back(c);

            // Add edge to graph
            edge_desc e;
            e = boost::add_edge(j-1, j+k-1, G).first; weights[e]=c;
        }
    }

    // compute MST leia style
    std::vector<edge_desc> mst;    // vector to store MST edges (not a property map!)
    boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));

    //std::cerr << "Leia MST" << std::endl;
    for (std::vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); ++it) {
        //std::cerr << "\t" << boost::source(*it, G) << " " << boost::target(*it, G) << std::endl;
    }
    
    int min_mst_cost = INT_MAX;

    std::set<int> mst_costs;

    // Remove one edge from MST and try computing new one, keep best version
    for (std::vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); ++it) {
        //std::cerr << "removing " << boost::source(*it, G) << " " << boost::target(*it, G) << "... " << std::endl;
        int source = boost::source(*it, G), target = boost::target(*it, G);
        boost::remove_edge(source, target, G);
        std::vector<edge_desc> new_mst;    // vector to store MST edges (not a property map!)
        boost::kruskal_minimum_spanning_tree(G, std::back_inserter(new_mst));

        int mst_cost = 0;
        //std::cerr << "Modified MST" << std::endl;
        for (std::vector<edge_desc>::iterator it = new_mst.begin(); it != new_mst.end(); ++it) {
            //std::cerr << "\t" << boost::source(*it, G) << " " << boost::target(*it, G) << std::endl;
            mst_cost += weights[*it];
        }

        mst_costs.insert(mst_cost);

        //std::cerr << "\tcost: " << mst_cost << std::endl;

        if (mst_cost < min_mst_cost) min_mst_cost = mst_cost;
        boost::add_edge(source, target, G);
    }

    for (auto it = mst_costs.begin(); it != mst_costs.end(); it++) std::cerr << *it << " ";
    std::cerr << std::endl;

    std::cout << min_mst_cost << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}
