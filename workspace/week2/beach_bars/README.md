# Beach Bars
## Description
You are given a set of positions along an axis. The goal is to find the optimal location for a bar. A location is optimal if you can reach a maximum number of positions from the position and the distance to them is minimal.

## Approach
Sliding window approach. 
```
1. read positions and sort them by ascending order
2. slide a window over positions
    for every position calculate number of reachable positions from the middle, and the distance to the furthest position.
    if (dist > 100)
        increment lower bound
    else 
        increment upper bound
3. keep only locations that reach a maximal amout of positions while keeping the distance to the furthest one minimal
```

