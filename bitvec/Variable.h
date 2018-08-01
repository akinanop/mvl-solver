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
#include <bitset>

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

struct IGNode
{
	int index;
	int level;
	int reason;
	bitset<MAX_DOMAIN> values;

	IGNode(int ind, int lev, int reas, bitset<MAX_DOMAIN> vals) {
		index=ind;
		level=lev;
		reason=reas;
		values=vals;
	}
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
  //CURRENT_DOMAIN = current domain of the variable
  //DOMAINSIZE = size of variable's domain

  //ATOMCNTPOS = array storing counts of positive occurences of each domain - for choosing decision literal

  //VARRECORD = linked list of clauses that watch the variable

  int VAR;
  bitset<MAX_DOMAIN> CURRENT_DOMAIN;
  int DOMAINSIZE;
  int * ATOMCNTPOS; // need this for choosing a decision literal

  //  array storing implication graph information for nodes of variable VAR
  vector< IGNode > graphNodes;

  VARRECORD * VARREC;
  //public variables and functions
public:
  //Zero argument constructor
  Variable();
  //Two argument constructor
  Variable(int var, int domain);
  //Destructor
  ~Variable();
  //AddRecord : adds the record to VARREC
  void addRecord(int c_id);
  //RemoveRecord: removes the record from the list (if not watched anymore)
  void removeRecord ( VARRECORD* record );
  //Print : prints the indexes of the clause for each var domain
  void Print();

  IGNode getIGNode(bitset<MAX_DOMAIN> values);

};
// End of Code
//
#endif
//**************************************************************
