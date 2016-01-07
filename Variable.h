//**************************************************************
//
// Finite Domain Solver - Hemal Lal
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
  //SAT = boolean variable true if variable assigned else false
  //LEVEL = Level at which the variable got satisfied and assigned , default -1
  //ATOMLEVEL = array storing levels at which each domain value is assigned
  //ATOMASSIGN = array storing whether = or != is satisfied, 
  //             0 is none, 1:=, -1:!=
  //ATOMCNTPOS = array storing counts of positive occurences of each domain
  //ATOMCNTNEG = array storing counts of negative occurences of each domain
  //ATOMRECPOS = array storing records of positive occurences of each domain
  //ATOMRECNEG = array storing records of negative occurences of each domain
  //FLAG = array storing true if the literal is involved in conflict
  //CLAUSEID = array storing the clause id which cause this literal to be 
  //           implied
  int VAR;
  int VAL;
  int DOMAINSIZE;
  bool SAT;
  int LEVEL;
  int * ATOMLEVEL;
  int * ATOMASSIGN;
  int * ATOMCNTPOS;
  int * ATOMCNTNEG;
  bool * FLAG;
  int * CLAUSEID;
  VARRECORD ** ATOMRECPOS;
  VARRECORD ** ATOMRECNEG;

  //public variables and functions
public:
  //Zero argument constructor
  Variable();
  //Two argument constructor
  Variable(int var, int domain);
  //Destructor
  ~Variable();
  //AddRecord : adds the record to appropriate fields, true = '='; false= '!'
  void AddRecord(int c_id, int d_id, bool flag);
  //Print : prints the indexes of the clause for each var domain
  void Print();
};
// End of Code
//
#endif
//**************************************************************
