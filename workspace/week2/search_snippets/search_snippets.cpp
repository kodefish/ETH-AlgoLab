#include <iostream>
#include <vector>
#include <climits>
using namespace std;

void testcase() {

        // Read num words
        int num_words; cin >> num_words;

        // Read num of occurences of each word
        vector<int> num_occurences;
        for (int i = 0; i < num_words; ++i) {
            int num_occur; cin >> num_occur;
            num_occurences.push_back(num_occur);
        }

        // Read positions of occurence of each word
        vector<vector<int>> occurences;
        for(int i = 0; i < num_words; ++i) {
            vector<int> tmp;
            occurences.push_back(tmp);
            for(int j = 0; j < num_occurences[i]; ++j) {
                int occur; cin >> occur;
                occurences[i].push_back(occur);
            }
        }

        vector<int> iterators(num_words, 0);
        vector<int> diffs;

        int min_diff = INT_MAX;
        bool iterate = true;
        while(iterate) {
            // For every word
            // Greatest position in text
            int max_occur_pos = INT_MIN;
            // Which word occurs last
            int max_occur_word = 0;
            // Smallest position in text
            int min_occur_pos = INT_MAX;
            // Which word occurs first
            int min_occur_word = 0;

            // Find greatest poistion and first positions - and the words that correspond to them
            for(int i = 0; i < iterators.size(); ++i) {
                int occurence = occurences[i][iterators[i]];
                if (max_occur_pos < occurence) {
                    max_occur_pos = occurence;
                    max_occur_word = i;
                } 
                if (min_occur_pos > occurence) {
                    min_occur_pos = occurence;
                    min_occur_word = i;
                }
            }

            // Compute length of sub-sequence
            int diff = max_occur_pos - min_occur_pos + 1;
            if(diff < min_diff) {
                min_diff = diff;
            }

            // Increment the last iterator
            if (iterators[min_occur_word] < num_occurences[min_occur_word] - 1) {
                ++iterators[min_occur_word];
            } else {
                iterate = false;
            } 
        }

        cout << min_diff << endl;
}

int main() {

    std::ios::sync_with_stdio(false);

    int num_testcase; cin >> num_testcase;

    for(int i = 0; i < num_testcase; ++i) {
        testcase();
    }

    return 0;
}
