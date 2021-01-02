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
  int VAL;
  bool EQUAL;
  //Zero argument constructor
  Literal() { VAR = -1; VAL = -1; EQUAL = false;}
  //Two argument constructor
  Literal(int vr, int vl) { VAR = vr; VAL = vl; EQUAL = true; }
  //Three argument constructor
  Literal(int vr, char ch, int vl)
  { VAR = vr; VAL = vl;
    if(ch == '=') EQUAL = true;
    else EQUAL = false;
  }
  //Three argument constructor
  Literal(int vr, bool eq, int vl)
  { VAR = vr; VAL = vl;
    EQUAL = eq;
  }
  bool operator==(const Literal& lit) const {
    return (VAR == lit.VAR && VAL == lit.VAL && EQUAL == lit.EQUAL);
  }
  bool operator!=(const Literal& lit) const {
    return !(*this==lit);
  }

  //Print : function to print the atom
  void Print()
  {
    cout<<VAR;
    if(EQUAL) cout<<"="<<VAL<<endl; else cout<<"!="<<VAL<<endl;
  //  cout<<INDEX;
  }

};
// End of Code
//
#endif
//**************************************************************
