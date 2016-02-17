//**********************************************************************//
//
// Finite Domain Solver - Hemal Lal
//
// File : Formula.cc
//
//**********************************************************************//
//Including Libraries and Header files
#include "Formula.h"
#include <cstring>
using namespace std;
//**********************************************************************//
//Default 0-arg constructor
Formula::Formula()
{
  //Default values - 10 atoms per clause, 10 variables per problem
  VARLIST.reserve(10);
  CLAUSELIST.reserve(10);
  TIMELIMIT = 3600; //1hour = 60mins * 60secs
  TIME_S = 0;
  TIME_E = 0;
  LEVEL = 0;
  UNITS = 0;
  BACKTRACKS = 0;
  DECISIONS = 0;
  ENTAILS = 0;
  UNITCLAUSE = -1;
  CONFLICT = false;
  CONFLICTINGCLAUSE = -1;
  DECSTACK.reserve(10);
}

//1-arg constructor
Formula::Formula(CommandLine * cline)
{
  //Setting appropriate values and resizing vectors
  VARLIST.reserve(cline->NUM_VAR+1);
  CLAUSELIST.reserve((int)(cline->CLAUSE_SIZE*1.2));
  TIMELIMIT = cline->TIME;
  TIME_S = 0;
  TIME_E = 0;
  LEVEL = 0;
  UNITS = 0;
  BACKTRACKS = 0;
  DECISIONS = 0;
  ENTAILS = 0;
  UNITCLAUSE = -1;
  CONFLICT = false;
  CONFLICTINGCLAUSE = -1;
  DECSTACK.reserve(cline->NUM_VAR+1);
}


//======================= READ INPUT =========================================\\
================================================================================

//BuildFunction
void Formula::BuildFunction(CommandLine * cline)
{
  //local variables
  ifstream infile;
  char line_buffer[3000];
  char word_buffer[3000];
  int clause_num = 0;
  int atom_num = 0;
  int var, val;
  char ch;
  char ch1;
  string str;
  int temp;
  Variable * temp_var = NULL;
  Literal * temp_atom = NULL;
  Clause * temp_clause = NULL;

  //to start variable from 1 and not from 0 we need this
  VARLIST.push_back( new Variable(0, 0));
  VARLIST[0]->SAT = true;

  TIME_S = GetTime();
  //opening infile to read and checking if it opens
  infile.open(cline->FILE, ios::in);
  if(!infile)
    {
      cout<<endl;
      cout<<"**** ERROR ****"<<endl;
      cout<<"Could not open input file : "<<endl;
      cout<<endl;
      exit(1);
    }

  //now reading each line from the infile and if needed to convert
  //then converting it and writing to outfile or just writing directly
  while(infile.getline(line_buffer, 3000))
    {
      temp_clause = new Clause(cline->CLAUSE_SIZE);
      //if line is comment - just write
      if(line_buffer[0] == 'c')
	{
	  //do nothing
	  ;
	}
      //else if line is p cnf var_num clause_num - just write
      else if(line_buffer[0] == 'p')
	{
	  //do nothing
	  temp = sscanf(line_buffer, "p cnf %d %d", &var, &val);
	  if((var != cline->NUM_VAR) || (val != cline->NUM_CLAUSE))
	    {
	      cout<<"**** ERROR ****"<<endl;
	      cout<<" Commandline arguments and file arguments do not match."<<endl;
	      cout<<endl;
	      exit(1);
	    }
	}
      //else if line is d var# domsize
      else if(line_buffer[0] == 'd')
	{
	  temp = sscanf(line_buffer, "d %d %d", &var, &val);
	  temp_var = new Variable(var, val);
	  VARLIST.push_back(temp_var);
	}
      //else its variables
      else
	{
	  char * lp = line_buffer;
	  do
	    {
	      char * wp = word_buffer;
	      //erasing all leading space/tabs
	      while (*lp && ((*lp == ' ') || (*lp == '\t')))
		{
		  lp++;
		}
	      //reading each value into word buffer
	      while (*lp && (*lp != ' ') && (*lp != '\t') && (*lp != '\n'))
		{
		  *(wp++) = *(lp++);
		}
	      *wp = '\0';
	      //converting into int and writing to file
	      if(strlen(word_buffer) != 0)
		{
		  temp = sscanf(word_buffer, "%d %c %d", &var, &ch, &val);
		  if(ch == '!')
		    temp = sscanf(word_buffer, "%d %c %c %d", &var, &ch, &ch1, &val);
		  //checking if variable = 0 i.e end of line
		  if(var != 0)
		    {
		      //add it to clause_list's atom_list
		      temp_atom = new Literal(var, ch, val);
		      temp_clause->AddAtom(temp_atom);

		      //increment the index representing value so that
		      //we can keep track of number of occurences of each
		      //variable and its domain value, if ch is = then increment
		      //the value count, else increment others count
		      if(ch == '=')
			{
			  VARLIST[var]->ATOMCNTPOS[val]++;
			  VARLIST[var]->AddRecord(clause_num, val, true);
			}
		      else
			{
			  VARLIST[var]->ATOMCNTNEG[val]++;
			  VARLIST[var]->AddRecord(clause_num, val, false);
			}
		      atom_num++;
		    }
		}
	    }while(*lp);
	  CLAUSELIST.push_back(temp_clause);
	  atom_num = 0;
	  ++clause_num;
	}
    }

  //closing file
  infile.close();
  TIME_E = GetTime();
}

//PrintVar
void Formula::PrintVar()
{
  //local variable
  int size = VARLIST.size();
  for(int x=1; x<size; x++)
    {
      cout<<VARLIST[x]->VAR<<"="<<VARLIST[x]->VAL;
      cout<<" Domain = "<<VARLIST[x]->DOMAINSIZE<<endl;
      VARLIST[x]->Print();
      cout<<endl;
    }
}

//PrintClause
void Formula::PrintClauses()
{
  //local variable
  int size = CLAUSELIST.size();
  for(int x=0; x<size; x++)
    {
      CLAUSELIST[x]->Print();
      cout<<endl;
    }
}

//PrintInfo
void Formula::PrintInfo()
{
  printf("\n");
  printf("Number of Decisions   : %d\n", DECISIONS);
  printf("Number of Units       : %d\n", UNITS);
  printf("Number of Backtracks  : %d\n", BACKTRACKS);
  printf("Number of Entails     : %d\n", ENTAILS);
  printf("Number of Levels      : %d\n", LEVEL);
  printf("Number of Variables   : %d\n", VARLIST.size()-1);
  printf("Number of Clauses     : %d\n", CLAUSELIST.size());
  printf("\n");
}
//============================================================================\\
================================================================================

bool Formula::verifyModel()
{
  //set all clause to false
  for(int i=0; i<CLAUSELIST.size(); i++)
    CLAUSELIST[i]->SAT = false;
  //for each variable and its value set all clauses in
  //which it occurs to true
  int val = 0;
  int domainsize = 0;
  VARRECORD * curr = NULL;
  for(int i=1; i<VARLIST.size(); i++)
    {
      val = VARLIST[i]->VAL;
      if(val != -1)
	{
	  domainsize = VARLIST[i]->DOMAINSIZE;
	  for(int j=0; j<domainsize; j++)
	    {
	      if(j != val)
		{
		  curr = VARLIST[i]->ATOMRECNEG[j];
		  while(curr)
		    {
		      CLAUSELIST[curr->c_num]->SAT = true;
		      curr = curr->next;
		    }
		}
	      else
		{
		  curr = VARLIST[i]->ATOMRECPOS[j];
		  while(curr)
		    {
		      CLAUSELIST[curr->c_num]->SAT = true;
		      curr = curr->next;
		    }
		}
	    }
	}
      else
	{
	  domainsize = VARLIST[i]->DOMAINSIZE;
	  for(int j=0; j<domainsize; j++)
	    {
	      if(VARLIST[i]->ATOMASSIGN[j] == -1)
		{
		  curr = VARLIST[i]->ATOMRECNEG[j];
		  while(curr)
		    {
		      CLAUSELIST[curr->c_num]->SAT = true;
		      curr = curr->next;
		    }
		}
	      else
		VARLIST[i]->VAL = j;
	    }
	}
    }
  return (checkSat());
}

// checkSat: if some clause is not SAT return false
bool Formula::checkSat()
{ cout <<"Checking satisfiabiity"<<endl;
  int size = CLAUSELIST.size();
  cout <<"# of clauses: "<<size<<endl;
  for(int i=0; i<size; i++)
    {
      if(!CLAUSELIST[i]->SAT)
	{ cout<<"Some clause not sat"<<endl;
	  return false;
	}
    }
    cout<<"All clauses are sat"<<endl;
  return true;
}

//checkConflict
int Formula::checkConflict()
{
  int size = CLAUSELIST.size();
  for(int i=0; i<size; i++)
    {
      //if clause is not SAT and has 0 unassigned literal
      //return index i
      if((CLAUSELIST[i]->NumUnAss == 0) &&
	 (!CLAUSELIST[i]->SAT))
	return i;
    }
  return -1;
}

//checkUnit
void Formula::checkUnit()
{
  int size = CLAUSELIST.size();
  UNITLIST.clear();
  for(int i=0; i<size; i++)
    {
      //if clause is not SAT and has 1 unassigned literal
      //add the index i into Unitlist
      if((CLAUSELIST[i]->NumUnAss == 1) &&
	 (!CLAUSELIST[i]->SAT))
	UNITLIST.push_back(i);
    }
}

//checkEntail
bool Formula::checkEntail(int var)
{
 if(ENTAILLITERAL)
    delete ENTAILLITERAL;

  int domainsize = 0;
  bool flag = false;
  int domainvalue = -1;

  //if variable not satisfied
  if(!VARLIST[var]->SAT)
    {
      domainsize = VARLIST[var]->DOMAINSIZE;
      //check its domain and see that exactly one
      //domain value has atomassign 0, if more than one
      //then there is no entailment
      for(int j=0; j<domainsize; j++)
	{
	  if(VARLIST[var]->ATOMASSIGN[j] == 0)
	    {
	      if(!flag)
		{
		  domainvalue = j;
		  flag = true;
		}
	      else
		{
		  flag = false;
		  break;
		}
	    }
	}
    }
  //if found an entail literal then assign memory to variable
  //and return true
  if(flag)
    {
      ENTAILLITERAL = new Literal(var, domainvalue);
      return true;
    }
  return false;
}

//chooseLiteral: MISTAKE in choosing the literal
Literal * Formula::chooseLiteral()
{
  int max = -1;
  int size = VARLIST.size();
  int domainsize = -1;
  int tvar = -1;
  int tval = -1;
  int tmax = -1;
  UNITCLAUSE = -1;
  for(int i=0; i<size; i++)
    {
      if(!VARLIST[i]->SAT)
	{
	  domainsize = VARLIST[i]->DOMAINSIZE;
	  for(int j=0; j<domainsize; j++)
	    {
	      if(VARLIST[i]->ATOMASSIGN[j] == 0)
		{
		  tmax = VARLIST[i]->ATOMCNTPOS[j] - VARLIST[i]->ATOMCNTNEG[j];
		//  if(max < tmax)
		  //  {
		      max = tmax;
		      tvar = i;
		      tval = j;
		    //}
		}
	    }
	}
    }

    cout<<"var: "<< tvar <<endl;
  return (tvar != -1 ? new Literal(tvar, tval) : NULL);
}

//reduceTheory
void Formula::reduceTheory(int var, bool equals, int val)
{
  if(equals)
    {
      cout<<"reducing : "<<var<<"="<<val<<" at "<<LEVEL<<endl;
      //first satisfy all clauses with literal, and remove
      //negate literal from claues
      satisfyClauses(var, equals, val);
      removeLiteral(var, !equals, val);
      VARLIST[var]->ATOMASSIGN[val] = 1;
      VARLIST[var]->ATOMLEVEL[val] = LEVEL;

      VARLIST[var]->VAL = val;
      VARLIST[var]->SAT = true;
      VARLIST[var]->LEVEL = LEVEL;

      VARLIST[var]->CLAUSEID[val] = UNITCLAUSE;
      //Add literal to DecisionStack
      DECSTACK.push_back(new Literal(var, '=', val));
      cout<<"reducing2 : "<<var<<"="<<val<<" at "<<LEVEL<<endl;

      //foreach domain value x from dom(v) which is not assigned
      //assign it
      for(int i=0; i<val && !CONFLICT; i++)
	{
	  if(VARLIST[var]->ATOMASSIGN[i] == 0)
	    {
	      satisfyClauses(var, !equals, i);
	      removeLiteral(var, equals, i);
	      VARLIST[var]->ATOMASSIGN[i] = -1;
	      VARLIST[var]->ATOMLEVEL[i] = LEVEL;
	      VARLIST[var]->CLAUSEID[i] = UNITCLAUSE;
	    }
	}
      for(int i=val+1; i<VARLIST[var]->DOMAINSIZE && !CONFLICT; i++)
	{

	  if(VARLIST[var]->ATOMASSIGN[i] == 0)
	    {
	      satisfyClauses(var, !equals, i);
	      removeLiteral(var, equals, i);
	      VARLIST[var]->ATOMASSIGN[i] = -1;
	      VARLIST[var]->ATOMLEVEL[i] = LEVEL;
	      VARLIST[var]->CLAUSEID[i] = UNITCLAUSE;
	    }
	}
    }
  else
    {
      cout<<"reducing : "<<var<<"!"<<val<<" at "<<LEVEL<<endl;
      //first satisfy all clauses with negate literal, and remove
      //literal from claues
      satisfyClauses(var, !equals, val);
      removeLiteral(var, equals, val);
      VARLIST[var]->ATOMASSIGN[val] = -1;
      VARLIST[var]->ATOMLEVEL[val] = LEVEL;

      VARLIST[var]->CLAUSEID[val] = UNITCLAUSE;

      //Add literal to DecisionStack
      DECSTACK.push_back(new Literal(var, '!', val));


      //check Entailment on this variable

   // TODO FIX BUG IS HERE!!!!

    }

    if(checkEntail(var))
  	{
  	  ENTAILS++;
  	  cout<<"Entailment .."<<ENTAILLITERAL->VAR<<"="<<ENTAILLITERAL->VAL<<endl;
  	  VARLIST[ENTAILLITERAL->VAR]->CLAUSEID[ENTAILLITERAL->VAL] = UNITCLAUSE;
  	  reduceTheory(ENTAILLITERAL->VAR, true, ENTAILLITERAL->VAL);
  	}
}

//satisfyClauses
inline void Formula::satisfyClauses(int var, bool equals, int val)
{
  int lit_var = -1;
  bool lit_equal = false;
  int lit_val = -1;
  VARRECORD * current = NULL;
  if(equals)
    current = VARLIST[var]->ATOMRECPOS[val];
  else
    current = VARLIST[var]->ATOMRECNEG[val];

  //for every clause that contains this literal, satisfy it
  //and update the counts of the literals
  while(current)
    {
      if(!CLAUSELIST[current->c_num]->SAT)
	{
	  CLAUSELIST[current->c_num]->SAT = true;
	  CLAUSELIST[current->c_num]->LEVEL = LEVEL;
	  for(int i=0; i<CLAUSELIST[current->c_num]->NumAtom; i++)
	    {
	      lit_var = CLAUSELIST[current->c_num]->ATOM_LIST[i]->VAR;
	      lit_equal = CLAUSELIST[current->c_num]->ATOM_LIST[i]->EQUAL;
	      lit_val = CLAUSELIST[current->c_num]->ATOM_LIST[i]->VAL;
	      if(VARLIST[lit_var]->ATOMASSIGN[lit_val] == 0)
		{
		  CLAUSELIST[current->c_num]->NumUnAss--;
		  if(lit_equal)
		    VARLIST[lit_var]->ATOMCNTPOS[lit_val]--;
		  else
		    VARLIST[lit_var]->ATOMCNTNEG[lit_val]--;
		}
	    }
	}
      current = current->next;
    }
  current = NULL;
}

//removeLiteral
inline void Formula::removeLiteral(int var, bool equals, int val)
{
  VARRECORD * current = NULL;
  //for every record of this literal reduce the number of
  //unassigned literals from unsatisfied clauses
  if(equals)
    current = VARLIST[var]->ATOMRECPOS[val];
  else
    current = VARLIST[var]->ATOMRECNEG[val];
  while(current)
    {
      if(!CLAUSELIST[current->c_num]->SAT)
	{
	  CLAUSELIST[current->c_num]->NumUnAss--;
	  if(equals)
	    VARLIST[var]->ATOMCNTPOS[val]--;
	  else
	    VARLIST[var]->ATOMCNTNEG[val]--;
	  if(CLAUSELIST[current->c_num]->NumUnAss == 1)
	    UNITLIST.push_front(current->c_num);
	  if(CLAUSELIST[current->c_num]->NumUnAss == 0)
	    {
	      CONFLICT = true;
	      CONFLICTINGCLAUSE = current->c_num;
	    }
	}
      current = current->next;
    }
  current = NULL;
}

//undoTheory
void Formula::undoTheory(int level)
{
  //for each variable v
  //for each domain d from dom(v) .. undo
  for(int i=1; i<VARLIST.size(); i++)
    {
      for(int j=0; j<VARLIST[i]->DOMAINSIZE; j++)
	{
	  //if this domain has been assigned at level this or greater undo
	  if(VARLIST[i]->ATOMLEVEL[j] > level)
	    {
	      if(VARLIST[i]->ATOMASSIGN[j] == -1)
		addLiteral(i, true, j);
	      else
		addLiteral(i, false, j);
	    }
	}
    }
  //for each variable v
  //for each domain d from dom(v) .. undo
  for(int i=1; i<VARLIST.size(); i++)
    {
      for(int j=0; j<VARLIST[i]->DOMAINSIZE; j++)
	{
	  //if this domain has been assigned at level this or greater undo
	  if(VARLIST[i]->ATOMLEVEL[j] > level)
	    {
	      if(VARLIST[i]->ATOMASSIGN[j] == -1)
		unsatisfyClauses(i, false, j, level);
	      else
		unsatisfyClauses(i, true, j, level);
	      VARLIST[i]->ATOMLEVEL[j] = -1;
	      VARLIST[i]->ATOMASSIGN[j] = 0;
	      VARLIST[i]->CLAUSEID[j] = -1;
	    }
	}
      if(VARLIST[i]->LEVEL > level)
	{
	  VARLIST[i]->LEVEL = -1;
	  VARLIST[i]->SAT = false;
	  VARLIST[i]->VAL = -1;
	}
    }
  //undo the decision stack
  int decsize = DECSTACK.size();
  for(int i=decsize-1; i>-1; i--)
    {
      if(VARLIST[DECSTACK[i]->VAR]->ATOMASSIGN[DECSTACK[i]->VAL] == 0)
	{
	  DECSTACK.erase(DECSTACK.begin()+i);
	}
    }
}

//unsatisfyClauses
inline void Formula::unsatisfyClauses(int var, bool equals, int val, int level)
{
  //cout<<"unsat .. "<<var<<(equals?"=":"!")<<val<<endl;
  VARRECORD * current = NULL;
  int lit_var = -1;
  bool lit_equal = false;
  int lit_val = -1;
  if(equals)
    current = VARLIST[var]->ATOMRECPOS[val];
  else
    current = VARLIST[var]->ATOMRECNEG[val];
  while(current)
    {
      if(CLAUSELIST[current->c_num]->LEVEL > level)
	{
	  for(int i=0; i<CLAUSELIST[current->c_num]->NumAtom; i++)
	    {
	      lit_var = CLAUSELIST[current->c_num]->ATOM_LIST[i]->VAR;
	      lit_equal = CLAUSELIST[current->c_num]->ATOM_LIST[i]->EQUAL;
	      lit_val = CLAUSELIST[current->c_num]->ATOM_LIST[i]->VAL;

	      if((VARLIST[lit_var]->ATOMLEVEL[lit_val] > level)
		 ||
		 (VARLIST[lit_var]->ATOMASSIGN[lit_val] == 0))
		{
		  CLAUSELIST[current->c_num]->NumUnAss++;
		  if(equals)
		    VARLIST[lit_var]->ATOMCNTPOS[lit_val]++;
		  else
		    VARLIST[lit_var]->ATOMCNTNEG[lit_val]++;
		}
	    }
	  CLAUSELIST[current->c_num]->LEVEL = -1;
	  CLAUSELIST[current->c_num]->SAT = false;
	}
      current = current->next;
    }
  current = NULL;
}

//addLiteral
inline void Formula::addLiteral(int var, bool equals, int val)
{
  cout<<"Add to partial interpretation: "<<var<<(equals?"=":"!")<<val<<endl;
  VARRECORD * current = NULL;
  //for every record of this literal increase the number of
  //unassigned literals from unsatisfied clauses
  if(equals)
    current = VARLIST[var]->ATOMRECPOS[val];
  else
    current = VARLIST[var]->ATOMRECNEG[val];
  while(current)
    {
      if(!CLAUSELIST[current->c_num]->SAT)
	{
	  CLAUSELIST[current->c_num]->NumUnAss++;
	  if(equals)
	    VARLIST[var]->ATOMCNTPOS[val]++;
	  else
	    VARLIST[var]->ATOMCNTNEG[val]++;
	}
      current = current->next;
    }
  current = NULL;
}

//anaylzeConflict: TODO FIX the mistake here: THE LEARNED CLAUSES ARE NOT entailed
// BY THE THEORY

int Formula::analyzeConflict()
{ cout << "Analyzing the conflict..."<< endl;

  //learnedClause
  Clause * learnedClause = new Clause();

  int CID = CONFLICTINGCLAUSE;
  int numLit = 0;
  int tlevel = LEVEL;
  int tvar = -1;
  int tval = -1;
  int tequal = false;
  int index = DECSTACK.size()-1; // vector of decisions/implications, ordered
  int csize = -1;
  int decsize = VARLIST.size();
/*
  for(int i=0; i<decsize; i++)
    {
      int dsize = VARLIST[i]->DOMAINSIZE;
      for(int j=0; j<dsize; j++)
	{
	  if(VARLIST[i]->ATOMLEVEL[j] != 0)
	    VARLIST[i]->FLAG[j] = true;
	  else
	    VARLIST[i]->FLAG[j] = true;
	}
} */


  while(1)
    {
      tvar = -1;
      tval = -1;
      tequal = false;
      if(CID > -1)
	{
	  csize = CLAUSELIST[CID]->NumAtom;
	  for(int i=0; i<csize; i++)
	    {
	      tvar = CLAUSELIST[CID]->ATOM_LIST[i]->VAR;
	      tval = CLAUSELIST[CID]->ATOM_LIST[i]->VAL;
	      tequal = CLAUSELIST[CID]->ATOM_LIST[i]->EQUAL;

	      if(!VARLIST[tvar]->FLAG[tval])
		{
		  VARLIST[tvar]->FLAG[tval] = true;

		  if((VARLIST[tvar]->VAL != -1) &&
		     (VARLIST[tvar]->CLAUSEID[tval] == VARLIST[tvar]->CLAUSEID[VARLIST[tvar]->VAL]))
		    VARLIST[tvar]->FLAG[VARLIST[tvar]->VAL] = true;
		  if(VARLIST[tvar]->ATOMLEVEL[tval] < tlevel)
		    {
		      if(tequal)
			learnedClause->AddAtom(new Literal(tvar, '=', tval));
		      else
			learnedClause->AddAtom(new Literal(tvar, '!', tval));
		      learnedClause->NumUnAss--;


		    }
		  else
		    {
		      numLit++;
		    }
		}
	    }
	}
      while((index > -1) && (!VARLIST[DECSTACK[index]->VAR]->FLAG[DECSTACK[index]->VAL]) && (CID != -1))
	{
	  cout<<"reducing index "<<index<<endl;
	  index--;
	}
      if(numLit == 1 || CID == -1)
	{
	  while(VARLIST[DECSTACK[index]->VAR]->ATOMLEVEL[DECSTACK[index]->VAL] != tlevel)
	    {
	      index++;
	    }
	  if(DECSTACK[index]->EQUAL)
	    learnedClause->AddAtom(new Literal(DECSTACK[index]->VAR, '!',
					       DECSTACK[index]->VAL));
	  else
	    learnedClause->AddAtom(new Literal(DECSTACK[index]->VAR, '=',
					       DECSTACK[index]->VAL));
	  learnedClause->NumUnAss--;
	  CLAUSELIST.push_back(learnedClause);
	  csize = learnedClause->NumAtom;
	  CID = CLAUSELIST.size()-1;
	  for(int i=0; i<csize; i++)
	    VARLIST[learnedClause->ATOM_LIST[i]->VAR]->AddRecord(CID,
								 learnedClause->ATOM_LIST[i]->VAL,
								 learnedClause->ATOM_LIST[i]->EQUAL);


	  //computing the backtrack level : CID
	  CID = 0;
        cout<< "learned a clause:"<<endl;
        learnedClause -> Print();
	  //if learned clause has only one literal then backtrack to level 0
	  if(csize == 1)
	    return CID;
	  for(int i=0; i<csize; i++)
	    {
	      if((tlevel != VARLIST[learnedClause->ATOM_LIST[i]->VAR]
		  ->ATOMLEVEL[learnedClause->ATOM_LIST[i]->VAL]) &&
		 (CID < VARLIST[learnedClause->ATOM_LIST[i]->VAR]
		  ->ATOMLEVEL[learnedClause->ATOM_LIST[i]->VAL]))
		CID = VARLIST[learnedClause->ATOM_LIST[i]->VAR]->ATOMLEVEL[learnedClause->ATOM_LIST[i]->VAL];
	    }
	  return CID;
	}
      else
	{
	  tvar = DECSTACK[index]->VAR;
	  tequal = DECSTACK[index]->EQUAL;
	  tval = DECSTACK[index]->VAL;
	  index--;
	  CID = VARLIST[tvar]->CLAUSEID[tval];
	}
      numLit--;
    }


  return 0;
}

//\\ UNIT PROPAGATION
bool Formula::unitPropagation()
{
  int lit_var = -1;
  bool lit_equal = false;
  int lit_val = -1;
  int unit_clause = -1;
  bool flag = false;

  //while there is no conflict and there exists a unit clause
  //find the unit literal and satisfy it
  while(!CONFLICT && !UNITLIST.empty())
    {
      UNITS++;
      unit_clause = UNITLIST.front();
      UNITCLAUSE = unit_clause;
      cout<<"Unit clause # : "<<unit_clause<<endl;
      UNITLIST.pop_front();
      if(!CLAUSELIST[unit_clause]->SAT)
	{
	  for(int i=0; i<CLAUSELIST[unit_clause]->NumAtom && !flag; i++)
	    {
	      lit_var = CLAUSELIST[unit_clause]->ATOM_LIST[i]->VAR;
	      lit_equal = CLAUSELIST[unit_clause]->ATOM_LIST[i]->EQUAL;
	      lit_val = CLAUSELIST[unit_clause]->ATOM_LIST[i]->VAL;
	      if(VARLIST[lit_var]->ATOMASSIGN[lit_val] == 0)
		{
		  flag = true;
		  //set the reason for this literal
		  VARLIST[lit_var]->CLAUSEID[lit_val] = unit_clause;
		}
	    }
	  if(flag)
	    {
	      cout<<"Unit : "<<lit_var<<(lit_equal?"=":"!=")<<lit_val<<endl;
	      reduceTheory(lit_var, lit_equal, lit_val);
	      flag = false;
	    }
	}
    }
  if(!CONFLICT)
    return true;
  else
    {
      cout<<"We reached a conflict in the clause #: "<<CONFLICTINGCLAUSE<<endl;
      UNITLIST.clear();
      return false;
    }
}

//\\============================================================================
//\\=================== NON-CHRONOLOGICAL BACKTRACK ============================
//\\============================================================================

int Formula::NonChronoBacktrack(int level)
{
  //start of finite domain extended dpll
  // return 0 : if theory satisfied
  // return 1 : if time out
  // return 2 : if CONFLICT and later used as unsatisfied

  //set LEVEL
  LEVEL = level;

  //Check if theory satisfied
  if(checkSat())
    return 0;

  //Check if time out
  TIME_E = GetTime();
  if((TIME_E - TIME_S) > TIMELIMIT)
    return 1;

  // If there is a unit clause, propagate
  if(!UNITLIST.empty())
    unitPropagation();

  //check if theory satisfied or not
  if(checkSat())
    return 0;

  //check if time out
  TIME_E = GetTime();
  if((TIME_E - TIME_S) > TIMELIMIT)
    return 1;

  //check if conflict
  if(CONFLICT)
    { cout << "There is a conflict at level: " << LEVEL << endl;
      if(LEVEL == 0)
	return 2;
      LEVEL = analyzeConflict();
      cout << "We are backtracking to the level: " << LEVEL << endl;
      BACKTRACKS++;
      CONFLICT = false;
      undoTheory(LEVEL);
      checkUnit();
      return NonChronoBacktrack(LEVEL);
    }

  //When there is no conflict, choose a literal to branch on
  Literal * atom = chooseLiteral();
  cout<<"Chose literal"<<endl;

  if(atom)
    {
      DECISIONS++;
      LEVEL++;
      cout<<"Decision : "<<atom->VAR<<(atom->EQUAL?"=":"!=")<<atom->VAL<<endl;
      reduceTheory(atom->VAR, atom->EQUAL, atom->VAL);
      return NonChronoBacktrack(LEVEL);
    }
  return 0;
}
//End Formula
//**********************************************************************//
