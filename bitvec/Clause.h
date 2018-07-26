//**************************************************************
//
// Finite Domain Solver
//
// File : Clause.h
// Description : Contains code for creating clause to store atoms,
// how many atoms there, how many unsatisfied, whether clause is
// satisfied or not, level at which it got satisfied.
//**************************************************************
#ifndef CLAUSE_H
#define CLAUSE_H

//Including Libraries and Header files
#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <string>
#include <vector>

#include "Literal.h"

using namespace std;
//**************************************************************
// Clause
// This class represents the Clause object which stores the atoms
// in the clause. It also stores number of atoms in the clause,
// number of unsatisfied claues, whether clause is satisfied or not,
// level at which it got satisfied.
//
// Start of Code
class Clause
{
  //public variables and functions
public:
  //ATOM_LIST = vector of atoms
  //SAT = boolean variable true if clause satisfied else false
  //NumAtom = number of atoms in the clause
  vector <Literal> ATOM_LIST;

  bool SAT;
  int NumAtom;
  //Watched Literals:
  Literal WATCHED[2];

  //Zero Argument Constructor
  Clause();
  //One Argument Constructor
  Clause(int size);
  //Destructor
  ~Clause();
  //AddAtom : adds an literal to clause
  void addAtom(Literal atom);
  //Print : function to print the clause
  void Print();

  //Accessor functions
  //getAtom : returns atom at index
  Literal getAtom(int index);
  //getSAT : returns the SAT value
  bool getSAT();
  //setSAT : sets SAT value
  void setSAT(bool val);
  //getNumAtom : returns the NumAtom value
  int getNumAtom();
};
// End of Code
//
#endif
//**************************************************************
