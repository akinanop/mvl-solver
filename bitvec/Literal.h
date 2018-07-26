//**************************************************************
//
// Finite Domain Solver
//
// File : Literal.h
// Description : Contains code for Literal class. Literal contains a
// variable and its value, eg. var = val or var ! val
//**************************************************************
#ifndef ATOM_H
#define ATOM_H

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

#define MAX_DOMAIN 64

using namespace std;
//**************************************************************
// Literal
// This class represents the Literal object for the solver which
// stores the variable name, and its value for the benchmark. It
// also stores a boolean value stating whether the atom is a
// positive or negative literal
//
// Start of Code
class Literal
{
  //public variables and functions
public:
  //VAR = variable name
  //VAL = value assigned to variable
  //EQUAL = is the atom = (true) or ! (false)
  int VAR;

  bitset<MAX_DOMAIN> VALS;

  //Zero argument constructor
  Literal() { VAR = -1; }
  //Two argument constructors
  Literal(int var, int val) {
	  VAR = var;
	  VALS = bitset<MAX_DOMAIN>();
	  if (val!=-1)
	  VALS.set(val);
  }
 Literal(int var, bitset<MAX_DOMAIN> vals) {
	  VAR = var;
	  VALS = vals;
  }

  bool superset(bitset<MAX_DOMAIN> vals) {
	  return (~VALS & vals).none();
  }

  bool falsifies(bitset<MAX_DOMAIN> vals) {
	  return (VALS & vals).none();
  }

  //Print : function to print the atom
  void Print()
  {
    cout<<VAR<<"=";
    cout<<VALS<<endl;
  }

};
// End of Code
//
#endif
//**************************************************************
