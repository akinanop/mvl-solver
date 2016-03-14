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
  REASON = NULL;
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
  REASON = NULL;
//  ORIGINALFORMULA = cline->NUM_CLAUSE;
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
//  printf("Number of Units       : %d\n", UNITS);
  printf("Number of Backtracks  : %d\n", BACKTRACKS);
  printf("Number of Entails     : %d\n", ENTAILS);
//  printf("Number of Levels      : %d\n", LEVEL);
  printf("Number of Variables   : %d\n", VARLIST.size()-1);
  printf("Number of Clauses     : %d\n", CLAUSELIST.size());
  printf("\n");
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
{ // cout<<"Checking satisfiability..."<<endl;
  int size = CLAUSELIST.size();
  for(int i=0; i<size; i++)
    {
      //if clause is not SAT return false
      if(!CLAUSELIST[i]->SAT) {	  // cout<<"Unsatisfied clause # "<<i<<endl;
    //  CLAUSELIST[i]->Print();
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
{ //cout<<"Checking if there is a unit clause..."<<endl;
  int size = CLAUSELIST.size();
//  UNITLIST.clear();
  for(int i=0; i<size; i++)
    {//if clause is not SAT and has 1 unassigned literal
      //add the index i into Unitlist
      if( (CLAUSELIST[i]->NumUnAss == 1 &&
	 !CLAUSELIST[i]->SAT ) || CLAUSELIST[i] -> NumAtom == 1 ){
    //   cout<<"Found a unit clause: "<<i<<endl;
	UNITLIST.push_back(i);
} else if ( CLAUSELIST[i]->NumUnAss == 0 && !CLAUSELIST[i]->SAT) CONFLICT = true;
}
}


//checkEntail
bool Formula::checkEntail(int var)
{  // cout<<"Checking entailment..."<<endl;

/*  if(ENTAILLITERAL){
    delete ENTAILLITERAL;
    cout<<"Successfully freed entailment var"<<endl; } */

  int domainsize = 0;
  bool flag = false;
  int domainvalue = -1;

  //if variable not satisfied
  if(!VARLIST[var]->SAT)
    {// cout<<var<<" is not assigned"<<endl;
      domainsize = VARLIST[var]->DOMAINSIZE;
      //check its domain and see that exactly one
      //domain value has atomassign 0, if more than one
      //then there is no entailment
      for(int j=0; j<domainsize; j++)
	{
	  if(VARLIST[var]->ATOMASSIGN[j] == 0)
	    { // cout<<var<<" is not assigned on "<<j<<endl;
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
    //  cout<<"Entailment... "<<ENTAILLITERAL->VAR<<"="<<ENTAILLITERAL->VAL<<endl;

      return true;
    }
  return false;
}

//checkPureLiteral
/*bool Formula::checkPureLiteral()
{
  return false;
} */

//chooseLiteral
Literal * Formula::chooseLiteral()
{
  int max = -99999999999999;
  int size = VARLIST.size();
  int domainsize = -1;
  int tvar = -1;
  int tval = -1;
  int tmax = -1;

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

 if (tvar != -1){
  //  cout<<"Decision literal: "<<tvar<<"="<<tval<<endl;

  /*  Clause * chooseClause = new Clause();
    chooseClause->NumUnAss = 0;
    chooseClause->SAT;
    chooseClause -> LEVEL = LEVEL;
    chooseClause -> AddAtom(new Literal(tvar,'=',tval));
    chooseClause -> AddAtom(new Literal(tvar,'!',tval));
    CLAUSELIST.push_back(chooseClause);
    int csize = chooseClause->NumAtom;
    int CID = CLAUSELIST.size()-1;
    for(int i=0; i<csize; i++){
      VARLIST[chooseClause->ATOM_LIST[i]->VAR]->AddRecord(CID,
                 chooseClause->ATOM_LIST[i]->VAL,
                 chooseClause->ATOM_LIST[i]->EQUAL); }
    // cout<<"Clause list: "<<CLAUSELIST.size()<<endl;
    VARLIST[tvar] -> CLAUSEID[tval] = CLAUSELIST.size()-1;
    UNITCLAUSE = VARLIST[tvar] -> CLAUSEID[tval];
    cout<<"Setting the reason for the decision literal "<<tvar<<"="<<tval<<endl;
    chooseClause -> Print(); */
    UNITCLAUSE = -1;
    return new Literal(tvar, tval); } else return NULL;
}
//reduceTheory
void Formula::reduceTheory(int var, bool equals, int val)
{

  if(equals)
    {
    //cout<<"Reducing literal: "<<var<<"="<<val<<" at level "<<LEVEL<<endl;
      //first satisfy all clauses with literal, and remove
      //negate literal from clasues
      satisfyClauses(var, equals, val);
      removeLiteral(var, !equals, val);
      VARLIST[var]->ATOMASSIGN[val] = 1;
      if(VARLIST[var]->ATOMLEVEL[val] == -1) VARLIST[var]->ATOMLEVEL[val] = LEVEL;

      VARLIST[var]->VAL = val;
      VARLIST[var]->SAT = true; // means variable is assigned
      VARLIST[var]->LEVEL = LEVEL;
      // VARLIST[var]->REASONS[val] = REASON;
    if(VARLIST[var]->CLAUSEID[val] == -10) VARLIST[var]->CLAUSEID[val] = UNITCLAUSE;
  //    cout<<"The reason for the literal: "<<endl;
    //  if (UNITCLAUSE > -1) CLAUSELIST[UNITCLAUSE]->Print(); else cout<<UNITCLAUSE<<endl;
      //Add literal to DecisionStack
    // cout<<"Size of the decision stack: "<<DECSTACK.size()<<endl;

      DECSTACK.push_back(new Literal(var, '=', val));
    //  cout<<"Size of the decision stack: "<<DECSTACK.size()<<endl;

    //  cout<<"Adding literal to the decision stack: "<<var<<"="<<val<<endl;
  //    cout<<"Size of the decision stack: "<<DECSTACK.size()<<endl;
      //foreach domain value x from dom(v) which is not assigned
      //assign it

      for(int i=0; i<val && !CONFLICT; i++)
	{

	  if(VARLIST[var]->ATOMASSIGN[i] == 0)
	    {
	      satisfyClauses(var, !equals, i);
	      removeLiteral(var, equals, i);
	      VARLIST[var]->ATOMASSIGN[i] = -1;
	      if(VARLIST[var]->ATOMLEVEL[i] == -1) VARLIST[var]->ATOMLEVEL[i] = LEVEL;
	     if(VARLIST[var]->CLAUSEID[val] == -10) VARLIST[var]->CLAUSEID[i] = UNITCLAUSE;
  //      cout<<"Set the reason for the literal "<<var<<(!equals?"=":"!")<<i<<endl;
    //    if (UNITCLAUSE > -1) CLAUSELIST[UNITCLAUSE]->Print(); else cout<<-1<<endl;
  //  cout<<"Size of the decision stack: "<<DECSTACK.size()<<endl;

      DECSTACK.push_back(new Literal(var, '!', i));

  //  cout<<"Adding literal to the decision stack: "<<var<<"!"<<i<<endl;
  //  cout<<"Size of the decision stack: "<<DECSTACK.size()<<endl;
	    }
	}
      for(int i=val+1; i<VARLIST[var]->DOMAINSIZE && !CONFLICT; i++)
	{
	  if(VARLIST[var]->ATOMASSIGN[i] == 0)
	    {
	      satisfyClauses(var, !equals, i);
	      removeLiteral(var, equals, i);
	      if(VARLIST[var]->ATOMLEVEL[i] == -1) VARLIST[var]->ATOMASSIGN[i] = -1;
	      VARLIST[var]->ATOMLEVEL[i] = LEVEL;
	      if(VARLIST[var]->CLAUSEID[val] == -10) VARLIST[var]->CLAUSEID[i] = UNITCLAUSE;
  //      cout<<"Set the reason for the literal "<<var<<(!equals?"=":"!")<<i<<endl;
      //  if (UNITCLAUSE > -1) CLAUSELIST[UNITCLAUSE]->Print(); else cout<<-1<<endl;
      DECSTACK.push_back(new Literal(var, '!', i));
  //  cout<<"Adding literal to the decision stack: "<<var<<"!"<<i<<endl;
//    cout<<"Size of the decision stack: "<<DECSTACK.size()<<endl;
	    }
	}



    }
  else
    {
  // cout<<"Reducing: "<<var<<"!"<<val<<" at level "<<LEVEL<<endl;
      //first satisfy all clauses with negate literal, and remove
      //literal from claues
      satisfyClauses(var, equals, val);
      removeLiteral(var, !equals, val);
      VARLIST[var]->ATOMASSIGN[val] = -1;
     if(VARLIST[var]->ATOMLEVEL[val] == -1) VARLIST[var]->ATOMLEVEL[val] = LEVEL;
     if(VARLIST[var]->CLAUSEID[val] == -10)  VARLIST[var]->CLAUSEID[val] = UNITCLAUSE;
      //Add literal to DecisionStack
  //    cout<<"Adding literal to the decision stack: "<<var<<"!"<<val<<endl;
      DECSTACK.push_back(new Literal(var, '!', val));


}
//check Entailment on this variable
if(checkEntail(var))
{
ENTAILS++;
//cout<<"Entailment... "<<ENTAILLITERAL->VAR<<"="<<ENTAILLITERAL->VAL<<endl;
/* Clause * entailClause = new Clause();
entailClause -> LEVEL = LEVEL;
for (int i=0; i < VARLIST[ENTAILLITERAL->VAR]->DOMAINSIZE; i++){
entailClause -> AddAtom(new Literal(ENTAILLITERAL->VAR,'=',i));
}
CLAUSELIST.push_back(entailClause);

/*int csize = entailClause->NumAtom;
int CID = CLAUSELIST.size()-1;
for(int i=0; i<csize; i++){
VARLIST[entailClause->ATOM_LIST[i]->VAR]->AddRecord(CID,
         entailClause->ATOM_LIST[i]->VAL,
         entailClause->ATOM_LIST[i]->EQUAL); } */

// VARLIST[ENTAILLITERAL->VAR]->CLAUSEID[ENTAILLITERAL->VAL] = -2;
//cout<<"Setting reason for the entailed literal "<<ENTAILLITERAL->VAR<<"="<<ENTAILLITERAL->VAL<<": "<<endl;
//// REASON = entailClause;
// CLAUSELIST[UNITCLAUSE]->Print();
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

//addLiteral : used when undoing the theory
inline void Formula::addLiteral(int var, bool equals, int val)
{
//  cout<<"add .. "<<var<<(equals?"=":"!")<<val<<endl;
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
  Clause * resolvent = new Clause(clause->NumAtom + reason->NumAtom);
  resolvent->LEVEL = LEVEL;
  // adding lit in clause s.t. lit not \entails literal
  for (int i=0; i<clause->ATOM_LIST.size();i++)
  {
    if (clause->ATOM_LIST[i]->VAR != literal->VAR ) resolvent->AddAtom(clause->ATOM_LIST[i]);
    else if (clause->ATOM_LIST[i]->VAL != literal->VAL && clause->ATOM_LIST[i]->EQUAL == literal->EQUAL) resolvent->AddAtom(clause->ATOM_LIST[i]);
    else if (clause->ATOM_LIST[i]->VAL == literal->VAL && clause->ATOM_LIST[i]->EQUAL != literal->EQUAL) resolvent->AddAtom(clause->ATOM_LIST[i]);
    else if (clause->ATOM_LIST[i]->VAL != literal->VAL && clause->ATOM_LIST[i]->EQUAL != literal->EQUAL && !clause->ATOM_LIST[i]->EQUAL) resolvent->AddAtom(clause->ATOM_LIST[i]);

  }
// adding lit in reason s.t. lit not \falsifies literal - compatible
  for (int i=0; i<reason->ATOM_LIST.size();i++)
  {
    if (reason->ATOM_LIST[i]->VAR != literal->VAR && !HasAtom(resolvent,reason->ATOM_LIST[i])) resolvent->AddAtom(reason->ATOM_LIST[i]);

    else if (reason->ATOM_LIST[i]->VAL != literal->VAL && reason->ATOM_LIST[i]->EQUAL != literal->EQUAL && !HasAtom(resolvent,reason->ATOM_LIST[i])) resolvent->AddAtom(reason->ATOM_LIST[i]);

    else if (reason->ATOM_LIST[i]->VAL == literal->VAL && reason->ATOM_LIST[i]->EQUAL == literal->EQUAL && !HasAtom(resolvent,reason->ATOM_LIST[i])) resolvent->AddAtom(reason->ATOM_LIST[i]);

    else if (reason->ATOM_LIST[i]->VAL != literal->VAL && reason->ATOM_LIST[i]->EQUAL == literal->EQUAL && reason->ATOM_LIST[i]->EQUAL ==  false && !HasAtom(resolvent,reason->ATOM_LIST[i])) resolvent->AddAtom(reason->ATOM_LIST[i]);
  }
 //cout<<"Resolvent: "<<endl;
 //resolvent->Print();

 return resolvent;
}


bool Formula::Potent(Clause * clause)
{
  int counter = 0;
  for (int i = 0; i < clause->NumAtom; i++)
  { //  if( VARLIST[whyFalse(clause->ATOM_LIST[i])->VAR]->ATOMLEVEL[whyFalse(clause->ATOM_LIST[i])->VAL] == LEVEL && VARLIST[clause->ATOM_LIST[i]->VAR]->FLAG[clause->ATOM_LIST[i]->VAL] == false ) counter++;
  //  if (VARLIST[clause->ATOM_LIST[i]->VAR]->LEVEL == LEVEL) counter++;
  if( VARLIST[clause->ATOM_LIST[i]->VAR]->ATOMLEVEL[clause->ATOM_LIST[i]->VAL] == LEVEL && VARLIST[clause->ATOM_LIST[i]->VAR]->FLAG[clause->ATOM_LIST[i]->VAL] == false && VARLIST[clause->ATOM_LIST[i]->VAR]->CLAUSEID[clause->ATOM_LIST[i]->VAL] != -1){
  //  cout<<"Increasing counter on "<< clause->ATOM_LIST[i]->VAR<<" "<<clause->ATOM_LIST[i]->VAL<<endl;

    counter++;

  }
  }

  if(counter != 1) return false;
  else return true;

}

int Formula::backtrackLevel(Clause * learnedClause)
{
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
{       //flag satisfied literals:
  for(int i=0; i<clause->NumAtom; i++)
  { Literal * current = clause->ATOM_LIST[i];
  //  current->Print();
   for(int j=0; j<DECSTACK.size(); j++)
    { if(current->VAR == DECSTACK[j]->VAR && current->VAL == DECSTACK[j]->VAL && current->EQUAL == DECSTACK[j]->EQUAL) VARLIST[current->VAR]->FLAG[current->VAL] = true;
    //  else VARLIST[current->VAR]->FLAG[current->VAL] = false;
     }
 }


  if (Potent(clause) || clause->NumAtom == 1) {
  //   clause->NumUnAss = 1;
  //  cout<<"Learned a clause: "<<endl;
   clause->Print();
    CLAUSELIST.push_back(clause);
    int csize = clause->NumAtom;
    int CID = CLAUSELIST.size()-1;
   for(int i=0; i<csize; i++)
      VARLIST[clause->ATOM_LIST[i]->VAR]->AddRecord(CID,
                 clause->ATOM_LIST[i]->VAL,
                 clause->ATOM_LIST[i]->EQUAL);
    UNITLIST.push_front(CLAUSELIST.size()-1);
    return clause;
}

  // Resolve clause and reason of its latest falsified literal

// cout<<"We are working with the clause: "<<endl;
// clause->Print();
  int csize = clause->NumAtom;
  //cout<<csize<<endl;
//  Literal * lastFalse = maxLit(clause); //Find latest falsified literal
  // Find the reason:
  Literal * max = maxLit(clause);
//  cout<<"Latest falsified literal: "<<endl;
//  max->Print();
  Literal * lastFalse = whyFalse(max);
//  cout<<"Earliest contradicting literal: "<<endl;
  lastFalse->Print();
// cout<<"It's reason: "<<endl;
  cout<<VARLIST[lastFalse->VAR]-> CLAUSEID[lastFalse->VAL]<<endl;
  if(VARLIST[lastFalse->VAR]-> CLAUSEID[lastFalse->VAL] > -1) CLAUSELIST[VARLIST[lastFalse->VAR]-> CLAUSEID[lastFalse->VAL]] -> Print();
Clause * resolvent = new Clause();
  // dealing with decision and entail reasons:
  if(VARLIST[lastFalse->VAR]-> CLAUSEID[lastFalse->VAL] == -1){

     Clause * chooseClause = new Clause();
      // chooseClause->NumUnAss = 0;
      // chooseClause->SAT;
      // chooseClause -> LEVEL = LEVEL;
      chooseClause -> AddAtom(new Literal(lastFalse->VAR,'=',lastFalse->VAL));
      chooseClause -> AddAtom(new Literal(lastFalse->VAR,'!',lastFalse->VAL));
      resolvent = resolve(clause,max,chooseClause);
  }
  else if(VARLIST[lastFalse->VAR]-> CLAUSEID[lastFalse->VAL] == -2){
    Clause * entailClause = new Clause();
    //entailClause -> LEVEL = LEVEL;
/*    for (int i=0; i < VARLIST[lastFalse->VAR]->DOMAINSIZE; i++){
    entailClause -> AddAtom(new Literal(lastFalse->VAR,'=',i));
  } */
  entailClause -> AddAtom(new Literal(lastFalse->VAR,'=',lastFalse->VAL));
  entailClause -> AddAtom(new Literal(lastFalse->VAR,'!',lastFalse->VAL));

    resolvent = resolve(clause,max,entailClause);
  }
  else
  {  resolvent = resolve(clause,max,CLAUSELIST[VARLIST[lastFalse->VAR]-> CLAUSEID[lastFalse->VAL]]); }

//   cout<<"Resolvent:"<<endl;
//   resolvent->Print();
   if (resolvent->ClauseisEqual(resolvent,clause)) {

//     cout<<"Learned a clause: "<<endl;
    clause->Print();
     CLAUSELIST.push_back(clause);
     int csize = clause->NumAtom;
     int CID = CLAUSELIST.size()-1;
    for(int i=0; i<csize; i++)
       VARLIST[clause->ATOM_LIST[i]->VAR]->AddRecord(CID,
                  clause->ATOM_LIST[i]->VAL,
                  clause->ATOM_LIST[i]->EQUAL);
  //   UNITLIST.push_front(CLAUSELIST.size()-1);
     return clause;

   }

  else return analyzeConflict(resolvent);
    }

Literal * Formula::maxLit(Clause * clause)
{ // cout<<"maxLit on: "<<endl;
//  clause->Print();
  int index = 0;
  int clindex = 0;
  int decisions = DECSTACK.size();
  for(int i=0; i<clause->NumAtom; i++)
  { Literal * current = clause->ATOM_LIST[i];
  //  current->Print();
   for(int j=decisions-1; j<decisions && j > -1; j--)
    { //cout<<current->VAR<<" "<< DECSTACK[j]->VAR <<endl;
    // cout<<current->VAL<<" "<<DECSTACK[j]->VAL<<endl;
     // current->EQUAL != DECSTACK[j]->EQUAL
      if( ( (current->VAR == DECSTACK[j]->VAR && current->VAL == DECSTACK[j]->VAL && current->EQUAL != DECSTACK[j]->EQUAL)  || (current->VAR == DECSTACK[j]->VAR && current->VAL != DECSTACK[j]->VAL && current->EQUAL == DECSTACK[j]->EQUAL && current->EQUAL == true)  )  && index <= j) {
      index = j; clindex = i;
   }

  }

}return clause->ATOM_LIST[clindex];
}
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

  //  cout<<"Literal in unit: "<<lit_var<<" "<<lit_val<<endl;

    if (VARLIST[lit_var]->ATOMASSIGN[lit_val] == 0) {// cout<<"Unit literal is : "<<lit_var<<(lit_equal?"=":"!=")<<lit_val<<endl;
    return new Literal(lit_var,(lit_equal?'=':'!'),lit_val);

  } }

  UNITLIST.pop_front();
  if (!UNITLIST.empty()) unitLiteral(CLAUSELIST[UNITLIST.front()]);
  else return new Literal(-1,'!',-1);



}

//\\====================NON-CHRONOLOGICAL BACKTRACK=============================

int Formula::NonChronoBacktrack()
{
  // LEVEL = 0;
  //start of finite domain extended dpll
  // return 0 : if theory satisfied
  // return 1 : if time out
  // return 2 : if CONFLICT and later used as unsatisfied
while(true){
  // cout<<"Number of clauses so far: "<<CLAUSELIST.size()<<endl;
  //Check if theory satisfied

checkUnit();

  if(checkSat())
    return 0; // add PrintModel(); - from DECSTACK
  //Check if time out
 TIME_E = GetTime();
 if((TIME_E - TIME_S) > TIMELIMIT)
   return 1;
  //check if conflict

for(int var=0; var<VARLIST.size();var++){ // not necessary here
  if(!CONFLICT && checkEntail(var))
  {
  ENTAILS++;
  // cout<<"Entailment... "<<ENTAILLITERAL->VAR<<"="<<ENTAILLITERAL->VAL<<endl;
  /* Clause * entailClause = new Clause();
  entailClause -> LEVEL = LEVEL;
  for (int i=0; i < VARLIST[ENTAILLITERAL->VAR]->DOMAINSIZE; i++){
  entailClause -> AddAtom(new Literal(ENTAILLITERAL->VAR,'=',i));
  }
  CLAUSELIST.push_back(entailClause);

  /*int csize = entailClause->NumAtom;
  int CID = CLAUSELIST.size()-1;
  for(int i=0; i<csize; i++){
  VARLIST[entailClause->ATOM_LIST[i]->VAR]->AddRecord(CID,
           entailClause->ATOM_LIST[i]->VAL,
           entailClause->ATOM_LIST[i]->EQUAL); } */

  // VARLIST[ENTAILLITERAL->VAR]->CLAUSEID[ENTAILLITERAL->VAL] = -2;
  //cout<<"Setting reason for the entailed literal "<<ENTAILLITERAL->VAR<<"="<<ENTAILLITERAL->VAL<<": "<<endl;
  //// REASON = entailClause;
  // CLAUSELIST[UNITCLAUSE]->Print();
  UNITCLAUSE = -2;
  reduceTheory(ENTAILLITERAL->VAR, true, ENTAILLITERAL->VAL);

  }
}
// checkUnit();
  while(!CONFLICT && !UNITLIST.empty()){
    int unit_clause = UNITLIST.front();
    UNITCLAUSE = unit_clause;
  //  cout<<"Unit clause:"<<endl; CLAUSELIST[unit_clause]->Print();
    Literal * unit = unitLiteral(CLAUSELIST[unit_clause]);
  //  REASON = CLAUSELIST[UNITCLAUSE];
  //  cout<<"Unit literal: "<<endl;
    if(unit->VAR == -1 && unit->VAL == -1 && unit->EQUAL == false) ; //cout<<"No units left."<<endl;
    else
    { // unit->Print();
     UNITLIST.pop_front();
     reduceTheory(unit->VAR, unit->EQUAL, unit->VAL);

    }

  }

  if(CONFLICT)
    {// cout << "There is a conflict at level: " << LEVEL << endl;
    // cout<<"Conflicting clause: "<<  endl;
  //   CLAUSELIST[CONFLICTINGCLAUSE] -> Print();

      if(LEVEL == 0) return 2;
      // otherwise



      LEVEL = backtrackLevel(analyzeConflict(CLAUSELIST[CONFLICTINGCLAUSE])); // learning a clause here
  //    cout << "We are backtracking to the level: " << LEVEL << endl;
      BACKTRACKS++;
  //    cout << "# of backtracks so far: "<<BACKTRACKS<<endl;
      CONFLICT = false;
      undoTheory(LEVEL);
    }
    // If there is a unit clause, propagate



    else { // otherwise choose a literal and propagate - no need for separate unit propagation
  Literal * atom = chooseLiteral();
  if(atom)
    {
      DECISIONS++;
      LEVEL++;
    /*  Clause * chooseClause = new Clause();
      chooseClause -> LEVEL = LEVEL;
      chooseClause -> SAT;
      chooseClause -> AddAtom(new Literal(atom->VAR,'=',atom->VAL));
      chooseClause -> AddAtom(new Literal(atom->VAR,'!',atom->VAL));
    //  REASON = chooseClause;
      CLAUSELIST.push_back(chooseClause); //decision...
      UNITCLAUSE = CLAUSELIST.size()-1; */
      UNITCLAUSE = -1; // REASON for subsequent falsified atoms
      reduceTheory(atom->VAR, atom->EQUAL, atom->VAL);
    }
  }
}
}
//End Formula
