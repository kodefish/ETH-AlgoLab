#include <iostream>
#include <vector>

typedef std::vector<int> vec;
typedef std::vector<vec> vec2;

bool contains(int a, int b, int i) {
    if (a <= b) {
        return a < i && i < b;
    } else {
        return !contains(b, a, i);
    }
}

void testcase() {
    // Read input
    int n, m; std::cin >> n >> m;
    // Store segments by finish, i.e. starts[i] = starting points of segments that end in i
    vec2 starts(m);
    for (int i = 0; i < n; i++) {
        int a, b; std::cin >> a >> b; 
        starts[b - 1].push_back(a - 1);
    }

    int num_seg = 0;
    int closest = 0;
    int first_start = -1;
    int i = 0;
    do {
        // Find closest ending segment that does not overlap and that is shortest.
        while (starts[closest].size() == 0) {
            closest = (closest + 1) % m;
        } 

        int new_i = i;
        int shortest_start = i; //i.e the one closest to the finish
        for (int j = 0; j < starts[closest].size(); j++) {
            if (!contains(starts[closest][j], closest, i)) {
                new_i = closest;
                shortest_start = std::max(shortest_start, starts[closest][j]);
            }
        }

        if (new_i != i) {
            i = new_i; 
            if (first_start == -1) {
                first_start = shortest_start; 
            }
            num_seg++;
        } else {
            closest = (closest + 1) % m;
        }
    } while (first_start < i && i < m - 1);

    std::cout << num_seg << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t--) testcase();
    return 0;
}
