#include <iostream>
#include <vector>

typedef std::vector<int> vec_i;
typedef std::vector<vec_i> vec_ii;

void testcase() {
    // Read input and computer partial sums over matrix (truncate the matrix for partial sums, discard anything to the left of the rightmost indice)
    int n; std::cin >> n;

    vec_ii partial_row_sum(n, vec_i(n, 0));
    vec_ii partial_matrix_sum(n, vec_i(n, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int mij; std::cin >> mij;
            if (j == 0) {
                partial_row_sum[i][j] = mij;
            } else {
                partial_row_sum[i][j] = partial_row_sum[i][j - 1] + mij;
            }

            partial_matrix_sum[i][j] = partial_row_sum[i][j];
            if (i > 0) partial_matrix_sum[i][j] += partial_matrix_sum[i-1][j];
        }
    }

    // Reduce problem to many instances of even pairs
    // for every i, j, we can use the partial sums of each column, and thus reduce to even pairs
    int even_matrices = 0;
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            int even_pairs = 1, odd_pairs = 0, res = 0;
            for (int h = 0; h < n; h++) {
                int sum_p = partial_matrix_sum[j][h];
                if (i > 0) sum_p -= partial_matrix_sum[i - 1][h];

                if (sum_p % 2 == 0) {
                    res += even_pairs;
                    even_pairs++;
                } else {
                    res += odd_pairs;
                    odd_pairs++;
                }
            }
            even_matrices += res;
        }
    }

    std::cout << even_matrices << std::endl;

}

int main() {
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}
