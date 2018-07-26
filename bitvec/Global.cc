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
		cout<<" (1) Finite Domain Solver (Non Chronological Backtracking) "<<endl;
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
			cout<<"**** Finite Domain Solver (NonChronological Backtracking) Help ****"<<endl;
			cout<<"**** Use the following format to run the program ****"<<endl;
			cout<<"% exe -solvenc -var <int> -clause <int> -file <string> -time <int> "<<endl<<endl;
			cout<<"where : "<<endl;
			cout<<"  exe             : * name of executable"<<endl;
			cout<<"  -solvenc        : * option stating to solve the finite domain problem"<<endl;
			cout<<"  -var            : number of variables in benchmark problem"<<endl;
			cout<<"  -clause         : number of clauses in benchmark problem"<<endl;
			cout<<"  -file           : * name of the input file"<<endl;
			cout<<"  -time           : amount of time allowed for solver to run"<<endl;
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
	cline->DOMAIN_SIZE = 10;
	cline->FILE = NULL;
	cline->MODEL_FILE = NULL;
	cline->TIME = 3600; //1 hour = 60mins * 60secs
	cline -> RESTARTS = 0;
	cline->MODEL = true;
	cline -> WATCH = true;
	cline -> VSIDS = false;
	cline -> LOG = false;


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

		//If option is for solving a domain problem : Non Chronological Backtracking
		else if(!strcmp(argv[1],"-solvenc"))
		{
			for(current = 2; current < argc; current++)
			{
				if (!strcmp(argv[current], "-restart"))
					cline->RESTARTS = atoi(argv[++current]);
				else if(!strcmp(argv[current], "-verbose"))
					cline->LOG = true;
				else if(!strcmp(argv[current], "-wl"))
					cline -> WATCH = true;
				else if(!strcmp(argv[current], "-vsids"))
					cline -> VSIDS = true;
				else if(!strcmp(argv[current], "-model"))
					cline->MODEL = true;
				else if(!strcmp(argv[current], "-var"))
					cline->NUM_VAR = atoi(argv[++current]);
				else if(!strcmp(argv[current], "-clause"))
					cline->NUM_CLAUSE = atoi(argv[++current]);
				else if(!strcmp(argv[current], "-clausesize"))
					cline->CLAUSE_SIZE = atoi(argv[++current]);
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
