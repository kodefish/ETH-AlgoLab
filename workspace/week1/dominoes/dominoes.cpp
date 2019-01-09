#include <iostream>
#include <vector>

void testcase() {
  // Read in num dominoes and their heights
  int num_dominoes; std::cin >> num_dominoes;
  std::vector<int> h(num_dominoes, 0);
  for (int i = 0; i < num_dominoes; i++) {
    std::cin >> h[i];
  }

  int max_toppled = h[0]; 
  int i = 0; 
  for (; i < max_toppled && i < num_dominoes; i++) {
    int curr = i + h[i];
    if (curr > max_toppled) {
      max_toppled = curr;
    }
  }
  // All the dominoes have toppled.
  std::cout << i << std::endl;
  return;

}

int main() {
  int t; std::cin >> t;
  while(t--) testcase();
  return 0;
}
