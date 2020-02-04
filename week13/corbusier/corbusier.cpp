#include <iostream>
#include <vector>

typedef std::vector<long> vec_l;
typedef std::vector<vec_l> vec_ll;

typedef std::vector<bool> vec_b;
typedef std::vector<vec_b> vec_bb;

bool dp(int h, int d, int num_disks, const int i, const int k, const vec_l &heights, vec_bb &memo_init, vec_bb &memo) {
    if (h == i && num_disks > 0) {
        return true; // satisfactory height reached
    }
    
    if (d == heights.size())  return false; // satisfactory height not reached, but no more disks left
    
    if (memo_init[d][h]) return memo[d][h]; // check memo
    
    // try taking current disk
    if (dp((h + heights[d]) % k, d + 1, num_disks + 1, i, k, heights, memo_init, memo)) {
        memo_init[d][h] = true;
        memo[d][h] = true;
        return true; // take disk d
    }
    
    //  try not taking current disk
    if (dp(h, d + 1, num_disks, i, k, heights, memo_init, memo)) {
        memo_init[d][h] = true;
        memo[d][h] = true;
        return true; // don't take disk d
    }
    
    // impossible starting at disk d with num_disks already taken
    memo_init[d][h] = true;
    memo[d][h] = false;
    return false;
}

void testcase() {
    long n, i, k; std::cin >> n >> i >> k;
    vec_l heights(n);
    for (int i = 0; i < n; i++) std::cin >> heights[i];
    
    std::cerr << k << " " << n << " " << std::endl;
    vec_bb memo_init(n, vec_b(k, false));
    vec_bb memo(n, vec_b(k, false));
    if (dp(0, 0, 0, i, k, heights, memo_init, memo)) std::cout << "yes" << std::endl;
    else std::cout << "no" << std::endl;
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}

