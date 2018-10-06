# Seach Snippets
## Description
Given a search query (set of words), and a list of positions in a text at which the words of the query occur, find the minimal length of the substring of the text containing all the search terms.

## Approach
Sliding Window. 
We keep a window containing at least one occurence of every word. We compute the length of the text between the two ends of the two iterators. We then increment the smallest iterator. If it's the last occurence of the word (and it's the smallest), then all following sub sequences will be longer, so we can stop there.
This is not the best implementation as we iterate over every word at every computation of the window.
