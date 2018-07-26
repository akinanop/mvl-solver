//**************************************************************
//
// Finite Domain Solver
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
  list <int> UNITLIST;
  int UNITCLAUSE;
  bool CONFLICT;
  int CONFLICTINGCLAUSE;
  vector <Literal> DECSTACK;
  int RESTARTS;
  bool LOG; // verbose
  bool WATCH; //watched literals option
  bool VSIDS; //vsids option

  //public variables and functions
  //Zero argument constructor
  Formula();
  //One argument constructor
  Formula (CommandLine * cline);
  //BuildFunction : builds the theory using the input cnf file
  int sat (Literal literal);
  bool hasAtom(Clause * clause, Literal atom);
  void watchedUndoTheory ( int level );
  void BuildFormula(CommandLine * cline);
  //PrintInfo : prints the backtrack, conflict, unit, entail, pure info
  void PrintInfo();
  // Print model
  void PrintModel();
  //verifyModel : verifies the model that was found, if any. returns true
  //if corret
  bool verifyModel();
  //checkSat : returns true if theory satisfied else false
  int watchedCheckSat(); // 1 sat, 2 continue
  //checkEntail : checks for entailed atom for var and returns entailed
  //domainvalue or -1 if no atom is entailed
  int checkEntail(int var);
  void WatchedUnitPropagation();
  Literal chooseLiteralVSIDS();
  void SwapPointer( VARRECORD* current );

  //reduceTheory : reduces the theory by satisfying literals/clauses
  void watchedReduceTheory(Literal lit);

  void watchedUpdateVar(int var);
  //analyzeConflict : finds the conflict, learns and creates a conflict clause,
  //add's the clause to theory and returns a backtrack level
  Clause * analyzeConflict(Clause * clause, bool freeClauseAfterUse);
  bool potent(Clause * clause);
  int backtrackLevel(Clause * clause);
  //resolve: Extended resolution
  Clause * resolve(Clause * clause, Literal literal, Clause * reason);
  //sets the watched literals of the clause to the two last falsified literals
  void assignWatched( Clause* clause);
  // watched literals algo from Jain:
  int WatchedLiterals( int restarts );
  Literal lazyWatchedChooseLiteral ();

};
// End of Code
//
#endif
//**************************************************************
