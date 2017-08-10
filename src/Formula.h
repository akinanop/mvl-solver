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
  int LASTFALSE; // for potent fn
  bool CONFLICT;
  int CONFLICTINGCLAUSE;
  vector <Literal *> DECSTACK;
  int RESTARTS;
  bool LOG; // verbose
  bool WATCH; //watched literals option
  bool CMV; //watched literals option
  bool VSIDS; //vsids option

  int WATCHED2; // temp, to fix wl algo
  //public variables and functions
  //Zero argument constructor
  Formula();
  //One argument constructor
  Formula (CommandLine * cline);
  //BuildFunction : builds the theory using the input cnf file
  bool falsifies ( Literal* literal1, Literal* literal2 );
  int sat (Literal* literal);
  bool hasAtom(Clause * clause, Literal * atom);
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
  bool checkSat();
  int watchedCheckSat(); // 1 sat, 0 falsif, 2 continue
  //checkUnit : checks for unit clauses and adds clause id to unitlist
  void checkUnit();
  //checkEntail : checks for entailed atom if any in the theory and
  //stores info in ENTAILLITEAL, returns true if finds one
  bool checkEntail(int var);
  //chooseLiteral : selects next branching literal from the current theory
  Literal* chooseLiteral();
  // choose random unassigned literal
  Literal* lazyChooseLiteral();
  void NewWatchedCheckUnit ();
void WatchedUnitPropagation();
  Literal* chooseLiteralVSIDS();
  void SwapPointer( int clause );

  //reduceTheory : reduces the theory by satisfying literals/clauses
  void reduceTheory(int var, bool equals, int val);
  void watchedReduceTheory(Literal * literal, int var, bool equals, int val);

  //satisfyClauses : reduces the theory by satisfying the clauses
  void watchedSatisfyLiteral(Literal * literal);

  void satisfyClauses(int var, bool equals, int val);
  //removeLiteral : reduces the theory by removing literals from the claues
  void removeLiteral(int var, bool equals, int val);
  void watchedFalsifyLiteral(int var, bool equals, int val);
  void watchedSatisfyLiteral(int var, bool equals, int val);
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
  bool potent(Clause * clause);
  int backtrackLevel(Clause * clause);
  //resolve: Extended resolution
  Clause * resolve(Clause * clause, Literal * literal, Clause * reason);
  //maxLit: return the literal in C that was falsified last (?)
  vector<int> maxLit(Clause * clause);
  //unitPropagation : does BCP in Finite Domain, returns true if no conflict
  //else returns false
  bool LitIsEqual(Literal * literal1, Literal * literal2);
  bool unitPropagation();
  //NonChronoBacktrack : Extended DPLL algorithm with clause learning and
  //non chronological backtracking, returns backtrack level
  int NonChronoBacktrack(int restarts);

  //ChronoBacktrack : Extended DPLL algorithm without any learning
  int ChronoBacktrack(int level);
  // watched literals algo from Jain:
  int WatchedLiterals( int restarts );
  Literal* lazyWatchedChooseLiteral ();

  // CMV watched literals algorithm:


  void tempSwapPointer ( int clause_num );
  inline void tempwatchedFalsifyLiteral ( int var, bool equals, int val );
  void tempassignWatched ( Clause* clause );
  int tempwatchedCheckSat () ;
  inline void tempwatchedSatisfyLiteral ( int var, bool equals, int val );

  Literal* tempwatchedCheckUnit () ;
  Literal * templazyWatchedChooseLiteral ();
  int tempWatchedLiterals ( int restarts );
  inline void tempwatchedSatisfyLiteral( Literal * literal );
  bool supported ( int var, Clause * clause ) ;

};
// End of Code
//
#endif
//**************************************************************
