//**************************************************************
//
// Finite Domain Solver - Hemal Lal
//
// File : Formula.h
// Description : represents the whole formula, contains the
// variables, all the clauses in the theory, maintains info about
// time spent for building, solving, and overall time. Also number
// of backtracks, conflicts etc.
//**************************************************************
#ifndef FORMULA_H
#define FORMULA_H

//Including Libraries and Header files
#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <string>
#include <list>
#include <vector>

#include "Global.h"
#include "Literal.h"
#include "Variable.h"
#include "Clause.h"

using namespace std;
//**************************************************************
// Formula
// This class represents the Formula object for the solver
// which stores all the variables, all the clauses in the theory,
// backtracks and conflict info, decision making info, etc.
//
// Start of Code
class Formula
{
  //public variables and functions
public:
  //VARLIST = list of all the variables
  //CLAUSELIST = list of all the clauses in the theory
  //TIMELIMIT = amount of time allowed for solving before timing out
  //TIME_S = starting time of solving
  //TIME_E = end time of solving
  //LEVEL = stores the current level
  //BACKTRACKS = stores number of backtracks done so far
  //DECISIONS = stores number of decisions done so far
  //UNITS = stores number of unit propagations done so far
  //ENTAILS = stores number of entailments done so far
  //ENTAILLITERAL = atom to store entailed literal info
  //UNITLIST = stores info about unit clauses in theory
  //UNITCLAUSE = clause that just got unit and satisfied
  //CONFLICT = boolean variable true when conflict in theory
  //CONFLICTINGCLAUSE = clause that is causing conflict in theory
  //DECSTACK = stores the decisions/implications made in order
  vector<Variable *> VARLIST;
  vector<Clause *> CLAUSELIST;
  int TIMELIMIT;
  double TIME_S;
  double TIME_E;
  int LEVEL;
  int BACKTRACKS;
  int DECISIONS;
  int UNITS;
  int ENTAILS;
  Literal * ENTAILLITERAL;
  list <int> UNITLIST;
  int UNITCLAUSE;
  bool CONFLICT;
  int CONFLICTINGCLAUSE;
  vector <Literal *> DECSTACK;
  Clause * REASON;
  // int ORIGINALFORMULA;
  //public variables and functions
  //Zero argument constructor
  Formula();
  //One argument constructor
  Formula (CommandLine * cline);
  // 2-arg constructor
  Formula(int vars, int clauses);
  //BuildFunction : builds the theory using the input cnf file
  bool HasAtom(Clause * clause, Literal * atom);

  void BuildFunction(CommandLine * cline);
  void BuildFunction(string name);
  //PrintVar : prints the variable and its value
  void PrintVar();
  //PrintClause : prints the clauses in the theory
  void PrintClauses();
  //PrintInfo : prints the backtrack, conflict, unit, entail, pure info
  void PrintInfo();
  //verifyModel : verifies the model that was found, if any. returns true
  //if corret
  bool verifyModel();
  //checkSat : returns true if theory satisfied else false
  bool checkSat();
  //checkConflict : return an integer value representing the conflict
  //clause else returns -1
  int checkConflict();
  //checkUnit : checks for unit clauses and adds clause id to unitlist
  void checkUnit();
  //checkEntail : checks for entailed atom if any in the theory and
  //stores info in ENTAILLITEAL, returns true if finds one
  bool checkEntail(int var);
  //chooseLiteral : selects next branching literal from the current theory
  void addReason(int var, int val); // needed for analyzing coflict
  Literal * chooseLiteral();
  //reduceTheory : reduces the theory by satisfying literals/clauses
  void reduceTheory(int var, bool equals, int val);
  //satisfyClauses : reduces the theory by satisfying the clauses
  void satisfyClauses(int var, bool equals, int val);
  //removeLiteral : reduces the theory by removing literals from the claues
  void removeLiteral(int var, bool equals, int val);
  //undoTheory : brings the theory back at the level stage
  void undoTheory(int level);
  //unsatisfyClauses : brings back the clauses that were satisfied before
  //this level
  void unsatisfyClauses(int var, bool equals, int val, int level);
  //addLiteral : adds literal reduced when satisfying other literal
  void addLiteral(int var, bool equals, int val);
  //analyzeConflict : finds the conflict, learns and creates a conflict clause,
  //add's the clause to theory and returns a backtrack level
  Clause * analyzeConflict(Clause * clause);
  bool Potent(Clause * clause);
  int backtrackLevel(Clause * clause);
  //whyFalse: return the literal inconsistent with the input literal that was added to the interpretation earliest
  Literal * whyFalse(Literal * literal);
  //resolve: Extended resolution
  Clause * resolve(Clause * clause, Literal * literal, Clause * reason);
  //maxLit: return the literal in C that was falsified last (?)
  Literal * maxLit(Clause * clause);
  //unitPropagation : does BCP in Finite Domain, returns true if no conflict
  //else returns false
  //bool unitPropagation();
// find unit literal in the unit clause
  Literal * unitLiteral(Clause * clause);
  //NonChronoBacktrack : Extended DPLL algorithm with clause learning and
  //non chronological backtracking, returns backtrack level
  int NonChronoBacktrack();

  int NonChronoBacktrackLoop(int level);
  //ChronoBacktrack : Extended DPLL algorithm without any learning
  int ChronoBacktrack(int level);
};
// End of Code
//
#endif
//**************************************************************
