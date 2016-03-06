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
{ cout<<"Checking satisfiability..."<<endl;
  int size = CLAUSELIST.size();
  for(int i=0; i<size; i++)
    {
      //if clause is not SAT return false
      if(!CLAUSELIST[i]->SAT) return false;
	  // cout<<"Unsatisfied clause # "<<i<<endl;
    }
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
{ cout<<"Checking if there is a unit clause..."<<endl;
  int size = CLAUSELIST.size();
  UNITLIST.clear();
  for(int i=0; i<size; i++)
    {
      //if clause is not SAT and has 1 unassigned literal
      //add the index i into Unitlist
      if((CLAUSELIST[i]->NumUnAss == 1) &&
	 (!CLAUSELIST[i]->SAT)){
	UNITLIST.push_back(i);
  cout<<"Found a unit clause: "<<i<<endl;}

    }
}

//checkEntail
bool Formula::checkEntail(int var)
{ cout<<"Checking entailment..."<<endl;

/*  if(ENTAILLITERAL){
    delete ENTAILLITERAL;
    cout<<"Successfully freed entailment var"<<endl; } */

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
    cout<<"Decision literal: "<<tvar<<"="<<tval<<endl;
    Clause * chooseClause = new Clause();
    chooseClause -> LEVEL = LEVEL;
    chooseClause -> AddAtom(new Literal(tvar,'=',tval));
    chooseClause -> AddAtom(new Literal(tvar,'!',tval));
    CLAUSELIST.push_back(chooseClause);
    // cout<<"Clause list: "<<CLAUSELIST.size()<<endl;
    VARLIST[tvar] -> CLAUSEID[tval] = CLAUSELIST.size()-1;
    UNITCLAUSE = VARLIST[tvar] -> CLAUSEID[tval];
    cout<<"Setting the reason for the decision literal "<<tvar<<"="<<tval<<endl;
    chooseClause -> Print();
    return new Literal(tvar, tval); } else return NULL;
}

//reduceTheory
void Formula::reduceTheory(int var, bool equals, int val)
{
  if(equals)
    {
      cout<<"Reducing literal: "<<var<<"="<<val<<" at level "<<LEVEL<<endl;
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
      cout<<"The reason for the literal: "<<endl;
      if (UNITCLAUSE > -1) CLAUSELIST[UNITCLAUSE]->Print(); else cout<<-1<<endl;
      //Add literal to DecisionStack
    //  cout<<"Size of the decision stack: "<<DECSTACK.size()<<endl;

      DECSTACK.push_back(new Literal(var, '=', val));
      cout<<"Adding literal to the decision stack: "<<var<<"="<<val<<endl;
      cout<<"Size of the decision stack: "<<DECSTACK.size()<<endl;
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
        cout<<"Set the reason for the literal "<<var<<(!equals?"=":"!")<<i<<endl;
        if (UNITCLAUSE > -1) CLAUSELIST[UNITCLAUSE]->Print(); else cout<<-1<<endl;
      DECSTACK.push_back(new Literal(var, '!', i));
    cout<<"Adding literal to the decision stack: "<<var<<"!"<<i<<endl;
    cout<<"Size of the decision stack: "<<DECSTACK.size()<<endl;
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
        cout<<"Set the reason for the literal "<<var<<(!equals?"=":"!")<<i<<endl;
        if (UNITCLAUSE > -1) CLAUSELIST[UNITCLAUSE]->Print(); else cout<<-1<<endl;
      DECSTACK.push_back(new Literal(var, '!', i));
    cout<<"Adding literal to the decision stack: "<<var<<"!"<<i<<endl;
    cout<<"Size of the decision stack: "<<DECSTACK.size()<<endl;
	    }
	}



    }
  else
    {
      cout<<"Reducing: "<<var<<"!"<<val<<" at level "<<LEVEL<<endl;
      //first satisfy all clauses with negate literal, and remove
      //literal from claues
      satisfyClauses(var, equals, val);
      removeLiteral(var, !equals, val);
      VARLIST[var]->ATOMASSIGN[val] = -1;
      VARLIST[var]->ATOMLEVEL[val] = LEVEL;
      VARLIST[var]->CLAUSEID[val] = UNITCLAUSE;
      //Add literal to DecisionStack
      cout<<"Adding literal to the decision stack: "<<var<<"!"<<val<<endl;
      DECSTACK.push_back(new Literal(var, '!', val));
      //check Entailment on this variable
      if(checkEntail(var))
    {
    ENTAILS++;
    cout<<"Entailment... "<<ENTAILLITERAL->VAR<<"="<<ENTAILLITERAL->VAL<<endl;
    Clause * entailClause = new Clause();
    entailClause -> LEVEL = LEVEL;
    for (int i=0; i < VARLIST[ENTAILLITERAL->VAR]->DOMAINSIZE; i++){
      entailClause -> AddAtom(new Literal(ENTAILLITERAL->VAR,'=',i));
    }
    CLAUSELIST.push_back(entailClause);
    VARLIST[ENTAILLITERAL->VAR]->CLAUSEID[ENTAILLITERAL->VAL] = CLAUSELIST.size()-1;
    cout<<"Setting reason for the entailed literal "<<ENTAILLITERAL->VAR<<"="<<ENTAILLITERAL->VAL<<": "<<endl;
    entailClause -> Print();
    reduceTheory(ENTAILLITERAL->VAR, true, ENTAILLITERAL->VAL);


    }

/*      if(checkEntail(var))
	{
	  ENTAILS++;
	  cout<<"Entailment .."<<ENTAILLITERAL->VAR<<"="<<ENTAILLITERAL->VAL<<endl;
    cout<<"Setting reason for the entailed literal "<<ENTAILLITERAL->VAR<<"="<<ENTAILLITERAL->VAL<<": "<<UNITCLAUSE<<endl;
	  VARLIST[ENTAILLITERAL->VAR]->CLAUSEID[ENTAILLITERAL->VAL] = UNITCLAUSE;
	  reduceTheory(ENTAILLITERAL->VAR, true, ENTAILLITERAL->VAL);
	} */

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
  cout<<"add .. "<<var<<(equals?"=":"!")<<val<<endl;
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
  resolvent->LEVEL = LEVEL;
  // adding lit in clause s.t. lit not \entails literal
  for (int i=0; i<clause->ATOM_LIST.size();i++)
  {
    if (clause->ATOM_LIST[i]->VAR != literal->VAR ) resolvent->AddAtom(clause->ATOM_LIST[i]);
    else if (clause->ATOM_LIST[i]->VAL != literal->VAL && clause->ATOM_LIST[i]->EQUAL == literal->EQUAL) resolvent->AddAtom(clause->ATOM_LIST[i]);
    else if (clause->ATOM_LIST[i]->VAL != literal->VAL && clause->ATOM_LIST[i]->EQUAL != literal->EQUAL && !clause->ATOM_LIST[i]->EQUAL) resolvent->AddAtom(clause->ATOM_LIST[i]);

  }
// adding lit in reason s.t. lit not \falsifies literal - compatible
  for (int i=0; i<reason->ATOM_LIST.size();i++)
  {
    if (reason->ATOM_LIST[i]->VAR != literal->VAR && !HasAtom(resolvent,reason->ATOM_LIST[i])) resolvent->AddAtom(reason->ATOM_LIST[i]);

    else if (reason->ATOM_LIST[i]->VAL != literal->VAL && reason->ATOM_LIST[i]->EQUAL != literal->EQUAL && !HasAtom(resolvent,reason->ATOM_LIST[i])) resolvent->AddAtom(reason->ATOM_LIST[i]);
  }
 cout<<"Resolvent: "<<endl;
 resolvent->Print();

 return resolvent;
}
//anaylzeConflict: TODO FIX the mistake here: THE LEARNED CLAUSES ARE NOT entailed
// BY THE THEORY

bool Formula::Potent(Clause * clause)
{
  int counter = 0;
  for (int i = 0; i < clause->ATOM_LIST.size(); i++)
  {
    if (VARLIST[clause->ATOM_LIST[i]->VAR]
    ->ATOMLEVEL[clause->ATOM_LIST[i]->VAL] == LEVEL) counter++;
    else counter = counter;
  }

  if(counter != 1) return false;
  else return true;

}

int Formula::backtrackLevel(Clause * learnedClause)
{ int tlevel = LEVEL;
  int csize = learnedClause->NumAtom;
  int CID = 0;
  //if learned clause has only one literal then backtrack to level 0
  if(csize == 1)
  return 0;

  for(int i=0; i<csize; i++)
  {
  if((tlevel!= VARLIST[learnedClause->ATOM_LIST[i]->VAR]
  ->ATOMLEVEL[learnedClause->ATOM_LIST[i]->VAL]) &&
  (CID < VARLIST[learnedClause->ATOM_LIST[i]->VAR]
  ->ATOMLEVEL[learnedClause->ATOM_LIST[i]->VAL]))
  CID = VARLIST[learnedClause->ATOM_LIST[i]->VAR]->ATOMLEVEL[learnedClause->ATOM_LIST[i]->VAL];
  }
  return CID;
}

Clause * Formula::analyzeConflict(Clause * clause)
{
  if (Potent(clause)) {
    clause->NumUnAss--;
    cout<<"Learned a clause: "<<endl;
    clause->Print();
    CLAUSELIST.push_back(clause);
    int csize = clause->NumAtom;
    int CID = CLAUSELIST.size()-1;
    for(int i=0; i<csize; i++)
      VARLIST[clause->ATOM_LIST[i]->VAR]->AddRecord(CID,
                 clause->ATOM_LIST[i]->VAL,
                 clause->ATOM_LIST[i]->EQUAL);
    return clause;
}
  Clause * resolvent = new Clause();
//  cout<<"Conflict at level: "<<LEVEL<<endl;
  //learnedClause
//  cout<<"Conflicting clause is: "<<endl;
//  clause->Print();
  int numLit = 0;
  int tlevel = LEVEL;
  int csize = -1;
  Literal * lastFalse;
  int lastIndex = DECSTACK.size()-1;

  // Resolve CID and latest falsified literal + reason

 cout<<"We are working with the clause: "<<endl;
 clause->Print();

 //Find latest falsified literal
  csize = clause->NumAtom;
//  cout<<"csize: "<<csize<<endl;
//  cout<<"decsize: "<<DECSTACK.size()<<endl;
  /*    for (int j=DECSTACK.size()-1; j>-1; j--)
      { cout<<"j: "<<j<<endl;
        for(int i=0; i<csize; i++)
          { cout<<"i: "<<i<<endl;
            Literal * lit = clause->ATOM_LIST[i];
            lit -> Print();
            if(lit->VAL == DECSTACK[j]->VAL && lit->VAR == DECSTACK[j]->VAR) lastIndex = j; break;
          }
        }*/
  cout<<"lastIndex is: "<<lastIndex<<endl;
  lastFalse = DECSTACK[lastIndex];
  resolvent = resolve(clause, new Literal(DECSTACK[lastIndex]->VAR,(DECSTACK[lastIndex]->EQUAL?'!':'='),DECSTACK[lastIndex]->VAL),CLAUSELIST[VARLIST[DECSTACK[lastIndex] ->VAR]-> CLAUSEID[DECSTACK[lastIndex]->VAL]]);
  // lastFalse->Print();
  // CLAUSELIST[VARLIST[DECSTACK[lastIndex] ->VAR]-> CLAUSEID[DECSTACK[lastIndex]->VAL]]->Print();

  return analyzeConflict(resolvent);

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
      cout<<"Unit propagation... "<<endl;
      UNITLIST.pop_front(); // Delete first element
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

		  VARLIST[lit_var]->CLAUSEID[lit_val] = unit_clause;
		}
	    }
	  if(flag)
	    {
	      cout<<"Unit literal: "<<lit_var<<(lit_equal?"=":"!=")<<lit_val<<endl;
        cout<<"Set the reason for this literal: "<<endl;
        if (unit_clause > -1) CLAUSELIST[unit_clause]->Print(); else cout<<-1<<endl;
	      reduceTheory(lit_var, lit_equal, lit_val);
	      flag = false;
	    }
	}
    }
  if(!CONFLICT)
    return true;
  else
    {
      cout<<"Conflict : "<<CONFLICTINGCLAUSE<<endl;
      UNITLIST.clear();
      return false;
    }
}

//\\====================NON-CHRONOLOGICAL BACKTRACK=============================

int Formula::NonChronoBacktrack(int level)
{
  //start of finite domain extended dpll
  // return 0 : if theory satisfied
  // return 1 : if time out
  // return 2 : if CONFLICT and later used as unsatisfied
  checkUnit();
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
      LEVEL = backtrackLevel(analyzeConflict(CLAUSELIST[CONFLICTINGCLAUSE]));
      cout << "We are backtracking to the level: " << LEVEL << endl;
      BACKTRACKS++;
      CONFLICT = false;
      undoTheory(LEVEL);
      checkUnit();
      return NonChronoBacktrack(LEVEL);
    }

  //When there is no conflict, choose a literal to branch on
  Literal * atom = chooseLiteral();
  if(atom)
    {
      DECISIONS++;
      LEVEL++;
      reduceTheory(atom->VAR, atom->EQUAL, atom->VAL);
      return NonChronoBacktrack(LEVEL);
    }
  return 0;

}
//End Formula
//**********************************************************************//
