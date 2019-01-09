#include <iostream>
#include <vector>

void testcase() {
  int n; std::cin >> n;
  std::vector<int> x(n, 0);
  std::vector<int> even(n, 0); // num even ending in i
  std::vector<int> odd(n, 0); // num odd ending in i

  int num_pairs = 0;
  for (int i = 0; i < n; i++) {
    std::cin >> x[i];
    if (i == 0) {
      if (x[i] == 0) {
	even[i] = 1;
      } else {
	odd[i] = 1;
      }
    } else {
      if (x[i] == 0) {
	even[i] = even[i - 1] + 1;
	odd[i] = odd[i - 1];
      } else {
	even[i] = odd[i - 1];
	odd[i] = even[i - 1] + 1;
      }
    }
    num_pairs += even[i];
  }

  std::cout << num_pairs << std::endl;
  
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) testcase();
  return 0;
}
