#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

typedef std::vector<std::string> vec_s;
typedef std::vector<int> vec_i;
typedef std::vector<std::pair<int, int>> vec_p;
typedef std::vector<vec_i> vec_ii;
typedef std::vector<vec_p> vec_pp;

int binary_search(int x, const vec_i &arr, const vec_i &age) {
    int l = -1;
    int r = arr.size() - 1;

    while (l + 1 < r) {
        int m = (l + r)/2;
        if (age[arr[m]] <= x) r = m;
        else l = m;
    }

    return arr[r];
}

void dfs(int root, const vec_ii &tree, vec_i &path, const vec_pp &queries, vec_i &answers, const vec_i &age) {
    path.push_back(root);

    // Process queries
    for (std::size_t k = 0; k < queries[root].size(); k++) {
        answers[queries[root][k].first] = binary_search(queries[root][k].second, path, age);
    }

    // DFS
    for (std::size_t i = 0; i < tree[root].size(); i++) {
        dfs(tree[root][i], tree, path, queries, answers, age);
    }

    path.pop_back();
}

void testcase() {
    int n, q; std::cin >> n >> q;

    // Work on indices, so create a 2 way mapping between names and idx
    std::map<std::string, int> species_to_idx;
    vec_s idx_to_species(n);
    vec_i age(n);
    int root; int max_age = -1;
    for (int i = 0; i < n; i++) {
        std::string s; int a;
        std::cin >> s >> a;
        species_to_idx[s] = i;
        idx_to_species[i] = s;
        age[i] = a;

        // Root is oldest species
        if (a > max_age) {
            max_age = a;
            root = i;
        }
    }

    // Save graph as adjacency list
    vec_ii tree(n);
    for(int i = 0;  i < n-1; i++) {
        std::string u, v;
        std::cin >> u >> v;

        tree[species_to_idx[v]].push_back(species_to_idx[u]);
    }

    // Save queries as pairs <idx, age> for each species
    vec_pp queries(n);
    for (int k = 0; k < q; k++) {
        std::string s; int b;
        std::cin >> s >> b;
        queries[species_to_idx[s]].push_back(std::make_pair(k, b));
    }

    // Perform a DFS, and answer the queries for each species as we visit that particular node
    // During the DFS, we maintain the path from the node to the vertex, which lets us efficiently
    // search it (using binary search) for the oldest species of age at most b.
    vec_i path;
    vec_i answers(q);
    dfs(root, tree, path, queries, answers, age);

    for (int i = 0; i < q; i++) {
        std::cout << idx_to_species[answers[i]];
        if (i < q-1) std::cout << " ";
    }
    std::cout << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t--) testcase();
    return 0;
}
