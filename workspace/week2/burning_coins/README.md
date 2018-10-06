# Burning Coins
## Description
Given _n_ coins, each with value $v_i$ lined up in a row, find the maximum amount of money you can get if you play optimally. 2 players, at each turn, they can either take the first or last coin. The game ends when there are no more coins to take.

## Approach
Dynamic Programming with memoization. The following function considers a subset of the coins between _i_ and _j_ (the rest are assumed to be taken). It then computes the result of taking either the first or last coin and returns the max amount if it's "my turn" and the minimum if the adversary is supposed to be playing. If it's "my turn", then we add the value of the coin, otherwise we don't (adversary gets the coin).
The bottom of the recursion is reached when i and j are the same, this case is trivial, so we look at whose turn it is, and depending on that we see who gets to keep it.
To avoid recomputing sub-problems, I used memoization. I keep an array _2_ x _num_coins_ x _num_coins_. Therefore I can easily check if I have already computed the solution to a subproblem.

```
int max_sum(i, j):
    // Bottom of recursion
    if (i == j)
        return my_turn ? coins[i] : 0;

    if (my_turn)
        return max(coins[i] + max_sum(i + 1, j), max_sum(i, j - 1) + coins[j]);
    else 
        return min(max_sum(i + 1, j), max_sum(i, j - 1));
    
```
