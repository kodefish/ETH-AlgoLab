#include <iostream>

void testcase(){
    int t1; std::cin >> t1;
    long t2; std::cin >> t2;
    std::string t3; std::cin >> t3;
    double t4; std::cin >> t4;

    std::cout << t1 << " " << t2 << " " << t3 << " " << t4 << " " << std::endl;
}

int main(){
    int num_test; std::cin >> num_test;

    for(int i = 0; i < num_test; i++) {
        testcase();
    }

    return 0;
}
