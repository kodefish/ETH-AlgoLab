#include <iostream>

void testcase() {
  int n; std::cin >> n;
  int partial_sum = 0, even_pairs = 1, odd_pairs = 0, res = 0;

  for (int i = 0; i < n; i++) {
    int tmp; std::cin >> tmp;
    partial_sum += tmp;

    // If partial sum is odd
    if (partial_sum % 2 != 0) {
      // Number of possible odd partial sums we can pick to create an even sum doubles
      res += odd_pairs;
      odd_pairs++;
    } else {
      // Number of possible even partial sums we can pick to create an even sum doubles
      res += even_pairs;
      even_pairs++;
    }
  }

  std::cout << res << std::endl;
}

int main() {
  int t; std::cin >> t;
  while(t--) testcase();
  return 0;
}
