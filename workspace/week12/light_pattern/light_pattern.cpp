// Did not understand the problem.. this should solve the first test case, but apparently does not.
#include <iostream>
//#include <boost/dynamic_bitset.hpp>

void testcase() {
    int N, K;
    unsigned long X;
    std::cin >> N >> K >> X;

    //boost::dynamic_bitset<> pattern(K, X);

    int num_on = 0;
    for (int i = 0; i < N; i++) {
        int state; std::cin >> state;
        std::cout << state << " ";
        num_on += state;
    }
    std::cout << std::endl;

    std::cout << num_on << " " << N - num_on + 1 << std::endl;
    std::cout << std::min(num_on, N - num_on + 1) << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t--) testcase();
    return 0;
}
