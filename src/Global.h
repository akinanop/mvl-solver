//**************************************************************
//
// Finite Domain Solver
//
// File : Global.h
// Description : Contains code for Commandline arguments parsing
// and calling functions to run solver
//**************************************************************
#ifndef GLOBAL_H
#define GLOBAL_H

//Including Libraries and Header files
#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <fstream>
#include <set>
#include <vector>
#include <sys/time.h>
#include "Literal.h"
#include "Variable.h"

using namespace std;
//**************************************************************
//Global Declaration - Variables
#define MAX_LINE 3000

struct CommandLine
{ //watched litetarals option
  bool WATCH;
  //verbose
  bool LOG;
  //Number of Variables in CNF File
  int NUM_VAR;
  //Number of Clauses in CNF File
  int NUM_CLAUSE;
  //Number of atoms in each Clause
  int CLAUSE_SIZE;
  //Boolean value : Satisfiable = True, Unsatisfiable = False
  bool SAT_UNSAT;
  //Stores the Max size of domain
  int DOMAIN_SIZE;
  //Boolean value : Random domain size = True, Fixed domain size = False
  bool DRAND;
  //Boolean value : defines if benchmark is in boolean form or modified cnf
  //form(finite)
  bool BOOL_FINITE;
  //File name
  char * FILE;
  //Model file name
  char * MODEL_FILE;
  //Time limit
  int TIME;

  int RESTARTS;
};

//Global Declaration - Functions
//GetTime
//This function sends the time at which it is called.
double GetTime();
//PrintHelp
//This function prints out the options available to use
void PrintHelp();
//ParseCommandLine
//This function is called from main passing the commandline
//Here it will initialize the variable of the structure CommandLine
//and return the object to main
void ParseCommandLine(CommandLine * cline, int argc, char ** argv);
//GenBenFin
//This is the main function for generating benchmark problem in modified
//cnf form
void GenBenFin(CommandLine * cline);
//GenBenBool
//This is the main function for generating boolean benchmark problem in
//boolean cnf form
void GenBenBool(CommandLine * cline);
//Boolean2Finite
//This function converts the boolean file into modified cnf finite format
void Boolean2Finite(CommandLine * cline);
//LinearEncode
//This function converts a finite domain file into a boolean domain format
void LinearEncode(CommandLine * cline);
//QuadraticEncode
//This function converts a finite domain file into a boolean domain format
void QuadraticEncode(CommandLine * cline);
//SolveFinite
//This function is called to solve the finite domain problem
//passed as the argument to the solver
void SolveFinite(CommandLine * cline, string type);
//logging
void Log (char *message);    // logs a message to sdout
void LogErr (char *message); // logs a message; execution is interrupted
#endif
//**********************************************************************//
