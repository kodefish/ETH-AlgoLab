# Buddies
## Description
Given a set of students _n_, each with a set of characteristics, find out if there is an arragement such that we can form _n/2_ pairs of students that share a certain number _c_ of characteristics. In our case we just need to find out if we can do better than a number of characteristics _f_. So we want to find out if there is an arragement of pairs with _c = f+1_ matching characteristics per pair.
If an arrangement with _f+1_ matching characteristics is possible, output _not optimal_, else output _optimal_

## Idea
Use set intersection and group matching.

### Set intersection
We use set intersection to compute a graph of size _n_. A node _(u, v)_ exists iff students _u_ and _v_ have _f+1_ matching characteristics. To find this out, we take the size of the intersection of their characteristics, and add the edge if the that size > f.

### Group Matching
Now that the graph representing all possible pairings is built, we use graph matching (provided by boost) to find a possible arrangement. If the number of edges is n/2, then each student has a buddy and therefore _f_ is not optimal. Otherwise, we can assume that _f_ is optimal.
