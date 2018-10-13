#include <iostream>
#include <vector>
#include <climits>

typedef std::vector<int> int_vec;
typedef std::vector<int_vec> int_vec_2d;

// Computes the max if there is only one attacker
int f2(int_vec &memo2, const int_vec &M, int start_idx, int end_idx) {

    // Base case 
    if (start_idx == end_idx - 1) {
        return (M[start_idx] == -1) ? -1 : 1;
    }

    if (memo2[start_idx] != -42) {
        return memo2[start_idx];
    }

    // Rec case
    int curr = M[start_idx] - start_idx + 1;
    int rec = f2(memo2, M, start_idx + 1, end_idx);
    memo2[start_idx] = (curr > rec) ? curr:rec;
    return memo2[start_idx];
}

int f(int_vec_2d &memo, int_vec &memo2, const int_vec &M, int num_attackers, int start_idx, int end_idx) {


    // Edge case
    if (start_idx == end_idx) {
        return -1;
    }

    // Check memo first 
    if (memo[num_attackers - 1][start_idx] != -42) {
        return memo[num_attackers - 1][start_idx];
    }

    // Base case, num of attackers is 1 -> return optimal solution
    if (num_attackers == 1) {
        int max_attack = f2(memo2, M, start_idx, end_idx);
        memo[num_attackers - 1][start_idx] = max_attack;
        return max_attack;
    }

    // Recursive step
    int max_attack = -1;
    for (int j = start_idx; j < end_idx - 1; ++j) {
        int curr_attack;
        if (M[j] == -1) {
            curr_attack = -1;
        } else {
            int rec_max = f(memo, memo2, M, num_attackers - 1, M[j] + 1, end_idx);
            if (rec_max == -1) {
                curr_attack = -1;
            } else {
                curr_attack = M[j] - j + 1 + rec_max;
            }
        }
        if (curr_attack > max_attack) {
            max_attack = curr_attack;
        }
    }
    memo[num_attackers - 1][start_idx] = max_attack;
    return max_attack;
}

void testcase() {
    // Read num defensive players, num attackers, attacker strength
    int n, m, k; std::cin >> n >> m >> k;

    // Read defensive line
    std::vector<int> v(n);
    for(int i = 0; i < n; ++i) {
        std::cin >> v[i];
    }

    // Step 1. Precompute M
    std::vector<int> M(n, -1);
    int i = 0; 
    int j = 0;
    int sum = v[i];
    int countK = 0;
    while (i < n && j < n) {
        if (sum <= k) {
            if (sum == k) {
                M[i] = j;
                countK++;
            }

            j++;
            sum += v[j];
        } else {
            sum -= v[i];
            i++;
        }
    }

    // Step 1.2 Check that we can actually have a valid solution
    if (countK < m) {
        std::cout << "fail" << std::endl;
        return;
    }

    // Step 2. Recursively compute the max value of defensers that can be attacked
    int_vec_2d memo(m, int_vec(n, -42));
    int_vec memo2(n, -42);
    int max_attacked = f(memo, memo2, M, m, 0, n);
    
    if (max_attacked < 0) {
        std::cout << "fail" << std::endl;
    } else {
        std::cout << max_attacked << std::endl;
    }

    return;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int num_test; std::cin >> num_test;

    for(int i = 0; i < num_test; ++i) {
        testcase();
    }

    return 0;
}
