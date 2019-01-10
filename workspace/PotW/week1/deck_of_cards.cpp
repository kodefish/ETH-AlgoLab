#include <iostream>
#include <vector>
#include <climits>
#include <cstdlib>

void testcase(){
  int n, k; std::cin >> n >> k;
  std::vector<int> P(n, 0);
  int sum = 0;
  for(int i = 0; i < n; i++){
    int v_i; std::cin >> v_i;
    sum += v_i;
    P[i] = sum;
  }

  // Sliding window
  int i = 0, j = 0;
  // Result values
  int min = INT_MAX, min_i = 0, min_j = 0;
  while(i != n - 1 || j != n - 1){
    int tmp;
    if (i == 0) {
      tmp = P[j];
    } else {
      tmp = P[j] - P[i - 1];
    }

    if (abs(k - tmp) < min) {
      min = abs(k - tmp);
      min_i = i;
      min_j = j;
    }

    // Increment sliding window
    if ((i >= j || tmp < k) && j < n - 1){
      j++;
    } else {
      i++;
    }
  }

  std::cout << min_i << " " << min_j << std::endl;
}

int main(){
  int t; std::cin >> t;
  while(t--) testcase();
  return 0;
}
