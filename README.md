# Finite Domain Satisfiablity Solver

[![Build Status](https://travis-ci.org/akinanop/mvl-solver.svg?branch=master)](https://travis-ci.org/akinanop/mvl-solver) [![Coverage Status](https://coveralls.io/repos/github/akinanop/mvl-solver/badge.svg?branch=master)](https://coveralls.io/github/akinanop/mvl-solver?branch=master)

To learn what is a finite satisfiability problem, go to the [wiki page](https://github.com/akinanop/mvl-solver/wiki). If you want to try out the solver, download the current folder and follow the instructions below. Click [here](https://github.com/akinanop/mvl-solver/wiki/Benchmarks) to see some problems you could solve.
The progress as of September 2016 is described in [conference poster](https://github.com/akinanop/mvl-solver/blob/master/literature/Conference%20Poster.pdf); for a more detailed description see [ESSLLI Proceedings] (http://www2.sfs.uni-tuebingen.de/esslli-stus-2016/esslli-stus-2016-proceedings.pdf), pp.99-111. The current status of the project is described in the [wiki](https://github.com/akinanop/mvl-solver/wiki).

*This project was partially supported by the Austrian Science Fund (FWF): I836-N23.* 




Table of contents:

* [Building the solver](https://github.com/akinanop/mvl-solver#building-the-solver)
* [Input format for the solver (extended DIMACS)](https://github.com/akinanop/mvl-solver#extended-dimacs-format)
* Using the solver:
  * [Solve a problem](https://github.com/akinanop/mvl-solver#finite-domain-solver-with-non-chronological-backtracking)
  * [Generate random benchmarks](https://github.com/akinanop/mvl-solver#generating-benchmark-problem)
  * [Convert between Boolean and Finite domain problems](https://github.com/akinanop/mvl-solver#convert-boolean-to-finite-domain)

*Note*: This solver corrects and builds upon the unsound solver created by Hemal A. Lal. See: http://www.d.umn.edu/~lalx0004/research/. Currently it implements [non-chronological backtracking with resolution-based learning](https://github.com/akinanop/mvl-solver/blob/master/literature/Algorithm%207.pdf) with the [watched literals](https://github.com/akinanop/mvl-solver/blob/master/literature/Watched%20literals.pdf) bookkeeping technique or without watched literals. It also implements chronological backtracking.

Roughly, about half of the original code was changed and more code was added. If you run ```dwdiff -s``` on the file containing the main functions, you get:

```
dwdiff -s /original_solver/Formula.cc /src/Formula.cc

old: 2848 words  1274 44% common  371 13% deleted  1203 42% changed
new: 6133 words  1274 20% common  1847 30% inserted  3012 49% changed
```


### Building the Solver


You can easily create an executable on Linux using g++ compiler in the following way:

The solver is built using the makefile in the project root directory and g++ compiler. 
There are two ways to build it:
* For runtime experiments use `make` in the project root directory. It builds the solver with the gcc optimization flag `-O3`. 
* For debugging use `make debug` in the project root directory. This does not optimize and adds debug information to the compiled code.

Below follows the description of the input the solver accepts as well as a description of how to use the program to solve a finite-domain problem in DIMACS CNF format, generate a benchmark problem or convert a Boolean problem to a Finite-domain problem.

### Extended DIMACS format

The solver accepts the problems in extended DIMACS CNF format, which is an extension of the standard DIMACS CNF format commonly used for Boolean SAT problems. There are 4 possible lines in a DIMACS file:

1. Comment line: This line contains comments and can be ignored.

    ``` c This is a comment line  ```

2. Problem line: This line contains information about the problem. It begins with a `p`. There is exactly one such line for each problem and it should be the first non-comment line in the problem.

    ```p mvcnf <NumVar> <NumClause>```

    where NumVar is the total number of variables in the problem, and NumClause is the number of
    clauses in the problem.

3. Domain line: This line contains information about the domain size of a variable. It begins with a `d`
and is followed by the variable and then by the domain size. The variables are represented by numbers from 1 to N where N is the total number of variables in the theory.

    ```d <VarName> <DomainSize>```

where VarName is the variable name, and DomainSize is the size of the domain of the variable.
There should be at most one domain line for each variable. The domain lines should be between the problem line and the clause lines and be ordered by VarName (start with variable `1` and end with variable `N`).

4. Clause line: each literal is of the form ```<VarName>=<DomainValue>``` or ```<VarName>!=<DomainValue>```. The literals are separated by space and each clause ends with a `0`, which is used as an end-marker. The domain values are numbers from `0` to `M-1`, where M is the domain size of the variable. As in DIMACS CNF format the literals are linked with OR operations and the clauses with AND operations.

**EXAMPLE**

```
c This is a pigeonhole problem with 3 pigeons and 2 holes
p mvcnf 3 5
d 1 2
d 2 2
d 3 2
2!=0 1!=0 0
3!=0 1!=0 0
3!=0 2!=0 0
2!=1 1!=1 0
3!=1 1!=1 0
3!=1 2!=1 0

```

More examples are available in subfolders of the folder [benchmarks](https://github.com/akinanop/mvl-solver/blob/master/benchmarks).

### Finite Domain Solver

The solver accepts problems in [extended DIMACS format](https://github.com/akinanop/mvl-solver/wiki/Extended-DIMACS-format).

#### Finite Domain Solver with Non-Chronological Backtracking

Use the following format to run the program with non-chronological backtracking.  Watched literals algorithm is a more efficient bookkeeping technique for backtracking, see [here](https://github.com/akinanop/mvl-solver/wiki/Unit-and-conflict-detection#watched-literals). The default heuristic without watched literals is [bookkeeping](https://github.com/akinanop/mvl-solver/wiki/Decision-heuristics#bookkeeping-heuristic), with watched literals it is [VSIDS](https://github.com/akinanop/mvl-solver/wiki/Decision-heuristics#vsids-heuristic).

``` ./mvl-solver -solvenc -file <string> ```

with the options :

```
  mvl-solver     : * name of executable
  -solvenc       : * option stating to solve the finite domain problem with non-chronological backtracking
  -file           : * name of the input file
  -verbose        : turns on the verbose mode
  -wl             : enable watched literals algorithm
  -restarts <int>       : restarts threshold, default - no restarts
  -time <int>          : amount of time allowed for solver to run (in seconds)
  -vsids          : vsids heuristics option
  -lazy 		  : lazy heuristics option

 * - required fields
```
*Example*: ``` ./mvl-solver -solvenc -file "example_SAT" ```

#### Finite Domain Solver with Chronological Backtracking

Use the following format to run the program with chronological backtracking. The default heuristic is [bookkeeping](https://github.com/akinanop/mvl-solver/wiki/Decision-heuristics#bookkeeping-heuristic).

``` ./mvl-solver -solvech -file <string> ```

with the options :

```
  mvl-solver     : * name of executable
  -solvech       : * option stating to solve the finite domain problem with chronological backtracking
  -file           : * name of the input file
  -verbose        : turns on the verbose mode
  -time <int>          : amount of time allowed for solver to run (in seconds)
  -lazy 		  : lazy heuristics option

 * - required fields
```
*Example*: ``` ./mvl-solver -solvech -file "example_SAT" ```


### Generating Benchmark Problem

If you want to generate a random benchmark problem, use the following format to run the program:

``` ./mvl-solver -genben -var <int> -clause <int> -file <string> ```

with the options :

```
   mvl-solver     | * name of executable
  -genben         | * option stating create benchmark problem
  -var            | * number of variables in benchmark problem
  -clause         | * number of clauses in benchmark problem
  -clausesize     |   number of atoms in each clause, [DEFAULT : 3]
  -sat            |   states whether this benchmark problem is satisfiable or not
                      [possible value : 1/0] [default : 1/true]
  -domain         |   states the domain size of each variables, [DEFAULT : 10]
  -drand          |   states whether to assign domain size of each variable randomly
                      [possible value : 1/0] [DEFAULT : 0/false]
  -bool           |   states whether the file is in boolean cnf (0) or modified cnf (1)
  -file           | * name of the output file

 * - required fields
```

Example: ``` ./mvl-solver -genben -var 4 -clause 18 -clausesize 2 -sat 1 -domain 2 -bool 1 -file "example_SAT" ```




### Convert Boolean to Finite Domain

Use the following format to run the program. The solver accepts problems in DIMACS format.

``` ./mvl-solver -b2f -file <string> -model <string> ```
with the options :

```
 mvl-solver             : * name of executable
 -b2f            : * option stating to convert file
 -file           : * name of the boolean file
 -model          : * name of the finite file

 * - required fields
```


### Convert Finite Domain to Boolean : Linear Encoding

Use the following format to run the program:

``` ./mvl-solver -linenc -file <string> -model <string> ```

with the options :
```
  mvl-solver             : * name of executable
 -linenc         : * option stating to convert file
 -file           : * name of the boolean file
 -model          : * name of the finite file

 * - required fields
```


### Convert Finite Domain to Boolean : Quadratic Encoding

Use the following format to run the program:

``` ./mvl-solver -quadenc -file <string> -model <string> ```

with the options :
```
  mvl-solver             : * name of executable
 -quadenc        : * option stating to convert file
 -file           : * name of the boolean file
 -model          : * name of the finite file

 * - required fields
```
