//
//  san_francisco.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <queue>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::property<boost::edge_weight_t, long> EW;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
							  boost::no_property, EW>
	weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;

typedef boost::graph_traits<weighted_graph>::out_edge_iterator out_edge_it;

using namespace std;

long bestSum(const int u, const int movesLeft, const weighted_graph &G, const weight_map &weights, vector<vector<long>> &memo)
{
	if (movesLeft == 0)
	{
		return 0;
	}

	long memoVal = memo.at(u).at(movesLeft);

	if (memoVal != -1)
	{
		return memoVal;
	}

	long best = 0;

	bool hasEdge = false;

	long prevBest = 0;
	out_edge_it ebeg, eend;
	for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg)
	{
		hasEdge = true;

		const long winning = weights[*ebeg];
		const long v = boost::target(*ebeg, G);

		const long rec = bestSum(v, movesLeft - 1, G, weights, memo);
		const long upp = rec + winning;

		prevBest = max(prevBest, rec);
		best = max(best, upp);
	}

	if (!hasEdge)
	{
		best = bestSum(0, movesLeft, G, weights, memo);
	}

	memo.at(u).at(movesLeft) = best;

	return best;
}

void testcase()
{
	long n, m, maxScore, moves;

	cin >> n >> m >> maxScore >> moves;

	weighted_graph G(n);
	weight_map weights = boost::get(boost::edge_weight, G);

	vector<vector<int>> edges(n, vector<int>(n, -1));

	for (int i = 0; i < m; i++)
	{
		long u, v, c;
		cin >> u >> v >> c;
		int cur = edges.at(u).at(v);
		if (cur == -1 || c > cur)
		{
			edges.at(u).at(v) = c;
		}
	}

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			int cur = edges.at(i).at(j);
			if (cur != -1)
			{
				boost::add_edge(i, j, EW(cur), G);
			}
		}
	}

	if (boost::out_degree(0, G) == 0)
	{
		cout << "Impossible\n";
		return;
	}

	vector<vector<long>> memo(n + 1, vector<long>(moves + 1, -1));

	int best = -1;
	for (int i = moves; i >= 0; i--)
	{
		long res = bestSum(0, i, G, weights, memo);

		if (res < maxScore)
		{
			break;
		}
		else
		{
			best = i;
		}
	}

	if (best != -1)
	{
		cout << best << "\n";
	}
	else
	{
		cout << "Impossible\n";
	}
}

int main()
{
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
