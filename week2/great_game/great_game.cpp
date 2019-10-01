#include <iostream>
#include <vector>
#include <climits>

typedef std::vector<int> vec;
typedef std::vector<vec> vec2;

const int MEMO_INIT = -1;

vec2 transitions;
vec2 memo;

int f(int pos, int turn, const int target) {
    // Base case
    if (pos == target - 1) return turn;

    // Check memo
    if (memo[turn % 2][pos] != MEMO_INIT) return memo[turn % 2][pos] + turn;

    // Play a turn
    int result = target;
    if (turn % 2 == 0) { // Sherlocks turn -> minimze
        int min_num_turns = INT_MAX;
        for(int v = 0; v < transitions[pos].size(); v++) {
            int num_turns = f(transitions[pos][v], turn + 1, target);
            if (num_turns < min_num_turns) min_num_turns = num_turns;
        }
        result = min_num_turns;
    } else { // Moriarty's turn -> maxmimse
        int max_num_turns = INT_MIN;
        for(int v = 0; v < transitions[pos].size(); v++) {
            int num_turns = f(transitions[pos][v], turn + 1, target);
            if (num_turns > max_num_turns) max_num_turns = num_turns;
        }
        result = max_num_turns;
    }

    // Save to memo , remove any previous turns because we want a sub-probelem solution
    memo[turn % 2][pos] = result - turn;
    return result;
}

void testcase() {
    int n, m; std::cin >> n >> m;
    int r, b; std::cin >> r >> b;
    r = r - 1; b = b - 1; // These are given between 1 and n, we want them between 0 and n-1

    transitions = vec2(n);
    for (int i = 0; i < m; i++) {
        int u, v; std::cin >> u >> v;
        u = u - 1; v = v - 1; // Same reason as r and b
        transitions[u].push_back(v);
    }

    memo = vec2(2, vec(n, MEMO_INIT));
    int num_moves_red = f(r, 0, n);

    memo = vec2(2, vec(n, MEMO_INIT));
    int num_moves_black = f(b, 0, n);

    if (num_moves_red < num_moves_black) {
        // Sherlock wins
        std::cout << "0" << std::endl;
    } else if (num_moves_red > num_moves_black) {
        // Moriarty wins
        std::cout << "1" << std::endl;
    } else if (num_moves_red == num_moves_black) {
        if (num_moves_red % 2 == 0) {
            // Moriarty wins
            std::cout << "1" << std::endl;
        } else {
            // Sherlock wins
            std::cout << "0" << std::endl;
        }
    }

}

int main() {
    std::ios::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}
