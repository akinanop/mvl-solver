//**********************************************************************//
//
// Finite Domain Solver - Hemal Lal
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
//SolveFinite
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
  fobj->BuildFunction(cline);
  //Compute Buildtime
  difftime = fobj->TIME_E - fobj->TIME_S;
  totaltime += difftime;
  printf("%s \t%2.4f\t", cline->FILE, fabs(difftime));
  //reinitialize the TIME_S and TIME_E
  fobj->TIME_S = GetTime();
  fobj->TIME_E = GetTime();
  //call the algorithm
  fobj->checkUnit();
  // *** Extended DPLL Algorithm Options
  if(type == "ch")
    {
      // 1. Chronological Backtrackin
      result = fobj->ChronoBacktrack(0); // The function defined in Formula.cc
    }
  // 2. NonChronological Backtracking with Clause learning
  //result = fobj->NonChronoBacktrack(0);
  else
    {
      //3. NonChronological Backtracking with Clause learning
      //this is a loop version, faster than recursive
      result = fobj->NonChronoBacktrackLoop(0); // The function defined in Formula.cc
    }
  //compute the search time
  difftime = fobj->TIME_E - fobj->TIME_S;
  totaltime += difftime;
  printf("%2.4f\t %2.4f\t", difftime, totaltime);
  if(result == 0)
    cout<<"SAT"<<endl;
  else if(result == 1)
    cout<<"TIMEOUT"<<endl;
  else
    cout<<"UNSAT"<<endl;
  fobj->PrintInfo();
  if(result == 0)
   {
      cout<<"Verifying model ... ";
       if(fobj->verifyModel())
 	cout<<"model is CORRECT"<<endl;
       else
 	cout<<"model is INCORRECT"<<endl;
     }
}
//**********************************************************************//
