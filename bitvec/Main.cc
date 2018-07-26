//**************************************************************
//
// Finite Domain Solver
// Authors: H. A. Lal, N. Pona
// File : Main.cc
// Description : Main Driver program code
//**************************************************************
//Including Libraries and Header files

#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <string>
#include <cstring>

#include "Global.h"
#include "Formula.h"

using namespace std;
//**************************************************************
//Start of Code
int main(int argc, char ** argv)
{
  //Start Solver Time
  cout<<endl<<"************************************************************************"<<endl;

  cout<<endl<<"Finite Domain Sat Solver"<<endl;

  cout<<endl<<"************************************************************************"<<endl;

  //Parse commandline info
  CommandLine * clineinfo = new CommandLine;
  ParseCommandLine(clineinfo, argc, argv);

  //Check what option is selected and call appropriate function
  if(argc <= 1)
    {
      PrintHelp();
    }

  else if(!strcmp(argv[1], "-solvenc"))
    {
      //Call SolveFinite
      SolveFinite(clineinfo, "nc");

    }

  else
    {
      PrintHelp();
    }

  //return nothing

  return 0;
}
//End of Code
//**************************************************************
