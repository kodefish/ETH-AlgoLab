#include <iostream>
#include <vector>

typedef std::vector<int> vec_i;
typedef std::vector<vec_i> mat_i;

void testcase(){
  int n; std::cin >> n;
  // Save matrix values and precompute the sums
  mat_i P(n + 1, vec_i(n + 1, 0));
  for (int i = 1; i <= n; i++){
    for (int j = 1; j <= n; j++){
      int x; std::cin >> x;
      // Precomputed sums matrix
      P[i][j] = x + P[i - 1][j] + P[i][j - 1] - P[i - 1][j - 1];
    }
  }

  int solution = 0;
  for (int i1 = 1; i1 <= n; i1++){
    for (int j1 = 1; j1 <= n; j1++){
      for (int i2 = i1; i2 <= n; i2++){
	for (int j2 = j1; j2 <= n; j2++){
	  // Compute value of sub-matrix (i1, j1)-(i2, j2)
	  int sub_sum = P[i2][j2] - P[i1 - 1][j2] - P[i2][j1 - 1] + P[i1 - 1][j1 - 1];
	  if (sub_sum % 2 == 0) {
	    solution++;
	  }
	}
      }
    }
  }

  std::cout << solution << std::endl;
}

int main() {
  int t; std::cin >> t;
  while(t--) testcase();
  return 0;
}
