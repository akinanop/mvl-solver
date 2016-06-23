
### Urgent

* Verify basic non-chronological algorithm
* => Verify watched literals non-chronological algorithm
* Implement wl from CMV paper

### June 2016

Expected outcomes:

1. Well-documented collection of benchmarks.

2. Well-documented and well-tested core solver (chronological backtracking and simple resolution-based non-chronological backtracking algos).

3. Implementation of advanced algorithms ``` Siddhartha Jain, Eoin O’Mahony, and Meinolf Sellmann. A complete multi-valued SAT solver. In Principles and Practice of Constraint Programming -
CP 2010 - 16th International Conference, CP 2010, St. Andrews, Scotland,
UK, September 6-10, 2010. Proceedings, pages 281–296, 2010. ```

  3.1. Watched literals with quantified supports;
  
  3.2. Non-Chronological backtracking based on unique implication variables;

4. Replicate experiments from the aforementioned paper (see benchmarks).



Easy tasks:

* look for and create more benchmarks 
* add pseudo-code for algos and motivation to the documentation (necessary for the esslli paper)
* implement extended watched literals
* run tests - OK - scripts are ready, just run them 


Moderate tasks:

* implement unique implication variable algorithm -- maybe have to add new data structure:
	* compare to the current algorithm

Difficult task:

* analyze the results of the tests



