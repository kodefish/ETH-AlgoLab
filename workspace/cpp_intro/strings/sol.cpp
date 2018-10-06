#include <iostream>

void testcase() {
    std::string s1; std::cin >> s1;
    std::string s2; std::cin >> s2;

    // print length
    std::cout << s1.length() << " " << s2.length() << std::endl;

    // print concatenated version
    std::cout << s1 << s2 << std::endl;

    // reverse the two
    std::string s1_reversed;
    for(int i = 0; i < s1.length(); i++) {
        s1_reversed += s1[s1.length() - i - 1];
    }

    std::string s2_reversed;
    for(int i = 0; i < s2.length(); i++) {
        s2_reversed += s2[s2.length() - i - 1];
    }

    // Swap the first letter of both
    char tmp = s1_reversed[0];
    s1_reversed[0] = s2_reversed[0];
    s2_reversed[0] = tmp;

    std::cout << s1_reversed << " " << s2_reversed << std::endl;

}

int main() {
    int t; std::cin >> t;

    for(int i = 0; i < t; i++) {
        testcase();
    }

    return 0;
}
