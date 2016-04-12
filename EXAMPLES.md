# Some simple problems

Here I put some examples of problems formalized in mvl-DIMACS language. For some of them there is a corresponding script in /tests/ folder.

## Pigeonhole problem

A problem of fitting n pigeons in n-1 holes can be expressed using n variables and [(n-1)^2 * n/2] clauses. The constraint that there has to be one pigeon per hole is taken care of automatically by domain constraints.

```
c This is a pigeonhole problem with 4 pigeons and 3 holes
p cnf 4 18
d 1 3
d 2 3
d 3 3
d 4 3
2!=0 1!=0 0
3!=0 1!=0 0
3!=0 2!=0 0
4!=0 1!=0 0
4!=0 2!=0 0
4!=0 3!=0 0
2!=1 1!=1 0
3!=1 1!=1 0
3!=1 2!=1 0
4!=1 1!=1 0
4!=1 2!=1 0
4!=1 3!=1 0
2!=2 1!=2 0
3!=2 1!=2 0
3!=2 2!=2 0
4!=2 1!=2 0
4!=2 2!=2 0
4!=2 3!=2 0
```

## N-queens

In the same way one can model the *alldifferent* constraint from CSP, which can be used to give a concise formulation of the n-queens problem. For the 8-queens problem we have 8 variables and 504 clauses. Similar Boolean SAT encoding consists of 64 variables and 774 clauses.

```
c This is a 4-queens problem
d 1 4
d 2 4
d 3 4
d 4 4
c alldifferent(1,2,3,4)
2!=0 1!=0 0
3!=0 1!=0 0
3!=0 2!=0 0
4!=0 1!=0 0
4!=0 2!=0 0
4!=0 3!=0 0
2!=1 1!=1 0
3!=1 1!=1 0
3!=1 2!=1 0
4!=1 1!=1 0
4!=1 2!=1 0
4!=1 3!=1 0
2!=2 1!=2 0
3!=2 1!=2 0
3!=2 2!=2 0
4!=2 1!=2 0
4!=2 2!=2 0
4!=2 3!=2 0
2!=3 1!=3 0
3!=3 1!=3 0
3!=3 2!=3 0
4!=3 1!=3 0
4!=3 2!=3 0
4!=3 3!=3 0
c alldifferent for down and up diagonals
1!=0 2!=1 0
1!=0 3!=2 0
1!=0 4!=3 0
1!=1 2!=2 0
1!=1 3!=3 0
1!=2 2!=3 0
2!=0 1!=1 0
2!=0 3!=1 0
2!=0 4!=2 0
2!=1 1!=2 0
2!=1 3!=2 0
2!=1 4!=3 0
2!=2 1!=3 0
2!=2 3!=3 0
3!=0 1!=2 0
3!=0 2!=1 0
3!=0 4!=1 0
3!=1 1!=3 0
3!=1 2!=2 0
3!=1 4!=2 0
3!=2 4!=3 0
3!=2 2!=3 0
4!=0 1!=3 0
4!=0 2!=2 0
4!=0 3!=1 0
4!=1 2!=3 0
4!=1 3!=2 0
4!=2 3!=3 0

```
