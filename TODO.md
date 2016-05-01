May-June 2016

Outcomes:

1. Well-documented collection of benchmarks.

2. Well-documented and (informally) verified core solver (chronological backtracking and basic non-chronological backtracking algos).
3. Implementation of watched literals algo and CSP heuristic from ``` Siddhartha Jain, Eoin O’Mahony, and Meinolf Sellmann. A complete multi-valued SAT solver. In Principles and Practice of Constraint Programming -
CP 2010 - 16th International Conference, CP 2010, St. Andrews, Scotland,
UK, September 6-10, 2010. Proceedings, pages 281–296, 2010. ```
4. Replicate experiments from the aforementioned paper (see benchmarks).

Easy tasks:

* create more benchmarks (GT_n, latin square)
* add separate functions for different # of restarts + command line option
* write human-friendly README


Moderate tasks:

* add verbose mode from command line
* check for unnecessary backtracks ???
* plan further implementation: watched literals, CSP heuristics etc.



