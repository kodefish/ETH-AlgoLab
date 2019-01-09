#include <iostream>

void testcase() {
  int n; std::cin >> n;
  int sum = 0; 
  for(int i = 0; i < n; i++) {
    int a; std::cin >> a;
    sum += a;
  }
  std::cout << sum << std::endl;
}

int main() {
  int t; std::cin >> t;
  while(t--) testcase();
  return 0;
}
