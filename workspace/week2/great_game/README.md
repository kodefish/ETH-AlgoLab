# Great Game
## Description
As input we get a board with _n_ positions and _m_ transitions, and 2 meeple starting positions. The game is that 2 players move the meeples along transitions. If the red meeple reaches the last posiiton first, then player 1 wins, if it's the black one then player 2 wins. The players move the meeples based on the number of moves they have made so far. If it's even, they move their meeple, otherwise they move their opponents meeple. The objective is to determine who has a winning strategy.

## Approach
The game is deterministic, based on where each meeple starts and which player has a first turn, the game's outcome can be easily determined. When moving his meeple a player will try to minimize his distance to the goal, and when moving his opponent's then he will try to maximize this distance.

Using dynamic programming, we can precompute the best and worst choices for each position on the board (bottom-up approach). Starting at the finish, we work our way  to the start of the board. The we simulate the game, counting how many moves each meeples makes. The one with the less moves wins.

The best and worst distances are computed as follows:
```
for every transition from u to v
    min_dist = min(1 + max_dist_from[v])
    max_dist = min(1 + min_dist_from[v])
```
Notice that `min_dist` is `1 + max_dist_from[...]`. This is done on purpose. Since the next move will be played by the adversary, we must take into account that he will try to maximize our distance to the target. The same reasoning follows for `max_dist`.
