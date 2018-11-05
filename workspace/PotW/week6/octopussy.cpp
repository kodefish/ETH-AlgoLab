#include <iostream>
#include <vector>
#include <climits>
#include <queue>

int get_parent(int node) {
  int parent_leaf;
  if (node % 2 == 0) {
    parent_leaf = (node - 2) / 2;
  } else {
    parent_leaf = (node - 1) / 2;
  }
  return parent_leaf;
}

struct LeafComparator {
  bool operator()(std::pair<int, int> p1, std::pair<int, int> p2) {
    return p1.second > p2.second;
  }
};

void testcase() {
  // Read in number of bombs
  int n; std::cin >> n;

  // Read in detonation times
  std::vector<int> detonation_time(n);
  std::cin >> detonation_time[0];
  for(int i = 1; i < n; i++) {
    int d_i; std::cin >> d_i;
    int parent_det_time = detonation_time[get_parent(i)];
    detonation_time[i] = std::min(d_i, parent_det_time - 1);
  }

  // Create set of leaf nodes - use priority queue to automatically sort the leaves and get
  // the next bomb to explode in constant time
  std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, LeafComparator> leaves;
  for(int i = (n - 1)/2; i < n; i++) {
    leaves.push(std::make_pair(i, detonation_time[i]));
  }

  // Track number of bombs each bomb is standing on
  std::vector<int> num_leaves((n - 1)/2, 2);

  // Start timer
  int t = 0;

  // Keep going until there are no more leaves, i.e. all bombs are desactivated
  while(!leaves.empty()) {
    // Compute detonation time of earliest bomb
    std::pair<int, int> next_det = leaves.top();
    leaves.pop();
    // Check if there is still time to deactivate it
    if (next_det.second - t < 1) {
      // If it's smaller than 1, then it explodes, so Bond failed, output no
      std::cout << "no" << std::endl;
      return;
    }
   
    // Remove min_leaf from min_leaf's parent's children, and add the parent leaf to the leaves if min_leaf was the last leaf.
    int parent_leaf = get_parent(next_det.first);
    if (parent_leaf >= 0 && --num_leaves[parent_leaf] < 1) {
      leaves.push(std::make_pair(parent_leaf, detonation_time[parent_leaf]));
    }

    t++;
  }

  std::cout << "yes" << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) testcase();
  return 0;
}
