//**************************************************************
//
// Finite Domain Solver
//
// File : Variable.h
// Description : represents the variable object for the solver
// which stores the variable name, the value it is assigned to,
// the level it got assigned, the claues it is related to for
// each domain value, the domain size, and few more things for
// bookkeeping purposes
//**************************************************************
#ifndef VARIABLE_H
#define VARIABLE_H

//Including Libraries and Header files
#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <string>

#include "Clause.h"

using namespace std;
//**************************************************************
// VARRECORD
// This is a linkedlist which stores the clause number in which
// the variable with that domain value exists
struct VARRECORD
{
  //c_num = clause number
  int c_num;
  //next = link to next element
  VARRECORD * next;
  VARRECORD * prev;
};

// Variable
// This class represents the variable object for the solver
// which stores the variable name, the value it is assigned to,
// the level it got assigned, the claues it is related to for
// each domain value, the domain size, and few more things for
// bookkeeping purposes
//
// Start of Code
class Variable
{
  //public variables and functions
public:
  //VAR = variable name
  //VAL = value assigned to this variable
  //DOMAINSIZE = size of variable's domain
  //ATOMLEVEL = array storing levels at which each domain value is assigned
  //ATOMASSIGN = array storing whether = or != is satisfied,
  //             0 is none, 1:=, -1:!=

  //ATOMCNTPOS = array storing counts of positive occurences of each domain - for choosing decision literal
  //ATOMCNTNEG = array storing counts of negative occurences of each domain

  //ATOMWATCHPOS, ATOMWATCHNEG = linked lists storing clause ids of clauses where the literal is watched

  //ATOMRECPOS, ATOMRECNEG = arrays storing clause ids of literal occurrences for positive/negative literals

  //CLAUSEID = array storing the clause id which cause this literal to be
  //           implied

  int VAR;
  int VAL;
  int DOMAINSIZE;
  //  array storing index (decstack size) at which literal is falsified
  int * ATOMINDEX; // order at which atoms where was falsified
  int * ATOMLEVEL;
  int * ATOMASSIGN;
  int * ATOMCNTPOS; // need this for choosing a decision literal
  int * ATOMCNTNEG; // need this for choosing a decision literal
  double * VSIDS_SCORE; // literal score for the VSIDS decision heuristic
  double * VSIDS_SCORE_NEG; // literal score for the VSIDS decision heuristic
  int * CLAUSEID;
  VARRECORD ** ATOMWATCHPOS;
  VARRECORD ** ATOMWATCHNEG;
  vector<int> * ATOMRECPOS;
  vector<int> * ATOMRECNEG;
  //public variables and functions
public:
  //Zero argument constructor
  Variable();
  //Two argument constructor
  Variable(int var, int domain);
  //Destructor
  ~Variable();
  //AddRecord : adds the record to appropriate fields, true = '='; false= '!'
  void addRecord(int c_id, int d_id, bool flag);
  //RemoveRecord : removes the record from the watched list
  void removeRecord(VARRECORD* record, int d_id, bool flag);
  //Print : prints the indexes of the clause for each var domain
  void Print();
};
// End of Code
//
#endif
//**************************************************************
