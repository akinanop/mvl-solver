//**********************************************************************//
// GenerateBenchmark
// This program generates benchmarks in modified cnf form
//
// Author: Hemal A Lal
// Date : 03-08-2004
//
//**********************************************************************//
//Including Libraries and Header files
#include "Global.h"
#include "Literal.h"

using namespace std;

//Global Declaration - Variables
//SOLN - array to store solution if problem satisfiable or unsatisfiable
Literal * SOLN;
//CLAUSE_LIST - 2-D array to store each clauses
Literal ** CLAUSE_LIST;
//VARIABLES - 1-D array where VAR is variable name and VALUE is domain size
Literal * VARIABLES;

//**********************************************************************//
//Function Definitions
//GenBenFin
//This function is the brain of generating benchmark problem. It initializes 
//the variables, creates a soln, and writes to the file
void GenBenFin(CommandLine * cline)
{
  //local variables
  int x = 0;
  int y = 0;
  int z = 0;

  srand(time(NULL));

  //Assign Memory to each global variable
  SOLN = new Literal[cline->NUM_VAR];

  CLAUSE_LIST = new Literal *[cline->NUM_CLAUSE];
  for(y=0; y<cline->NUM_CLAUSE; y++)
    CLAUSE_LIST[y] = new Literal[cline->CLAUSE_SIZE];

  VARIABLES = new Literal [cline->NUM_VAR];

  //Assigning domain values to each variable
  //fixed size
  if(!cline->DRAND)
    {
      for(x=0; x<cline->NUM_VAR; x++)
	{
	  VARIABLES[x].VAR = x;
	  VARIABLES[x].VAL = cline->DOMAIN_SIZE;
	}
    }
  //random domain size
  else
    {
      for(x=0; x<cline->NUM_VAR; x++)
	{
	  VARIABLES[x].VAR = x;
	  VARIABLES[x].VAL = rand() % cline->DOMAIN_SIZE;
	}      
    }

  //Generating solution
  for(x=0; x<cline->NUM_VAR; x++)
    {
      SOLN[x].VAR = x;
      SOLN[x].VAL = rand() % VARIABLES[x].VAL;
    }

  //Generating clauses
  bool already_exist = false;
  for(y=0; y<cline->NUM_CLAUSE; y++)
    {
      for(x=0; x<cline->CLAUSE_SIZE; x++)
	{
	  //checking if problem needs to be solvable or random
	  //if satisfiable
	  if(cline->SAT_UNSAT && x==0) 
	    {
	      //assigning variable to clause
	      CLAUSE_LIST[y][x].VAR = SOLN[rand() % cline->NUM_VAR].VAR;
	      CLAUSE_LIST[y][x].VAL = SOLN[CLAUSE_LIST[y][x].VAR].VAL;
	    }
	  
	  //else unsatisfiable
	  else
	    {
	      //assigning variable to clause
	      CLAUSE_LIST[y][x].VAR = rand() % cline->NUM_VAR;
	      //checking if variable already exist	
	      do
		{
		  already_exist = false;
		  for(z=0; z<x && !already_exist; z++)
		    {
		      if(CLAUSE_LIST[y][x].VAR == CLAUSE_LIST[y][z].VAR)
			{
			  CLAUSE_LIST[y][x].VAR = rand() % cline->NUM_VAR;
			  already_exist = true;
			}
		      else
			{
			  already_exist = false;
			}
		    }
		}while(already_exist);	  
	      CLAUSE_LIST[y][x].VAL = rand() % VARIABLES[CLAUSE_LIST[y][x].VAR].VAL;
	    }
	}
    }

  //opening and writing to outputfile
  ofstream outfile;
  outfile.open(cline->FILE, ios::out);
  if(!outfile)
    {
      cout<<endl;
      cout<<"**** ERROR ****"<<endl;
      cout<<" Could not open output file to write "<<endl;
      cout<<"****       ****"<<endl;
      cout<<endl;
    }
  else
    {
      outfile<<"p cnf "<<cline->NUM_VAR<<" "<<cline->NUM_CLAUSE<<endl;
      for(y=0; y<cline->NUM_VAR; y++)
	{
	  outfile<<"d "<<VARIABLES[y].VAR+1<<" "<<VARIABLES[y].VAL<<endl;
	}
      for(y=0; y<cline->NUM_CLAUSE; y++)
	{
	  for(x=0; x<cline->CLAUSE_SIZE; x++)
	    {
	      outfile<<CLAUSE_LIST[y][x].VAR+1<<"="<<CLAUSE_LIST[y][x].VAL<<" ";
	    }
	  outfile<<"0"<<endl;
	}
    }
  outfile.close();
}

//GenBenBool
//This function is the brain of generating benchmark problem. It initializes the
//variables, creates a soln, and writes to the file in boolean form
void GenBenBool(CommandLine * cline)
{
  //local variables
  int x = 0;
  int y = 0;
  int z = 0;
  int * BSOLN;  
  int ** BCLAUSE_LIST;
  srand(time(NULL));
  
  //Assign Memory to each global variable
  BSOLN = new int[cline->NUM_VAR+1];

  BCLAUSE_LIST = new int *[cline->NUM_CLAUSE];
  for(y=0; y<cline->NUM_CLAUSE; y++)
    BCLAUSE_LIST[y] = new int[cline->CLAUSE_SIZE];

  //Generating solution
  for(x=1; x<cline->NUM_VAR+1; x++)
    {
      if(rand() % 2)
	BSOLN[x] = x;
      else
	BSOLN[x] = -x;
    }

  //Generating clauses
  bool already_exist = false;
  for(y=0; y<cline->NUM_CLAUSE; y++)
    {
      for(x=0; x<cline->CLAUSE_SIZE; x++)
	{
	  //checking if problem needs to be solvable or random
	  //if satisfiable
	  if(cline->SAT_UNSAT && x==0) 
	    {
	      //assigning variable to clause
	      if(rand()%2)
		{
		  BCLAUSE_LIST[y][x] = BSOLN[(rand() % cline->NUM_VAR)+1];
		}
	      else
		{
		  BCLAUSE_LIST[y][x] = -1 * BSOLN[(rand() % cline->NUM_VAR)+1];
		}
	    }
	  
	  //else unsatisfiable
	  else
	    {
	      //assigning variable to clause
	      if(rand()%2)
		{
		  BCLAUSE_LIST[y][x] = (rand() % cline->NUM_VAR) + 1;
		}
	      else
		{
		  BCLAUSE_LIST[y][x] = -1 * ((rand() % cline->NUM_VAR) + 1);
		}
	      //checking if variable already exist	
	      do
		{
		  already_exist = false;
		  for(z=0; z<x && !already_exist; z++)
		    {
		      if(BCLAUSE_LIST[y][x] == BCLAUSE_LIST[y][z])
			{
			  BCLAUSE_LIST[y][x] = (rand() % cline->NUM_VAR) + 1;
			  already_exist = true;
			}
		      else
			{
			  already_exist = false;
			}
		    }
		}while(already_exist);	  
	    }
	}
    }

  //opening and writing to outputfile
  ofstream outfile;
  outfile.open(cline->FILE, ios::out);
  if(!outfile)
    {
      cout<<endl;
      cout<<"**** ERROR ****"<<endl;
      cout<<" Could not open output file to write "<<endl;
      cout<<"****       ****"<<endl;
      cout<<endl;
    }
  else
    {
      outfile<<"p cnf "<<cline->NUM_VAR<<" "<<cline->NUM_CLAUSE<<endl;
      for(y=0; y<cline->NUM_CLAUSE; y++)
	{
	  for(x=0; x<cline->CLAUSE_SIZE; x++)
	    {
	      outfile<<BCLAUSE_LIST[y][x]<<" ";
	    }
	  outfile<<"0"<<endl;
	}
    }
  outfile.close();
}
//End GenBenFunction
//**********************************************************************//
