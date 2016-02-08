February 2016

Conduct preliminary expriments:

#### Test the solver on randomly generated benchmarks

* Test for Phase transition and compare to the original results: http://www.d.umn.edu/~lalx0004/research/ReducedResults.htm
```
Domain: 2-10 (step 1)
Variables: 100-1000 (step 100)
Ratio: 1-10 (step 1)
````
* Test unsatisfiable benchmarks described in Jain 2008 (Chapter 6): GT_N, Pigeonhole and Pebbling problems.

#### Compare MVL-Solver with MiniSAT or Z3 
* Compare performance of differenet encodings (linear and quadratic).
* Note number of backtracks and decisions, as well as time.
* Convert boolean to finite domain (pigeonhole and random) and compare the performance. Use the random generator in RandomSAT/. Download Z3 here: https://github.com/Z3Prover/z3.
