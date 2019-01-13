// Special thanks goes to tonykarkats from github for kindly making his repo publicly available
#include <iostream>
#include <vector>
#include <map>
#include <climits>

typedef std::vector<int> ivec;
typedef std::vector<ivec> ivec_2;
typedef std::vector<ivec_2> ivec_3;

// Maps light configuration to the minimum number of light flicks needed to acheive it
// considering only switches in the [start, end] range
std::map<ivec, int> compute_light_configs(int start, int end, int n, int m, const ivec_3 &switches) {
    int len = end - start + 1;
    std::map<ivec, int> config;

    // Iterate over all possible flick configs
    for(int flick = 0; flick < (1 << len); flick++) {
        ivec lights(m);  
        int flick_cnt = 0;
        // Go over all the switches in specified range.
        // Add number of lights according to whether the lights are on or off.
        for(int sw = start; sw <= end; sw++) {
            // Figure out if switch is on or off
            bool off = flick & (1 << (sw - start));
            if (off) {
                flick_cnt++;
            }

            // Go over all the rooms
            for (int r = 0; r < m; r++) {
                lights[r] += switches[sw][r][off];
            }
        }

        auto it = config.find(lights);
        if (it != config.end()) {
            it->second = std::min(it->second, flick_cnt); 
        } else {
            config.insert(std::make_pair(lights, flick_cnt));
        }
    }

    return config;
}

void testcase() {
    int n, m; std::cin >> n >> m;

    ivec brightness(m, 0);
    for(int i = 0; i < m; i++) {
        std::cin >> brightness[i]; 
    }

    ivec_3 switches(n, std::vector<std::vector<int>>(m, ivec(2, 0)));
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            std::cin >> switches[i][j][0] >> switches[i][j][1]; 
        }
    }

    // Split into two halves and compute min flicks for all possible light combinations. Yes, all of them.
    int split = n / 2;
    std::map<ivec, int> a, b;
    a = compute_light_configs(0, split, n, m, switches);
    b = compute_light_configs(split + 1, n - 1, n, m, switches);

    // Combine the two lists to see if we got a possible solution
    int min_flicks = INT_MAX;
    for(auto a_itr : a) {
        ivec lacking(m); // lights still needing to be turned on 
        for(int i = 0; i < m; i++) {
            lacking[i] =  brightness[i] - a_itr.first[i];
        }

        // See if there is a corresponding config in the other half
        auto b_itr = b.find(lacking);
        if(b_itr != b.end()) {
            min_flicks = std::min(min_flicks, a_itr.second + b_itr->second); 
        }
    }

    if (min_flicks < INT_MAX) {
        std::cout << min_flicks << std::endl; 
    } else {
        std::cout << "impossible" << std::endl; 
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t--) testcase();
    return 0;
}
