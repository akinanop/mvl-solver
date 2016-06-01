//**********************************************************************//
//
// Finite Domain Solver
//
// File : SolveFinite.cc
//
//**********************************************************************//
//Including Libraries and Header files
#include "Global.h"
#include "Formula.h"
#include <time.h>

using namespace std;
//Global Declaration - Variables

//**********************************************************************//
//Function Definitions
//SolveFinite: fn to read input and build corresponding objects for solving, calls the cdpll algo
//This function calls the finite domain solver and tries to find a model for
//the problem if the problem is satisfiable
void SolveFinite(CommandLine * cline, string type)
{
  //local variable
  double difftime = 0;
  double totaltime = 0;
  int result = -1;
  Formula * fobj;

  //Creating object
  fobj = new Formula(cline);
  //Building database
  fobj->BuildFormula(cline);
  //Compute Buildtime
  difftime = fobj->TIME_E - fobj->TIME_S;
  totaltime += difftime;
  printf("%s \t%2.4f\t", cline->FILE, fabs(difftime));
  //reinitialize the TIME_S and TIME_E
  fobj->TIME_S = GetTime();
  fobj->TIME_E = GetTime();
  //call the algorithm
  // fobj->checkUnit(); // move this check to the algo

  // *** Extended DPLL Algorithm Options

 if(type == "ch")

 {
      // 1. Chronological Backtracking
     result = fobj->ChronoBacktrack(0); // The function defined in FDSolver/Formula.cc
    }

  // 2. NonChronological Backtracking with Clause learning
 else
    {
      //3. NonChronological Backtracking with Clause learning - "loop version"
    //  result = fobj->NonChronoBacktrack(0); "recursive version"
      if ( cline->WATCH ) {
    	  result = fobj->WatchedLiterals();
      }
      else if (cline->RESTARTS != 0) {
        result = fobj->NonChronoBacktrack(cline->RESTARTS);
      }
      else result = fobj->NonChronoBacktrack(); // The function defined in Formula.cc
    }

  // compute the search time
  difftime = fobj->TIME_E - fobj->TIME_S;
  totaltime += difftime;
  //cout<<"Solving time: "<<endl;
  printf("%2.4f\t %2.4f\t", difftime, totaltime);
  cout<<endl;
  cout<<endl;
  cout << '\a';
  if(result == 0)
    cout<<"SAT"<<endl;
  else if(result == 1)
    cout<<"TIMEOUT"<<endl;
  else
    cout<<"UNSAT"<<endl;
  fobj->PrintInfo();
  if(result == 0)
   { if(cline->MODEL) { cout<<"The model: "<<endl;
     fobj->PrintModel(); }
      cout<<"Verifying model ... ";
       if(fobj->verifyModel())
 	cout<<"model is CORRECT"<<endl;
       else
 	cout<<"model is INCORRECT"<<endl;
     }
}
//**********************************************************************//
