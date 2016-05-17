//**********************************************************************//
//
// Finite Domain Solver
// File : Formula.cc
//
//**********************************************************************//
//Including Libraries and Header files
#include "Formula.h"
#include "Clause.h"
#include <cstring>
#include <stdexcept>

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
  RESTARTS = 0;
  LOG = false;
}

//1-arg constructor
Formula::Formula(CommandLine * cline)
{
  //Setting appropriate values and resizing vectors
//  VARLIST.reserve(cline->NUM_VAR+1);
//  CLAUSELIST.reserve((int)(cline->CLAUSE_SIZE*1.2));
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
//  DECSTACK.reserve(cline->NUM_VAR+1);
  RESTARTS = 0;
  LOG = cline->LOG;
  // WATCH = cline->WATCH;
}

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
	/*  if((var != cline->NUM_VAR) || (val != cline->NUM_CLAUSE))
	    {
	      if (LOG) cout<<"**** ERROR ****"<<endl;
	      if (LOG) cout<<" Commandline arguments and file arguments do not match."<<endl;
	      if (LOG) cout<<endl;
	      exit(1);
	    } */
	}


      //else if line is d var# domsize
      else if(line_buffer[0] == 'd')
	{
	  temp = sscanf(line_buffer, "d %d %d", &var, &val);
	  temp_var = new Variable(var, val);
	  VARLIST.push_back(temp_var);
	}

/*  else if (line_buffer[0] == 'C') {
     CSP csp = new CSP();
     csp->alldifferent();
  } */
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
    if (cline -> WATCH) {
      temp_clause -> WATCHED[0] = temp_clause -> ATOM_LIST[0];
      if (temp_clause -> NumAtom > 1) {
      temp_clause -> WATCHED[1] = temp_clause -> ATOM_LIST[1];
    } else temp_clause -> WATCHED[1] = NULL;
  }
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

void Formula::PrintModel()
{
  for(int i=0; i<DECSTACK.size();i++)
  {
    if(DECSTACK[i]->EQUAL) DECSTACK[i]->Print();
  }
  if (LOG) cout<<endl;
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
  printf("Decisions   : %d\n", DECISIONS);
//  printf("Number of Units       : %d\n", UNITS);
  printf("Backtracks  : %d\n", BACKTRACKS);
  printf("Entails     : %d\n", ENTAILS);
//  printf("Number of Levels      : %d\n", LEVEL);
  printf("Variables   : %d\n", VARLIST.size()-1);
  printf("Clauses     : %d\n", CLAUSELIST.size());
  printf("Restarts   : %d\n", RESTARTS);

  printf("\n");
}

bool Formula::hasClause(Clause * clause){
  for(int i=0; i < CLAUSELIST.size(); i++){
    if(clause->ClauseisEqual(CLAUSELIST[i],clause)) return true;
  }
  return false;
}


//verifyModel
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

//checkSat
bool Formula::checkSat()
{  //if (LOG) cout<<"Checking satisfiability..."<<endl;
  int size = CLAUSELIST.size();
  for(int i=0; i<size; i++)
    {
      //if clause is not SAT return false
      if(!CLAUSELIST[i]->SAT) {
        //  if (LOG) cout<<"Unsatisfied clause # "<<i<<endl;
    // CLAUSELIST[i]->Print();
     return false; }
    }
  return true;
}

//checkConflict: returns id of a conflicting clause
int Formula::checkConflict()
{
  int size = CLAUSELIST.size();
  for(int i=0; i<size; i++)
    {
      //if clause is not SAT and has 0 unassigned literal
      //return index i
      if((CLAUSELIST[i]->NumUnAss == 0) &&
	 (!CLAUSELIST[i]->SAT)){
     CONFLICT = true;
    CONFLICTINGCLAUSE = i;
	return CONFLICTINGCLAUSE; }
    }
  return -1;
}

//checkUnit
void Formula::checkUnit()
{ //if (LOG) cout<<"Checking if there is a unit clause..."<<endl;
  int size = CLAUSELIST.size();
  UNITLIST.clear();
  for(int i=0; i<size; i++)
    {//if clause is not SAT and has 1 unassigned literal
      //add the index i into Unitlist
      if( (CLAUSELIST[i]->NumUnAss == 1 &&
	 !CLAUSELIST[i]->SAT ) ){
  // if (LOG) cout<<"Found a unit clause: "<<i<<endl;
	UNITLIST.push_back(i);
} else if ( CLAUSELIST[i]->NumUnAss == 0 && !CLAUSELIST[i]->SAT) CONFLICT = true;
}
}


//checkEntail
bool Formula::checkEntail(int var)
{  // if (LOG) cout<<"Checking entailment..."<<endl;

  int domainsize = 0;
  bool flag = false;
  int domainvalue = -1;

  //if variable not satisfied
  if(!VARLIST[var]->SAT)
    {// if (LOG) cout<<var<<" is not assigned"<<endl;
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
    //  if (LOG) cout<<"Entailment... "<<ENTAILLITERAL->VAR<<"="<<ENTAILLITERAL->VAL<<endl;

      return true;
    }
  return false;
}

//chooseLiteral
Literal * Formula::chooseLiteral()
{
  int max = -9999999999999999;
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
		  if(max < tmax)
		    {
		      max = tmax;
		      tvar = i;
		      tval = j;
		    }
		}
	    }
	}
    }
  return (tvar != -1 ? new Literal(tvar, tval) : NULL);
}

//reduceTheory
void Formula::reduceTheory(int var, bool equals, int val)
{

  if(equals)
    {
    //if (LOG) cout<<"Reducing literal: "<<var<<"="<<val<<" at level "<<LEVEL<<endl;
      //first satisfy all clauses with literal, and remove
      //negate literal from clasues
      satisfyClauses(var, equals, val);
      removeLiteral(var, !equals, val);
      VARLIST[var]->ATOMASSIGN[val] = 1;
      VARLIST[var]->ATOMLEVEL[val] = LEVEL;

      VARLIST[var]->VAL = val;
      VARLIST[var]->SAT = true; // means variable is assigned
      VARLIST[var]->LEVEL = LEVEL; // value assigned a positive value at this level

    VARLIST[var]->CLAUSEID[val] = UNITCLAUSE;
  //    if (LOG) cout<<"The reason for the literal: "<<endl;
    //  if (UNITCLAUSE > -1) CLAUSELIST[UNITCLAUSE]->Print(); else if (LOG) cout<<UNITCLAUSE<<endl;
      //Add literal to DecisionStack
      DECSTACK.push_back(new Literal(var, '=', val));

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
  //      if (LOG) cout<<"Set the reason for the literal "<<var<<(!equals?"=":"!")<<i<<endl;
    //    if (UNITCLAUSE > -1) CLAUSELIST[UNITCLAUSE]->Print(); else if (LOG) cout<<-1<<endl;

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
  //      if (LOG) cout<<"Set the reason for the literal "<<var<<(!equals?"=":"!")<<i<<endl;
      //  if (UNITCLAUSE > -1) CLAUSELIST[UNITCLAUSE]->Print(); else if (LOG) cout<<-1<<endl;


	    }
	}
    }
  else
    {
  // if (LOG) cout<<"Reducing: "<<var<<"!"<<val<<" at level "<<LEVEL<<endl;

      //first satisfy all clauses with negate literal, and remove
      //literal from claues
      satisfyClauses(var, equals, val);
      removeLiteral(var, !equals, val);
      VARLIST[var]->ATOMASSIGN[val] = -1;
      // reason and level should be assigned only once:

     // if(VARLIST[var]->ATOMLEVEL[val] == -1)
     VARLIST[var]->ATOMLEVEL[val] = LEVEL;
  //   if(VARLIST[var]->CLAUSEID[val] == -10)
     VARLIST[var]->CLAUSEID[val] = UNITCLAUSE;
      //Add literal to DecisionStack
  //    if (LOG) cout<<"Adding literal to the decision stack: "<<var<<"!"<<val<<endl;
      DECSTACK.push_back(new Literal(var, '!', val));

}
//check Entailment on this variable
if(checkEntail(var))
{
ENTAILS++;
//if (LOG) cout<<"Entailment... "<<ENTAILLITERAL->VAR<<"="<<ENTAILLITERAL->VAL<<endl;
UNITCLAUSE = -2;
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

//satisfyClauses
inline void Formula::watchedSatisfyLiteral(Literal * literal, int var, bool equals, int val)
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
      if(CLAUSELIST[current->c_num]->WATCHED[0]->SAT != 1)
	{ if(!LitisEqual(CLAUSELIST[current->c_num]->WATCHED[1], literal))
     CLAUSELIST[current->c_num]->WATCHED[0] = literal;
     else { CLAUSELIST[current->c_num]->WATCHED[1] == CLAUSELIST[current->c_num]->WATCHED[0];
     CLAUSELIST[current->c_num]->WATCHED[0] = literal;
   }
    }
  current = current->next;
}
}


//removeLiteral
inline void Formula::watchedFalsifyLiteral(Literal * literal, int var, bool equals, int val)
{
  VARRECORD * current = NULL;
  if(equals)
    current = VARLIST[var]->ATOMRECPOS[val];
  else
    current = VARLIST[var]->ATOMRECNEG[val];

      while(current)
        {
          if( CLAUSELIST[current->c_num]->WATCHED[0]->SAT != 1  &&
    	( LitisEqual(CLAUSELIST[current->c_num]->WATCHED[0], literal)
        || LitisEqual(CLAUSELIST[current->c_num]->WATCHED[1], literal)  ) ) SwapPointer(CLAUSELIST[current->c_num]);
        current = current->next;

        }
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
	  if(CLAUSELIST[current->c_num]->NumUnAss == 1){ //if (LOG) cout<<"adding unit clause: "<<current->c_num<<endl;
     UNITLIST.push_front(current->c_num);  }

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

	      VARLIST[i]->ATOMLEVEL[j] = -10;
	      VARLIST[i]->ATOMASSIGN[j] = 0;
	      VARLIST[i]->CLAUSEID[j] = -10;
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
  // if (LOG) cout<<"unsatisfy .. "<<var<<(equals?"=":"!")<<val<<endl;
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

//addLiteral : used when undoing the theory
inline void Formula::addLiteral(int var, bool equals, int val)
{
 // if (LOG) cout<<"add .. "<<var<<(equals?"=":"!")<<val<<endl;
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


bool Formula::HasAtom(Clause * clause, Literal * atom)
{
 for(int i=0; i<clause->NumAtom;i++){
   if (clause->ATOM_LIST[i]->VAR == atom->VAR && clause->ATOM_LIST[i]->EQUAL == atom->EQUAL && clause->ATOM_LIST[i]->VAL == atom->VAL) return true;
 }
 return false;
}

Clause * Formula::resolve(Clause * clause, Literal * literal, Clause * reason)
{
  Clause * resolvent = new Clause();
  // the literals from clause C that are satisfied by at least one interpretation that does not satisfy L
  for (int i=0; i<clause->ATOM_LIST.size();i++)
  {
    if (clause->ATOM_LIST[i]->VAR != literal->VAR ) resolvent->AddAtom(clause->ATOM_LIST[i]);

    else if (clause->ATOM_LIST[i]->VAL == literal->VAL && clause->ATOM_LIST[i]->EQUAL != literal->EQUAL) resolvent->AddAtom(clause->ATOM_LIST[i]);

    else if (clause->ATOM_LIST[i]->VAL != literal->VAL && clause->ATOM_LIST[i]->EQUAL == literal->EQUAL) resolvent->AddAtom(clause->ATOM_LIST[i]);

  }
// the literals from C that are satisfied by at least one interpretation that also satisfies L.
  for (int i=0; i<reason->ATOM_LIST.size();i++)
  {
   if (reason->ATOM_LIST[i]->VAR != literal->VAR && !HasAtom(resolvent,reason->ATOM_LIST[i])) resolvent->AddAtom(reason->ATOM_LIST[i]);

    else if (reason->ATOM_LIST[i]->VAL == literal->VAL && reason->ATOM_LIST[i]->EQUAL == literal->EQUAL && !HasAtom(resolvent,reason->ATOM_LIST[i])) resolvent->AddAtom(reason->ATOM_LIST[i]);

    else if (reason->ATOM_LIST[i]->VAL != literal->VAL && reason->ATOM_LIST[i]->EQUAL != literal->EQUAL && !HasAtom(resolvent,reason->ATOM_LIST[i])) resolvent->AddAtom(reason->ATOM_LIST[i]);

    else if (reason->ATOM_LIST[i]->VAL != literal->VAL && reason->ATOM_LIST[i]->EQUAL == literal->EQUAL && reason->ATOM_LIST[i]->EQUAL ==  false && !HasAtom(resolvent,reason->ATOM_LIST[i])) resolvent->AddAtom(reason->ATOM_LIST[i]);
  }

 return resolvent;
}


bool Formula::Potent(Clause * clause)
{ // TRYING TO FIX DECISION REASON PROBLEM
  int index = 0;
  int counter = 0;
  for (int i = 0; i < clause->NumAtom; i++)
  {
  if( VARLIST[clause->ATOM_LIST[i]->VAR]->ATOMLEVEL[clause->ATOM_LIST[i]->VAL] == LEVEL
// with this line potency counter == 0:
//  && VARLIST[clause->ATOM_LIST[i]->VAR]->CLAUSEID[clause->ATOM_LIST[i]->VAL] != -1
  ){  index = i; counter++; }
  }
  if (LOG) cout<<"Potency counter: "<<counter<<endl;
/* if((counter > 1) && VARLIST[clause->ATOM_LIST[index]->VAR]->CLAUSEID[clause->ATOM_LIST[index]->VAL] == -1){throw std::invalid_argument( "Decision reason, but counter is > 1" );}
  else */ if(counter != 1) return false;
  else {
     return true;}

}

int Formula::backtrackLevel(Clause * learnedClause)
{ // if (LOG) cout << "Finding backtrack level..."<<endl;
  int max = -1;
  int csize = learnedClause->NumAtom;


  //if learned clause has only one literal then backtrack to level 0
if(csize == 1)
  return 0;

  for(int i=0; i<csize; i++)
  {
  if((LEVEL > VARLIST[learnedClause->ATOM_LIST[i]->VAR]
  ->ATOMLEVEL[learnedClause->ATOM_LIST[i]->VAL]) &&
  (max < VARLIST[learnedClause->ATOM_LIST[i]->VAR]
  ->ATOMLEVEL[learnedClause->ATOM_LIST[i]->VAL]))
  max = VARLIST[learnedClause->ATOM_LIST[i]->VAR]->ATOMLEVEL[learnedClause->ATOM_LIST[i]->VAL];
  }
  if(max != -1) return max;
  else return LEVEL-1;
}

Literal * Formula::whyFalse(Literal * atom){
  for(int i=0; i<DECSTACK.size();i++){

    if(atom->VAR == DECSTACK[i]->VAR && atom->VAL == DECSTACK[i]->VAL && atom->EQUAL != DECSTACK[i]->EQUAL ) { return DECSTACK[i]; }
    else if(atom->VAR == DECSTACK[i]->VAR && atom->VAL != DECSTACK[i]->VAL && atom->EQUAL == DECSTACK[i]->EQUAL && atom->EQUAL) return DECSTACK[i];

  }
}

Clause * Formula::analyzeConflict(Clause * clause)
{


  if (Potent(clause)) {
   clause->NumUnAss = 0;
   if (LOG) { cout<<"Learned a clause: "<<endl;
   clause->Print(); }
  //  if (LOG) cout<<"Size of the clause: "<< clause->NumAtom<<endl;

    CLAUSELIST.push_back(clause);
    int csize = clause->NumAtom;
    int CID = CLAUSELIST.size()-1;
   for(int i=0; i<csize; i++) {
      VARLIST[clause->ATOM_LIST[i]->VAR]->AddRecord(CID,
                 clause->ATOM_LIST[i]->VAL,
                 clause->ATOM_LIST[i]->EQUAL); }

    return clause;
}

  // Resolve clause and reason of its latest falsified literal

 // if (LOG) cout<<"We are working with the clause: "<<endl;
//clause->Print();
  int csize = clause->NumAtom;
  //if (LOG) cout<<csize<<endl;
//  Literal * lastFalse = maxLit(clause); //Find latest falsified literal
  // Find the reason:
  Literal * lastFalse = maxLit(clause);
 //if (LOG) cout<<"Latest falsified literal: "<<endl;
//  max->Print();
 //if (LOG) cout<<"It's reason: "<<endl;
//  if (LOG) cout<<VARLIST[lastFalse->VAR]-> CLAUSEID[lastFalse->VAL]<<endl;
//  if(VARLIST[lastFalse->VAR]-> CLAUSEID[lastFalse->VAL] > -1) CLAUSELIST[VARLIST[lastFalse->VAR]-> CLAUSEID[lastFalse->VAL]] -> Print();
Clause * resolvent = new Clause();
  // dealing with decision and entail reasons:



  if(VARLIST[lastFalse->VAR]-> CLAUSEID[lastFalse->VAL] == -1 ){
  //  if (LOG) cout<<"Decision reason!"<<endl;
  //  lastFalse = maxLit(clause);
     Clause * chooseClause = new Clause();
      chooseClause -> AddAtom(new Literal(lastFalse->VAR,'=',lastFalse->VAL));
      chooseClause -> AddAtom(new Literal(lastFalse->VAR,'!',lastFalse->VAL));
      resolvent = resolve(clause,lastFalse,chooseClause);
  }

/*  if(VARLIST[lastFalse->VAR]-> CLAUSEID[lastFalse->VAL] == -1 ){

  // clause->NumUnAss = 0; //???
    CLAUSELIST.push_back(clause);
    int csize = clause->NumAtom;
    int CID = CLAUSELIST.size()-1;
   for(int i=0; i<csize; i++) {
      VARLIST[clause->ATOM_LIST[i]->VAR]->AddRecord(CID,
                 clause->ATOM_LIST[i]->VAL,
                 clause->ATOM_LIST[i]->EQUAL);
  }
  if (LOG) cout<<"Decision reason!"<<endl;

  return clause;

} */



  else if(VARLIST[lastFalse->VAR]-> CLAUSEID[lastFalse->VAL] == -2){
    // if (LOG) cout<<"Entail reason!"<<endl;
    Clause * entailClause = new Clause();
 for (int i=0; i < VARLIST[lastFalse->VAR]->DOMAINSIZE; i++){
    entailClause -> AddAtom(new Literal(lastFalse->VAR,'=',i)); }
    resolvent = resolve(clause,lastFalse,entailClause);
}
  else
  {  resolvent = resolve(clause,lastFalse,CLAUSELIST[VARLIST[lastFalse->VAR]-> CLAUSEID[lastFalse->VAL]]); }
  if (LOG) { cout<<"Resolvent:"<<endl;
  resolvent->Print(); }
 return analyzeConflict(resolvent);
}
//Finding the literal falsified the latest in the clause
Literal * Formula::maxLit(Clause * clause)
{ //if (LOG) cout<<"maxLit on: "<<endl;
//  clause->Print();
  int index = 0;
  int clindex = 0;
  int decisions = DECSTACK.size();

  for(int i=0; i<clause->NumAtom; i++)
  { Literal * current = clause->ATOM_LIST[i];
   for(int j=decisions-1; j<decisions && j > -1; j--)
    {
      if( ( (current->VAR == DECSTACK[j]->VAR && current->VAL == DECSTACK[j]->VAL && current->EQUAL != DECSTACK[j]->EQUAL)  || ( current->VAR == DECSTACK[j]->VAR && current->VAL != DECSTACK[j]->VAL && current->EQUAL == DECSTACK[j]->EQUAL && current->EQUAL )  )  && index <= j) { //TODO FIX
      index = j; clindex = i;
   }
  }

}
if (VARLIST[clause->ATOM_LIST[clindex]->VAR]->CLAUSEID[clause->ATOM_LIST[clindex]->VAL]==-1) if (LOG) cout<<"decision lit"<<endl;
if (LOG) { cout<<"maxLit: "<<endl;
clause->ATOM_LIST[clindex]->Print(); }
return clause->ATOM_LIST[clindex];

}

// Find unit literal in the unit clause
Literal * Formula::unitLiteral(Clause * unit)
{
  int lit_var = -1;
  bool lit_equal = false;
  int lit_val = -1;
//  bool flag = false;

if(unit->NumAtom == 1) return new Literal(unit->ATOM_LIST[0]->VAR,(unit->ATOM_LIST[0]->EQUAL?'=':'!'),unit->ATOM_LIST[0]->VAL);

for(int i=0; i<unit->NumAtom; i++)
  {
    lit_var = unit->ATOM_LIST[i]->VAR;
    lit_equal = unit->ATOM_LIST[i]->EQUAL;
    lit_val = unit->ATOM_LIST[i]->VAL;

    if (VARLIST[lit_var]->ATOMASSIGN[lit_val] == 0) {// if (LOG) cout<<"Unit literal is : "<<lit_var<<(lit_equal?"=":"!=")<<lit_val<<endl;
    return new Literal(lit_var,(lit_equal?'=':'!'),lit_val);

  } }

  UNITLIST.pop_front();
  if (!UNITLIST.empty()) unitLiteral(CLAUSELIST[UNITLIST.front()]);
  else return new Literal(-1,'!',-1);



}


// WATCHED LITERALS ALGORITHM
// WE HAVE TO MODIFY SEVERAL FUNCTIONS:
int Formula::watchedCheckSat(){
  for ( int i; i < CLAUSELIST.size(); i++ ){
    if ( CLAUSELIST[i] -> WATCHED[0] -> SAT == 0
      && ( CLAUSELIST[i] -> WATCHED[1] -> SAT == 0
        || CLAUSELIST[i] -> WATCHED[1] -> SAT == 2 ) ) return 0 ;
    else if ( CLAUSELIST[i] -> WATCHED[0] -> SAT == 0 ) return 2 ;
  }
  CONFLICT = true;
  return 1 ;
}

Literal * Formula::watchedCheckUnit(){
  for ( int i; i < CLAUSELIST.size(); i++ ){
    if ( CLAUSELIST[i] -> WATCHED[0] -> SAT == 2
       && ( CLAUSELIST[i] -> WATCHED[1] == NULL
      || CLAUSELIST[i] -> WATCHED[1] -> SAT == 2 ) )
      return CLAUSELIST[i] -> WATCHED[0];

    else if ( CLAUSELIST[i] -> WATCHED[0] -> SAT == 0
      &&  CLAUSELIST[i] -> WATCHED[1] -> SAT == 2 )
      return CLAUSELIST[i] -> WATCHED[1];
}
return NULL;
  }


Literal * Formula::watchedChooseLiteral(){
  for ( int i; i < CLAUSELIST.size(); i++ ){
    if ( CLAUSELIST[i] -> WATCHED[0] -> SAT == 2) return  CLAUSELIST[i] -> WATCHED[0];
  }
}
//add new function to swap pointers
void Formula::SwapPointer(Clause * clause){

  if (clause -> WATCHED[1] -> SAT == 2){
    for (int i; i < clause -> NumAtom; i++){
      if ( clause -> ATOM_LIST[i] -> SAT == 2 && !LitisEqual(clause-> ATOM_LIST[i], clause -> WATCHED[1]))
      clause -> WATCHED[0] =  clause-> ATOM_LIST[i]; }
    }
  else if (clause -> WATCHED[0] -> SAT == 2)
  {

    for (int i; i < clause -> NumAtom; i++){
      if ( clause-> ATOM_LIST[i] -> SAT == 2 && !LitisEqual(clause-> ATOM_LIST[i], clause -> WATCHED[0]))
      clause -> WATCHED[1] =  clause-> ATOM_LIST[i]; }

  }

  }

bool Formula::LitisEqual(Literal * literal1, Literal * literal2){
  if (literal1 -> VAR == literal2 -> VAR && literal1 -> VAL == literal2 -> VAL
    && literal1 -> EQUAL == literal2 -> EQUAL) return true;
  else return false;
}

void Formula::watchedReduceTheory(Literal * literal, int var, bool equals, int val){

  if(equals)
    {
    if (LOG) cout<<"Reducing literal: "<<var<<"="<<val<<" at level "<<LEVEL<<endl;
      //first satisfy all clauses with literal, and remove
      //negate literal from clasues
      //now keep track of the watched literals
      watchedSatisfyLiteral(literal,var, equals, val);
      watchedFalsifyLiteral(literal, var, !equals, val);
      VARLIST[var]->ATOMASSIGN[val] = 1;
      VARLIST[var]->ATOMLEVEL[val] = LEVEL;

      VARLIST[var]->VAL = val;
      VARLIST[var]->SAT = true; // means variable is assigned REDUNDANT!
      VARLIST[var]->LEVEL = LEVEL; // value assigned a positive value at this level

    VARLIST[var]->CLAUSEID[val] = UNITCLAUSE;
  //    if (LOG) cout<<"The reason for the literal: "<<endl;
    //  if (UNITCLAUSE > -1) CLAUSELIST[UNITCLAUSE]->Print(); else if (LOG) cout<<UNITCLAUSE<<endl;
      //Add literal to DecisionStack
      DECSTACK.push_back(new Literal(var, '=', val));

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
  //      if (LOG) cout<<"Set the reason for the literal "<<var<<(!equals?"=":"!")<<i<<endl;
    //    if (UNITCLAUSE > -1) CLAUSELIST[UNITCLAUSE]->Print(); else if (LOG) cout<<-1<<endl;

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
  //      if (LOG) cout<<"Set the reason for the literal "<<var<<(!equals?"=":"!")<<i<<endl;
      //  if (UNITCLAUSE > -1) CLAUSELIST[UNITCLAUSE]->Print(); else if (LOG) cout<<-1<<endl;


      }
  }
    }
  else
    {
  // if (LOG) cout<<"Reducing: "<<var<<"!"<<val<<" at level "<<LEVEL<<endl;

      //first satisfy all clauses with negate literal, and remove
      //literal from claues
      satisfyClauses(var, equals, val);
      removeLiteral(var, !equals, val);
      VARLIST[var]->ATOMASSIGN[val] = -1;
      // reason and level should be assigned only once:

     // if(VARLIST[var]->ATOMLEVEL[val] == -1)
     VARLIST[var]->ATOMLEVEL[val] = LEVEL;
  //   if(VARLIST[var]->CLAUSEID[val] == -10)
     VARLIST[var]->CLAUSEID[val] = UNITCLAUSE;
      //Add literal to DecisionStack
  //    if (LOG) cout<<"Adding literal to the decision stack: "<<var<<"!"<<val<<endl;
      DECSTACK.push_back(new Literal(var, '!', val));

  }
  //check Entailment on this variable
  if(checkEntail(var))
  {
  ENTAILS++;
  //if (LOG) cout<<"Entailment... "<<ENTAILLITERAL->VAR<<"="<<ENTAILLITERAL->VAL<<endl;
  UNITCLAUSE = -2;
  reduceTheory(ENTAILLITERAL->VAR, true, ENTAILLITERAL->VAL);

  }
}
//\\====================Watched literals NON-CHRONOLOGICAL BACKTRACK=============================

int Formula::WatchedLiterals(){
  while(true){
    // if (LOG) cout<<"Number of clauses so far: "<<CLAUSELIST.size()<<endl;
    //Check if theory satisfied
    if(watchedCheckSat() == 1)
      return 0; //

    //Check if time out
   TIME_E = GetTime();
   if((TIME_E - TIME_S) > TIMELIMIT)
     return 1;

    //check if conflict
    if(CONFLICT)
      { if (LOG) cout << "There is a conflict at level: " << LEVEL << endl;
       if (LOG) { cout<<"Conflicting clause: "<<  endl;
       CLAUSELIST[CONFLICTINGCLAUSE] -> Print();}

        if(LEVEL == 0) return 2;

        LEVEL = backtrackLevel(analyzeConflict(CLAUSELIST[CONFLICTINGCLAUSE]));
        if (LOG) cout << "We are backtracking to the level: " << LEVEL << endl;
        BACKTRACKS++;
        if (LOG) cout << "# of backtracks so far: "<<BACKTRACKS<<endl;
        CONFLICT = false;
        undoTheory(LEVEL);
      }
      // If there is a unit clause, propagate

      if(watchedCheckUnit() != NULL)
        unitPropagation();

      // otherwise choose a literal and propagate - no need for separate unit propagation
  if(!CONFLICT)
  {

    Literal * atom = watchedChooseLiteral();
    if(atom)
      {
        DECISIONS++;
        LEVEL++;
        if (LOG) cout<<"Decision: "<<atom->VAR<<(atom->EQUAL?'=':'!')<<atom->VAL<<endl;
        UNITCLAUSE = -1; // REASON for subsequent falsified atoms
        atom -> SAT = 1;
        atom -> LEVEL = LEVEL;
        watchedReduceTheory(atom, atom->VAR, atom->EQUAL, atom->VAL);
    }

  }
  }

}




// with restarts
int Formula::NonChronoBacktrack(int restarts)
{ int restartCount = restarts;
  //int RESTARTS = 0;
  // LEVEL = 0;
  //start of finite domain extended dpll
  // return 0 : if theory satisfied
  // return 1 : if time out
  // return 2 : if CONFLICT and later used as unsatisfied
while(true){
  // if (LOG) cout<<"Number of clauses so far: "<<CLAUSELIST.size()<<endl;
  //Check if theory satisfied
  if(checkSat())
    return 0; // add PrintModel(); - from DECSTACK
  //Check if time out
 TIME_E = GetTime();
 if((TIME_E - TIME_S) > TIMELIMIT)
   return 1;
  //check if conflict

  if(CONFLICT)
    { if (LOG) cout << "There is a conflict at level: " << LEVEL << endl;
     if (LOG) { cout<<"Conflicting clause: "<<  endl;
     CLAUSELIST[CONFLICTINGCLAUSE] -> Print(); }

      if(LEVEL == 0) return 2;

      LEVEL = backtrackLevel(analyzeConflict(CLAUSELIST[CONFLICTINGCLAUSE]));
      if (LOG) cout << "We are backtracking to the level: " << LEVEL << endl;
      BACKTRACKS++;
      if (LOG) cout << "# of backtracks so far: "<<BACKTRACKS<<endl;
      CONFLICT = false;
      if(BACKTRACKS == restartCount) {
        undoTheory(0);
        restartCount = BACKTRACKS + restarts; RESTARTS++;
      }
      else undoTheory(LEVEL);
    }
    // If there is a unit clause, propagate

    checkUnit();
    if(!UNITLIST.empty())
      unitPropagation();

    // otherwise choose a literal and propagate - no need for separate unit propagation
if(!CONFLICT)
{

  Literal * atom = chooseLiteral();
  if(atom)
    {
      DECISIONS++;
      LEVEL++;
      if (LOG) cout<<"Decision: "<<atom->VAR<<(atom->EQUAL?'=':'!')<<atom->VAL<<endl;
      UNITCLAUSE = -1; // REASON for subsequent falsified atoms
      reduceTheory(atom->VAR, atom->EQUAL, atom->VAL);
  }

}
}


}


int Formula::NonChronoBacktrack()
{ //int restartCount = restarts;
  //int RESTARTS = 0;
  // LEVEL = 0;
  //start of finite domain extended dpll
  // return 0 : if theory satisfied
  // return 1 : if time out
  // return 2 : if CONFLICT and later used as unsatisfied
while(true){
  // if (LOG) cout<<"Number of clauses so far: "<<CLAUSELIST.size()<<endl;
  //Check if theory satisfied
  if(checkSat())
    return 0; // add PrintModel(); - from DECSTACK
  //Check if time out
 TIME_E = GetTime();
 if((TIME_E - TIME_S) > TIMELIMIT)
   return 1;
  //check if conflict

  if(CONFLICT)
    { if (LOG) cout << "There is a conflict at level: " << LEVEL << endl;
     if (LOG) { cout<<"Conflicting clause: "<<  endl;
     CLAUSELIST[CONFLICTINGCLAUSE] -> Print();}

      if(LEVEL == 0) return 2;

      LEVEL = backtrackLevel(analyzeConflict(CLAUSELIST[CONFLICTINGCLAUSE]));
      if (LOG) cout << "We are backtracking to the level: " << LEVEL << endl;
      BACKTRACKS++;
      if (LOG) cout << "# of backtracks so far: "<<BACKTRACKS<<endl;
      CONFLICT = false;
      undoTheory(LEVEL);
    }
    // If there is a unit clause, propagate

    checkUnit();
    if(!UNITLIST.empty())
      unitPropagation();

    // otherwise choose a literal and propagate - no need for separate unit propagation
if(!CONFLICT)
{

  Literal * atom = chooseLiteral();
  if(atom)
    {
      DECISIONS++;
      LEVEL++;
      if (LOG) cout<<"Decision: "<<atom->VAR<<(atom->EQUAL?'=':'!')<<atom->VAL<<endl;
      UNITCLAUSE = -1; // REASON for subsequent falsified atoms
      reduceTheory(atom->VAR, atom->EQUAL, atom->VAL);
  }

}
}


}


int Formula::ChronoBacktrack(int level)
{
  //start of finite domain extended dpll
  // return 0 : if theory satisfied
  // return 1 : if time out
  // return 2 : if conflict and later used as unsatisfied

  //set LEVEL
  LEVEL = level;

  //check if theory satisfied or not
  if(checkSat())
    return 0;

  //check if time out
  TIME_E = GetTime();
  if((TIME_E - TIME_S) > TIMELIMIT)
    return 1;

  //check unit literal
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
    {
      BACKTRACKS++;
      CONFLICT = false;
      undoTheory(LEVEL-1);
      LEVEL = LEVEL-1;
      return 2;
    }

  //since all is fine, now need to choose a literal
  //to branch on
  Literal * atom = chooseLiteral();
  if(atom)
    {
      DECISIONS++;
      LEVEL++;
      reduceTheory(atom->VAR, atom->EQUAL, atom->VAL);
      int result = ChronoBacktrack(LEVEL);
      if(result == 0)
	return 0;
      else if(result == 1)
	return 1;
      else if((result == 2) && (LEVEL != 0))
	{
	  reduceTheory(atom->VAR, !atom->EQUAL, atom->VAL);
	  return ChronoBacktrack(LEVEL);
	}
      else
	return 2;
    }
  else
    {
       cout<<"No Branch Atom selected"<<endl;
    }
  return 0;
}

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
      //if (LOG) cout<<"unit c : "<<unit_clause<<endl;
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
	    // if (LOG) cout<<"Reducing on unit literal: "<<lit_var<<(lit_equal?"=":"!=")<<lit_val<<endl;
	      reduceTheory(lit_var, lit_equal, lit_val);
	      flag = false;
	    }
	}
    }
  if(!CONFLICT)
    return true;
  else
    {
    //  if (LOG) cout<<"Conflict : "<<CONFLICTINGCLAUSE<<endl;
      UNITLIST.clear();
      return false;
    }
}


//End Formula
