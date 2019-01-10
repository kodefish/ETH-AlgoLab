#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

typedef std::vector<double> vec;
typedef std::vector<std::vector<double>> vec2;

double f(int k, int m, int n, const vec &p, int i, vec2 &memo){
  if (k >= m) return 1.0;
  // Base case - magician has left or we ran out of monay
  if (i == n || k == 0) return 0.0;
  
  // Check memo
  if (memo[k][i] != -1) return memo[k][i];

  double max_p = 0; // max probability of winning
  for (int b = 0; b <= k; b++) {
    // Calculate win and loss for bet b
    max_p = std::max(max_p, // Current best probability
		     p[i] * f(k + b, m, n, p, i + 1, memo) // probability if today is won
		     + (1 - p[i]) * f(k - b, m, n, p, i + 1, memo)); // probability if today is lost
  }

  memo[k][i] = max_p;
  return memo[k][i];
}

void testcase(){
  // Read input
  int n, k, m; std::cin >> n >> k >> m;
  vec p(n, 0);
  for(int i = 0; i < n; i++){
    std::cin >> p[i];
  }

  vec2 mem(2*m, vec(n + 1, -1));
  std::cout << f(k, m, n, p, 0, mem) << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  std::cout << std::fixed << std::setprecision(5);
  int t; std::cin >> t;
  while(t--) testcase();
  return 0;
}
