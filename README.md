## Finite Domain Satisfiablity Solver

*Note*: This solver corrects and improves the (unsound) solver created by Hemal A. Lal. See: http://www.d.umn.edu/~lalx0004/research/. In particular, now on the counterexample described in [*Jain_report.pdf*, p.28] the solver produces the correct result.

Below follows the description of how to execute the program.

### Generating Benchmark Problem

If you want to generate a random benchmark problem, use the following format to run the program:

``` exe -genben -var <int> -clause <int> -clausesize <int> -sat <1/0>
      -domain <int> -drand <1/0> -bool <1/0> -file <string> ```

where :

```
exe               | * name of executable

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

EXAMPLE: ``` ./Solver -genben -var 4 -clause 18 -clausesize 2 -sat 0 -domain 2 -bool 1 -file "finite.txt" ```

### Finite Domain Solver with Chronological Backtracking

Use the following format to run the program. The solver accepts problems in extended DIMACS format. See *report.pdf*, p. 30 for the syntax description.

``` exe -solvech -var <int> -clause <int> -file <string> -time <int> ```

where :

```
  exe             : * name of executable
  -solvech        : * option stating to solve the finite domain problem
  -var            : * number of variables in benchmark problem
  -clause         : * number of clauses in benchmark problem
  -file           : * name of the input file
  -time           : amount of time allowed for solver to run (in seconds)

 * - required fields
```
EXAMPLE: ``` ./Solver -solvenc -var 4 -clause 18 -file "counterexample.txt" ```


### Finite Domain Solver with NonChronological Backtracking

Use the following format to run the program:

``` exe -solvenc -var <int> -clause <int> -file <string> -time <int> ```

where :
```
  exe             : * name of executable
  -solvenc        : * option stating to solve the finite domain problem
  -var            : * number of variables in benchmark problem
  -clause         : * number of clauses in benchmark problem
  -file           : * name of the output file
  -time           : amount of time allowed for solver to run

 * - required fields
```


### Convert Boolean to Finite Domain

Use the following format to run the program. The solver accepts problems in DIMACS format.

``` ./Solver -b2f -file <string> -model <string> ```
where :

```
 exe             : * name of executable
 -b2f            : * option stating to convert file
 -file           : * name of the boolean file
 -model          : * name of the finite file

 * - required fields
```


### Convert Finite Domain to Boolean : Linear Encoding

Use the following format to run the program:

``` exe -linenc -file <string> -model <string> ```

where :
```
 exe             : * name of executable
 -linenc         : * option stating to convert file
 -file           : * name of the boolean file
 -model          : * name of the finite file

 * - required fields
```


### Convert Finite Domain to Boolean : Quadratic Encoding

Use the following format to run the program:

``` exe -quadenc -file <string> -model <string> ```

where :
```
 exe             : * name of executable
 -quadenc        : * option stating to convert file
 -file           : * name of the boolean file
 -model          : * name of the finite file

 * - required fields
```
