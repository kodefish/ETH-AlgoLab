# Defensive Line
## Description
Given a defensive line of $n$ football players, each with a certain strength $v_i$. In front, we have our m attackers, each with the same force $k$. An attacker can attack a line of defensers, such that his strength is equal to the sum of the defensers strength. Our task is to find the maximum number of defenders we can take down. Now if all the attackers can't attack, then that attack is considered as "failed".

## Approach
Pre-processing data, then double dynamic programming (yeah this one is a fun one, but boy was it worth it). 

### Intuition
The idea behind the approach here was to use DP to test out all the possible combinations. Let's look at an example.  
Given the line of defenders [3 1 1 1 1 2], 2 attackers each with strength 3, we have multiple attack strategies:  
(3) (1 1 1) 1 2 -> 4 attacked
(3) 1 (1 1 1) 2 -> 4 attacked
(3) 1 1 1 (1 2) -> 3 attacked
3 (1 1 1) (1 2) -> 5 attacked 

As we can see, there are 4 non-failing strategies. the one we want to find is the last one, where we can take down 5 defenders.

How did we find these strategies? First we choose the defenders attacked by the first attacker, and we do the same, for one less attacker, on the left-over array of defenders (to the right). This problem is slightly similar to the knapsack problem, where we need to decide if we want to keep the current item or not.

From the statement above we can derive the following recursion:
Base case (1 attacker): $f(num_attackers, start_idx) = max_{j from start_idx to end and M[j] != -1}(M[j] - j + 1)$
Recursion (more than 1 attacker): $f(num_attackers, start_idx) = max_{j from start_idx to end-1}(M[j] - j + 1 + f(num_attackers - 1, M[j] + 1)$
Where M is an array we compute during pre-processing, containing all the potential attacked-sub-arrays. 

### Pre-processing
Since all the attackers attack with strength k, we can precompute all the sub-arrays of defenders whose strengths sum up to k. So we get the following:
$M[i] = sum_{j from i to M[i]}(v_j)$. If there is no sub-array starting at $i$, then $M[i] = -1$.
Also an important point to notice here, we are pre-computing all the potential sub-arrays. If there are less potential attacked sub-arrays then there attackers, then there will be no best strategy. The little extra computation it takes to figure this out is completely worth it compared the potential time save. So the pre-processing uses a sliding window to pre-compute and count all the sub-arrays.

```
i = 0, j = 0
var sum = v[i]
var count = 0
var M[n]
while(i < end && j < end)
    if (sum <= k) 
        if (sum == k)
            M[i] = j
            count++

        sum += v[++j]
    else
        sum -= v[i++]
```

The incrementing order of i and j, and access to v is crucial here. So first increment j, then take $v_j$, and first access $v_i$ and then increment $i$. This is really just a basic implementation of a sliding window that computes all the sub-arrays that sum up to k. The variable count keeps track of the number of sub-arrays, and if after the end of this step, if $num_attackers > count$ then we can immediately print "fail" and return

### Recursion
Now that the pre-computation is complete, the fun can begin. We implement the recursion described above, but in order to increase effeciency, the base case is itself solved using DP, which is a very basic case of our problem. Since all DP in this problem takes a top-down approach, we use memoization to avoid recomputing sub-problems.
The function $f(num_attackers, start_idx)$ returns the largest amount of defenders that can be attacked if there are $num_attackers$, and we consider the defenders only after index $start_idx$. So to solve the problem, we want the value of $f(n, 0)$.

#### Base case
The base case is the case where there is only one attacker. So given a starting index $i$, we want to compute the longest sub-array.
$f(i) = max(M[i] - i + 1, f(i+1)$, where the base case is when $i$ is the last element. If $v_{end} = k$, then return 1, otherwise return -1 (no sub-array starts here).
This recursion basically just computes the max subarray possible for every index i. We use this in the general case, and through DP and memoization we avoid recomputing a bunch of base cases (there is alot, especially with 100'000 defenders).

#### Recursive step
The recursive step is where we take the defenition from above:
$f(num_attackers, start_idx) = max_{j from i to end-1}(M[j] - j + 1 + f(num_attackers-1, M[j] + 1))$
So here we compute the number of attackers if we start at j, plus the best case taking everything after the sub-array starting at $j$.
Before computing the value at $j$, if $M[j] = -1$, then we just skip it (check out the code for implementation details like this).
