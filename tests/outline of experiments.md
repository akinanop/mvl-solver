###  Preliminary experiments (Feb 2016)

#### Test the solver on randomly generated benchmarks

  1. Vary variables to clause ratio for different domains.
The original experiments described here http://www.d.umn.edu/~lalx0004/research/ReducedResults.htm are unsound due to the mistake in the solver. Moreover, the random procedure used there produces only easy cases.
  Use the experiments described in *RandomSAT/Phase transition for SAT.pdf*. Download Z3 here: https://github.com/Z3Prover/z3.
```
Domain: 2-10
Variables: 75,100,175
Clause size fixed: 3
Ratio: 1-8
```
For Domain=2 (SAT) the sharp phase transition happens at ratio ~4.25. Check whether this holds for other domain sizes. Challenge: for SAT to see the phase transition one had to use fixed-clause random procedure; one has to adapt this for finite domains in a sound way.           

  2. Convert random boolean formulae to finite domain using different encodings (linear and quadratic) and compare performance.

#### Test the solver on unsatisfiable benchmarks

The benchmarks are described in Jain 2008 (Chapter 6): GT_n, Pigeonhole and Pebbling problems.

1. Note number of backtracks and decisions, as well as time.
2. Convert finite domain to boolean and compare the performance.


### Notes

Finite domain (Domain/Var/Clause) | Linear encodings (Var/Clause)
--------------|-----------------
3/75/290 | 375/890
3/100/400 | 500/1200
3/175/690 | 875/2090


TODO: remove domain info after conversion from finite domain to boolean
