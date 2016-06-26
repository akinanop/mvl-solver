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
 // INDEX = point (# of previous decisions) when literal was falsified
  int VAR;
  bitset MULTIVAL; //  multi-literals

  //Zero argument constructor
  Literal() { VAR = -1;}
  //Two argument constructor
  Literal(int vr, int vl) {
    VAR = vr;
    bitset<domain> values;
    MULTIVAL = values;
    MULTIVAL[vl] = 1;
  }
  Literal(int vr, int domain) {
    VAR = vr;
    bitset<domain> values;
    MULTIVAL = values;
  }

  //Three argument constructor
/*  Literal(int vr, char ch, int vl) {
    VAR = vr;
    bitset<domain> values;
    MULTIVAL = values;

    if(ch == '=')  MULTIVAL[vl] = 1;;
  } */

  //4 argument constructor
  Literal ( int vr, char ch, int vl, int domain ) {
    VAR = vr;
    bitset<domain> values;
    MULTIVAL = values;
    if(ch == '=')  MULTIVAL[vl] = 1;
    else {
        for ( int i = 0; i < MULTIVAL.size() && i != vl; i++ ) {
          MULTIVAL[i] = 1;
        }
    }
  }

  //Print : function to print the atom
  void Print()
  {

    for ( int i = 0; i < MULTIVAL.size(); i++ ){
        cout<<VAR;
        if ( MULTIVAL[i] == 1 ) cout<<"="<<i<<endl;
        else cout<<"!="<<VAL<<endl;
    }

  //  cout<<INDEX;
  }

};
// End of Code
//
#endif
//**************************************************************
