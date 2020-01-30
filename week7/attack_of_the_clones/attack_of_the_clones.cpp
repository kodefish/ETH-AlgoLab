// Credit to LuigiAndMario for his solution
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

typedef std::pair<int, int> pair_i;
typedef std::vector<pair_i> vec_pi;

bool edf(pair_i t1, pair_i t2) {
    return t1.second < t2.second;
}

void testcase() {
    int n, m; std::cin >> n >> m;

    vec_pi tasks(n);
    std::map<int, int> tasks_per_interval;
    for (int i = 0 ; i < n ; i++) {
        int a, b; std::cin >> a >> b;
        tasks[i] = std::make_pair(--a, --b); // a, b are given from 1 to m (included)
        
        tasks_per_interval[a]++;
        tasks_per_interval[b]--;
        if (a > b) {
            tasks_per_interval[0]++;
        }
    }
    
    // General assumption (from the description): there is an interval s where there's at most 10 tasks
    int s = -1;
    int nb_tasks = 0;
    int min_tasks = n;
    const int MAX_TASKS = 10;
    for (auto it : tasks_per_interval) {
        nb_tasks += it.second;
        if (nb_tasks <= min_tasks && nb_tasks <= MAX_TASKS) {
            s = it.first;
            min_tasks = nb_tasks;
        }
    }
    
    // Now that we have this least protected segment, we can shift everything so that it starts there
    // that way, we have tasks that go over this segment, and tasks that don't
    vec_pi regular_tasks, modular_tasks;

    for (int i = 0 ; i < n ; i++) {
        int start = tasks[i].first;
        int deadline = tasks[i].second;
        
        pair_i shifted = std::make_pair(start - s, deadline - s);
        if (start <= s) {
            shifted.first += m;
        }
        if (deadline <= s) {
            shifted.second += m;
        }
        
        if ((start <= deadline && (start <= s && s <= deadline)) || (start > deadline && (start <= s || s <= deadline))) {
            // A task is modular if either:
            //  it was previously regular, and starts before s and ends after, or
            //  it was previously modular, and either started before s or ended after s
            modular_tasks.push_back(shifted);
        } else {
            regular_tasks.push_back(shifted);
        }
    }
    
    std::sort(regular_tasks.begin(), regular_tasks.end(), edf);
    
    int scheduled = 0;
    int start = -1;
    for (auto it : regular_tasks) {
        if (it.first > start) {
            start = it.second;
            scheduled++;
        }
    }
    
    // Now we try to schedule every one of those modular tasks
    for (auto it : modular_tasks) {
        int start = it.first;
        int deadline = it.second % m;
        
        int schedulable = 1; // It starts at 1 because *it is already scheduled
        for (auto jt : regular_tasks) {
            if (jt.first > deadline && jt.second < start) {
                // In other words: if *jt does not overlap with *it (the modular task)
                deadline = jt.second;
                schedulable++;
            }
        }
        
        scheduled = std::max(scheduled, schedulable);
    }
    
    std::cout << scheduled << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}


