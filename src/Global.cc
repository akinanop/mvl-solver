//**************************************************************
//
// Finite Domain Solver
//
// File : Global.cc
// Description : Contains code for Commandline arguments parsing
// and calling functions to run solver
//**************************************************************
//Including Libraries and Header files
#include "Global.h"
#include <cstring>


using namespace std;
//**********************************************************************//

void Log (char *message){ ; };    // logs a message to sdout
void LogErr (char *message){ ; }; // logs a message; execution is interrupted



//GetTime
//This function returns time in seconds at which it was called
double GetTime()
{
  //local variables
  double t;
  struct timeval tval;
  gettimeofday(&tval, NULL);
  t = (double)tval.tv_sec + (double)tval.tv_usec / 1.0e+6;
  return t;
}

//PrintHelp
//Prints out the messages
void PrintHelp()
{
  //local variable
  int option = 0;
  do
  {
    //Print Menu
    cout<<"**** Menu Options ****"<<endl;
    cout<<" (1) Generate Benchmark Problems "<<endl;
    cout<<" (2) Finite Domain Solver (Chronological Backtracking) "<<endl;
    cout<<" (3) Finite Domain Solver (Non Chronological Backtracking) "<<endl;
    cout<<" (4) Convert Boolean to Finite "<<endl;
    cout<<" (5) Convert Finite to Boolean : Linear Encoding"<<endl;
    cout<<" (6) Convert Finite to Boolean : Quadratic Encoding"<<endl;
    cout<<" (0) Quit"<<endl;
    cout<<endl;
    cout<<" Enter Choice now : ";
    cin>>option;
    cout<<endl;

    //Start Printing messages based on option
    switch(option)
    {
      case 0:
      {
        cout<<"    Goodbye!!!     "<<endl;
        exit(1);
        break;
      }
      case 1:
      {
        cout<<"**** Generating Benchmark Problem Help ****"<<endl;
        cout<<"**** Use the following format to run the program ****"<<endl;
        cout<<"% exe -genben -var <int> -clause <int> -clausesize <int> -sat <1/0>"<<endl;
        cout<<"      -domain <int> -drand <1/0> -file <string>"<<endl<<endl;
        cout<<"where : "<<endl;
        cout<<"  exe             : * name of executable"<<endl;
        cout<<"  -genben         : * option stating create benchmark problem"<<endl;
        cout<<"  -var            : * number of variables in benchmark problem"<<endl;
        cout<<"  -clause         : * number of clauses in benchmark problem"<<endl;
        cout<<"  -clausesize     :   number of atoms in each clause, [DEFAULT : 3]"<<endl;
        cout<<"  -sat            :   states whether this benchmark problem is satisfiable or not"<<endl;
        cout<<"                      [possible value : 1/0] [default : 1/true]"<<endl;
        cout<<"  -domain         :   states the domain size of each variables, [DEFAULT : 10]"<<endl;
        cout<<"  -drand          :   states whether to assign domain size of each variable randomly"<<endl;
        cout<<"                      [possible value : 1/0] [DEFAULT : 0/false]"<<endl;
        cout<<"  -bool           :   states whether the file is in boolean cnf or modified cnf"<<endl;
        cout<<"  -file           : * name of the output file"<<endl;
        cout<<endl;
        cout<<" * - required fields"<<endl<<endl;
        break;
      }

      case 2:
      {
        cout<<"**** Finite Domain Solver (Chronological Backtracking) Help ****"<<endl;
        cout<<"**** Use the following format to run the program ****"<<endl;
        cout<<"% exe -solvech -var <int> -clause <int> -file <string> -time <int> "<<endl<<endl;
        cout<<"where : "<<endl;
        cout<<"  exe             : * name of executable"<<endl;
        cout<<"  -solvech        : * option stating to solve the finite domain problem"<<endl;
        cout<<"  -var            : * number of variables in benchmark problem"<<endl;
        cout<<"  -clause         : * number of clauses in benchmark problem"<<endl;
        cout<<"  -file           : * name of the output file"<<endl;
        cout<<"  -time           : amount of time allowed for solver to run"<<endl;
        cout<<endl;
        cout<<" * - required fields"<<endl<<endl;
        break;
      }

      case 3:
      {
        cout<<"**** Finite Domain Solver (NonChronological Backtracking) Help ****"<<endl;
        cout<<"**** Use the following format to run the program ****"<<endl;
        cout<<"% exe -solvenc -var <int> -clause <int> -file <string> -time <int> "<<endl<<endl;
        cout<<"where : "<<endl;
        cout<<"  exe             : * name of executable"<<endl;
        cout<<"  -solvenc        : * option stating to solve the finite domain problem"<<endl;
        cout<<"  -var            : * number of variables in benchmark problem"<<endl;
        cout<<"  -clause         : * number of clauses in benchmark problem"<<endl;
        cout<<"  -file           : * name of the output file"<<endl;
        cout<<"  -time           : amount of time allowed for solver to run"<<endl;
        cout<<endl;
        cout<<" * - required fields"<<endl<<endl;
        break;
      }

      case 4:
      {
        cout<<"**** Convert Boolean to Finite Domain ****"<<endl;
        cout<<"**** Use the following format to run the program ****"<<endl;
        cout<<"% exe -b2f -file <string> -model <string>"<<endl;
        cout<<"where : "<<endl;
        cout<<" exe             : * name of executable"<<endl;
        cout<<" -b2f            : * option stating to convert file "<<endl;
        cout<<" -file           : * name of the boolean file "<<endl;
        cout<<" -model          : * name of the finite file "<<endl;
        cout<<endl;
        cout<<" * - required fields"<<endl<<endl;
        break;
      }

      case 5:
      {
        cout<<"**** Convert Finite Domain to Boolean : Linear Encoding****"<<endl;
        cout<<"**** Use the following format to run the program ****"<<endl;
        cout<<"% exe -linenc -file <string> -model <string>"<<endl;
        cout<<"where : "<<endl;
        cout<<" exe             : * name of executable"<<endl;
        cout<<" -linenc         : * option stating to convert file "<<endl;
        cout<<" -file           : * name of the boolean file "<<endl;
        cout<<" -model          : * name of the finite file "<<endl;
        cout<<endl;
        cout<<" * - required fields"<<endl<<endl;
        break;
      }

      case 6:
      {
        cout<<"**** Convert Finite Domain to Boolean : Quadratic Encoding****"<<endl;
        cout<<"**** Use the following format to run the program ****"<<endl;
        cout<<"% exe -quadenc -file <string> -model <string>"<<endl;
        cout<<"where : "<<endl;
        cout<<" exe             : * name of executable"<<endl;
        cout<<" -quadenc        : * option stating to convert file "<<endl;
        cout<<" -file           : * name of the boolean file "<<endl;
        cout<<" -model          : * name of the finite file "<<endl;
        cout<<endl;
        cout<<" * - required fields"<<endl<<endl;
        break;
      }

      default:
      {
        break;
      }
    }
    getchar();
    getchar();
  }while(option != 0);
}
//End PrintHelp

//ParseCommandLine
//parsing the commandline options
void ParseCommandLine(CommandLine * cline, int argc, char ** argv)
{
  //local variables
  int current = 1;

  //Default Initialization
  cline->NUM_VAR = -1;
  cline->NUM_CLAUSE = -1;
  cline->CLAUSE_SIZE = 3;
  cline->SAT_UNSAT = true;
  cline->DOMAIN_SIZE = 10;
  cline->DRAND = false;
  cline->BOOL_FINITE = false;
  cline->FILE = NULL;
  cline->MODEL_FILE = NULL;
  cline->TIME = 3600; //1 hour = 60mins * 60secs
  cline->RESTARTS = 0;
  //Check if enough arguments supplied
  if(argc < 2)
  {
    PrintHelp();
  }

  else
  {
    //If Menu/Help option selected
    if(!strcmp(argv[1], "-help"))
    {
      PrintHelp();
    }

    //If option is for generating benchmark problems
    else if(!strcmp(argv[1],"-genben"))
    {
      for(current = 2; current < argc; current++)
      {
        if(!strcmp(argv[current], "-var"))
        cline->NUM_VAR = atoi(argv[++current]);
        else if(!strcmp(argv[current], "-clause"))
        cline->NUM_CLAUSE = atoi(argv[++current]);
        else if(!strcmp(argv[current], "-clausesize"))
        cline->CLAUSE_SIZE = atoi(argv[++current]);
        else if(!strcmp(argv[current], "-sat"))
        cline->SAT_UNSAT = atoi(argv[++current]);
        else if(!strcmp(argv[current], "-domain"))
        cline->DOMAIN_SIZE = atoi(argv[++current]);
        else if(!strcmp(argv[current], "-drand"))
        cline->DRAND = atoi(argv[++current]);
        else if(!strcmp(argv[current], "-bool"))
        cline->BOOL_FINITE = atoi(argv[++current]);
        else if(!strcmp(argv[current], "-file"))
        cline->FILE = argv[++current];
        else
        ;
      }
    }

    //If option is for converting boolean to finite domain
    else if(!strcmp(argv[1], "-b2f"))
    {
      for(current = 2; current < argc; current++)
      {
        if(!strcmp(argv[current], "-file"))
        cline->FILE = argv[++current];
        else if(!strcmp(argv[current], "-model"))
        cline->MODEL_FILE = argv[++current];
        else
        ;
      }
    }

    //If option if for converting finite domain to boolean : Linear Encoding
    else if(!strcmp(argv[1], "-linenc"))
    {
      for(current = 2; current < argc; current++)
      {
        if(!strcmp(argv[current], "-file"))
        cline->FILE = argv[++current];
        else if(!strcmp(argv[current], "-model"))
        cline->MODEL_FILE = argv[++current];
        else
        ;
      }
    }

    //If option if for converting finite domain to boolean : Quadratic Encoding
    else if(!strcmp(argv[1], "-quadenc"))
    {
      for(current = 2; current < argc; current++)
      {
        if(!strcmp(argv[current], "-file"))
        cline->FILE = argv[++current];
        else if(!strcmp(argv[current], "-model"))
        cline->MODEL_FILE = argv[++current];
        else
        ;
      }
    }

    //If option is for solving a domain problem : Chronological Backtracking
    else if(!strcmp(argv[1],"-solvech"))
    {
      for(current = 2; current < argc; current++)
      {
        if(!strcmp(argv[current], "-var"))
        cline->NUM_VAR = atoi(argv[++current]);
        else if(!strcmp(argv[current], "-clause"))
        cline->NUM_CLAUSE = atoi(argv[++current]);
        else if(!strcmp(argv[current], "-clausesize"))
        cline->CLAUSE_SIZE = atoi(argv[++current]);
        else if(!strcmp(argv[current], "-bool"))
        cline->BOOL_FINITE = atoi(argv[current]);
        else if(!strcmp(argv[current], "-file"))
        cline->FILE = argv[++current];
        else if(!strcmp(argv[current], "-model"))
        cline->MODEL_FILE = argv[++current];
        else if(!strcmp(argv[current], "-time"))
        cline->TIME = atoi(argv[++current]);
        else
        ;
      }

    }

    //If option is for solving a domain problem : Non Chronological Backtracking
    else if(!strcmp(argv[1],"-solvenc"))
    {
      for(current = 2; current < argc; current++)
      {
        if (!strcmp(argv[current], "-restart"))
        cline->RESTARTS = atoi(argv[++current]);
        else if(!strcmp(argv[current], "-verbose"))
        cline->LOG = true;
        else if(!strcmp(argv[current], "-cmv"))
        cline->CMV_WATCHED = true;
        else if(!strcmp(argv[current], "-wl"))
        cline->WATCH = true;
        else if(!strcmp(argv[current], "-model"))
        cline->MODEL = true;
        else if(!strcmp(argv[current], "-var"))
        cline->NUM_VAR = atoi(argv[++current]);
        else if(!strcmp(argv[current], "-clause"))
        cline->NUM_CLAUSE = atoi(argv[++current]);
        else if(!strcmp(argv[current], "-clausesize"))
        cline->CLAUSE_SIZE = atoi(argv[++current]);
        else if(!strcmp(argv[current], "-bool"))
        cline->BOOL_FINITE = atoi(argv[current]);
        else if(!strcmp(argv[current], "-file"))
        cline->FILE = argv[++current];
        else if(!strcmp(argv[current], "-model"))
        cline->MODEL_FILE = argv[++current];
        else if(!strcmp(argv[current], "-time"))
        cline->TIME = atoi(argv[++current]);
        else
        ;
      }

    }

    else
    {
      //nothing
      ;
    }
  }
}
//End ParseCommandLine
//**********************************************************************//
