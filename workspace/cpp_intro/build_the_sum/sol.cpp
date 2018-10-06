#include <iostream>

void testcase() {

        int n; std::cin >> n;
        // Sum all the numbers
        int result = 0;
        for(int i = 0; i < n; i++) {
            int tmp; std::cin >> tmp;
            result += tmp;
        }

        std::cout << result << std::endl;
    
}

int main() {

        std::ios_base::sync_with_stdio(false);

        int t; std::cin >> t;

        for(int i = 0; i < t; i++) {
            testcase();
        }

        return 0;
}
