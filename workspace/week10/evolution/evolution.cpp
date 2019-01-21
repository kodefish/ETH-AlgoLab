#include <iostream>
#include <map>
#include <string>
#include <cstring>
#include <climits>

void testcase() {
    int N, Q; std::cin >> N >> Q;

    std::map<std::string, int> age;
    std::string root_species; int max_age = INT_MIN;

    for (int i = 0; i < N; i++) {
        std::string s; int a; std::cin >> s >> a;
        age[s] = a;
        if (a > max_age) {
            max_age = a;
            root_species = s;
        }
    }

    std::map<std::string, std::string> ancestor;
    for (int i = 0; i < N - 1; i++) {
        std::string s, p; std::cin >> s >> p;
        ancestor[s] = p;
    }

    for (int q = 0; q < Q; q++) {
        std::string s; int b; std::cin >> s >> b;
        while (s != root_species) {
            if (age[ancestor[s]]  > b) 
                break;
            else 
                s = ancestor[s];
        }
        std::cout << s << " ";
    }
    std::cout << std::endl;


}

int main() {
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}
