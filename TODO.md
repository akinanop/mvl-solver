
### Urgent

* run solver on pgnhole problems - OK - as expected
* graph coloring preliminary tests
* verify algo on -1 -- see lit
* run solver on nqueens again

### May-June 2016

Outcomes:

1. Well-documented collection of benchmarks.

2. Well-documented and well-tested verified core solver (chronological backtracking and basic non-chronological backtracking algos).
3. Implementation of watched literals algo and CSP heuristic from ``` Siddhartha Jain, Eoin O’Mahony, and Meinolf Sellmann. A complete multi-valued SAT solver. In Principles and Practice of Constraint Programming -
CP 2010 - 16th International Conference, CP 2010, St. Andrews, Scotland,
UK, September 6-10, 2010. Proceedings, pages 281–296, 2010. ```
4. Replicate experiments from the aforementioned paper (see benchmarks).

Easy tasks:

* create more benchmarks (GT_n, latin square...) - OK
* add separate functions for different # of restarts + command line option - OK
* write human-friendly README - OK 
* add pseudo-code for algos and motivation to the documentation (necessary for the esslli paper)
* implement CSP heuristics
* do tests - OK - scripts are ready, just run them 

Moderate tasks:

* add verbose mode from command line
* check for unnecessary backtracks (there are more compared to the FDSolver)
* watched literals algo



