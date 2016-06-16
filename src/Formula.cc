//**********************************************************************//
//
// Finite Domain Solver
//
// File : Formula.cc
//
//**********************************************************************//
//Including Libraries and Header files
#include "Formula.h"
#include "Clause.h"
#include <cstring>
#include <stdexcept>
#include <climits>
using namespace std;
//**********************************************************************//
//Default 0-arg constructor
Formula::Formula () {
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
	WATCH = false;
	CMV = false;
}

//1-arg constructor
Formula::Formula ( CommandLine * cline ) {
	//Setting appropriate values and
	VARLIST.reserve(10);
	CLAUSELIST.reserve(10);
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
	DECSTACK.reserve(10);
	RESTARTS = 0;
	LOG = cline->LOG;
	WATCH = cline->WATCH;
	CMV =  cline->CMV;
}

// Parse input and build the formula aka theory
void Formula::BuildFormula ( CommandLine* cline ) {
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
			sscanf(line_buffer, "p cnf %d %d", &var, &val);

		}
		//else if line is d var# domsize
		else if(line_buffer[0] == 'd')
		{
			sscanf(line_buffer, "d %d %d", &var, &val);
			temp_var = new Variable(var, val);
			VARLIST.push_back(temp_var);
		}
		//else its clause lines
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
					sscanf(word_buffer, "%d %c %d", &var, &ch, &val);
					if(ch == '!')
						sscanf(word_buffer, "%d %c %c %d", &var, &ch, &ch1, &val);
					//checking if variable = 0 i.e end of line
					if(var != 0)
					{
						//add it to clause_list's atom_list
						temp_atom = new Literal(var, ch, val);
						temp_clause->addAtom(temp_atom);

						//increment the index representing value so that
						//we can keep track of number of occurrences of each
						//variable and its domain value, if ch is = then increment
						//the value count, else increment others count
						if(ch == '=')
						{
							VARLIST[var]->ATOMCNTPOS[val]++;
							VARLIST[var]->addRecord(clause_num, val, true);
						}
						else
						{
							VARLIST[var]->ATOMCNTNEG[val]++;
							VARLIST[var]->addRecord(clause_num, val, false);
						}
						atom_num++;
					}
				}
			}while(*lp);
			CLAUSELIST.push_back(temp_clause);


			if ( cline -> CMV ) {

				// watch literals that concern different variable

				Literal* watched1 = temp_clause -> ATOM_LIST[0];

				// we will be choosing a watched literal as a decision literal, thus track watched literals:

				VARLIST[watched1 -> VAR] -> ATOMWATCH[watched1 -> VAL] = 1;


				Literal* watched2 = NULL;

				if ( temp_clause -> NumAtom > 1 ) {

					// by default assign the second literal:
					temp_clause -> W2 = 1;
					watched2 = temp_clause -> ATOM_LIST[1];

					VARLIST[watched2 -> VAR] -> ATOMWATCH[watched2 -> VAL] = 2;

					for ( int i = 1; i < temp_clause -> NumAtom; i++) {

						// look for a literal with a different variable

						if ( temp_clause -> ATOM_LIST[i] -> VAR != watched1 -> VAR ) {

							temp_clause -> W2 = i;

							watched2 = temp_clause -> ATOM_LIST[i];

							VARLIST[watched2 -> VAR] -> ATOMWATCH[watched2 -> VAL] = 2;
							break;

						}
					}

				}

				// if only one literal, watched2 is null

				else {
					temp_clause -> W2 = -1;
				}

			}

			// with watched literals option, assign first two literals in the clause to watched1 and watched2


			if ( cline -> WATCH ) {

				Literal* watched1 = temp_clause -> ATOM_LIST[0];
				temp_clause -> WATCHED[0] = watched1;
				// we will be choosing a watched literal as a decision literal, thus track watched literals:
				VARLIST[watched1 -> VAR] -> ATOMWATCH[watched1 -> VAL] = 1;

				if ( temp_clause -> NumAtom > 1 ) {

					Literal* watched2 = temp_clause -> ATOM_LIST[1];
					temp_clause -> WATCHED[1] = watched2;
					VARLIST[watched2 -> VAR] -> ATOMWATCH[watched2 -> VAL] = 2;

				} else temp_clause -> WATCHED[1] = NULL; // if only one literal, watched2 is null
			}


			atom_num = 0;
			++clause_num;
		}
	}

	//closing file
	infile.close();
	TIME_E = GetTime();
}

// Printing functions

void Formula::PrintVar()
{

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
	for(unsigned int  i=0; i<DECSTACK.size();i++)
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
	printf( "Variables   : %zu\n", VARLIST.size()-1 );
	printf( "Clauses     : %zu\n", CLAUSELIST.size() );
	printf("Restarts   : %d\n", RESTARTS);

	printf("\n");
}


bool Formula::verifyModel() {
	//set all clause to false
	for(unsigned int  i=0; i<CLAUSELIST.size(); i++)
		CLAUSELIST[i]->SAT = false;
	//for each variable and its value set all clauses in
	//which it occurs to true
	int val = 0;
	int domainsize = 0;
	VARRECORD * curr = NULL;
	for(unsigned int  i=1; i<VARLIST.size(); i++) {
		val = VARLIST[i]->VAL;
		if(val != -1) {
			domainsize = VARLIST[i]->DOMAINSIZE;
			for(int j=0; j<domainsize; j++) {
				if(j != val) {
					curr = VARLIST[i]->ATOMRECNEG[j];
					while(curr) {
						CLAUSELIST[curr->c_num]->SAT = true;
						curr = curr->next;
					}
				}
				else {
					curr = VARLIST[i]->ATOMRECPOS[j];
					while(curr) {
						CLAUSELIST[curr->c_num]->SAT = true;
						curr = curr->next;
					}
				}
			}
		}
		else {
			domainsize = VARLIST[i]->DOMAINSIZE;
			for(int j=0; j<domainsize; j++){
				if(VARLIST[i]->ATOMASSIGN[j] == -1) {
					curr = VARLIST[i]->ATOMRECNEG[j];
					while(curr) {
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

// Functions used in the extended DPLL algorithm:

bool Formula::checkSat () {

	// if some clause in the formula is not satisfied return false

	if ( LOG ) cout << "Checking satisfiability..." << endl;

	int size = CLAUSELIST.size();

	for ( int i = 0; i < size; i++ ) {
		if ( ! CLAUSELIST[i] -> SAT ) {
			return false;
		}
	}
	return true;
}

int Formula::checkConflict () {

	// if some clause is not satisfied and has 0 unassigned literals returns id of a conflicting clause

	int size = CLAUSELIST.size();

	for ( int i = 0; i < size; i++ ) {
		if ( (CLAUSELIST[i] -> NumUnAss == 0) && ( ! CLAUSELIST[i] -> SAT ) ) {
			CONFLICT = true;
			CONFLICTINGCLAUSE = i;
			return CONFLICTINGCLAUSE;
		}
	}
	return -1;
}

void Formula::checkUnit () {

	// if clause is not satisfied and has 1 unassigned literal add it to the unitlist
	// also checks for conflicts

	if ( LOG ) cout << "Checking if there is a unit clause..." << endl;

	int size = CLAUSELIST.size();

	UNITLIST.clear();

	for ( int i = 0; i < size; i++ ) {
		if ( ( CLAUSELIST[i] -> NumUnAss == 1 && ! CLAUSELIST[i] -> SAT ) ) {
			if (LOG) cout << "Found a unit clause: " << i << endl;
			UNITLIST.push_back(i);
		}
		else if ( CLAUSELIST[i] -> NumUnAss == 0 && ! CLAUSELIST[i] -> SAT ) {
			CONFLICTINGCLAUSE = i;
			CONFLICT = true; }
	}
}

bool Formula::checkEntail ( int var ) {

	// check if there is exactly one domain value is not assigned for var

	// if ( LOG ) cout << "Checking entailment..." << endl;

	int domainsize = 0;
	bool flag = false;
	int domainvalue = -1;

	if ( ! VARLIST[var] -> SAT ) {

		domainsize = VARLIST[var] -> DOMAINSIZE;

		for ( int j = 0; j < domainsize; j++ ) {
			if ( VARLIST[var] -> ATOMASSIGN[j] == 0 ) {
				if( ! flag ) {

					domainvalue = j;
					flag = true;

				} else {

					flag = false;
					break;

				}
			}
		}
	}

	// if found an entail literal then assign memory to variable and return true

	if ( flag ) {
		ENTAILLITERAL = new Literal ( var, domainvalue );
		return true;
	}
	return false;
}

Literal* Formula::chooseLiteral () {

	/*  Pick a literal which is not yet satisfied, and which satisfies more clauses and removes less literals,
	 *  use literal counts for this. Other heuristics are possible.
	 */

	int max = INT_MIN;
	int tmax = -1;
	int tvar = -1;
	int tval = -1;
	UNITCLAUSE = -1;

	for ( unsigned int i = 0; i < VARLIST.size(); i++ ) {
		if ( ! VARLIST[i] -> SAT ) {
			for ( int j = 0; j < VARLIST[i] -> DOMAINSIZE; j++ ) {
				if ( VARLIST[i] -> ATOMASSIGN[j] == 0 ) {
					tmax = VARLIST[i] -> ATOMCNTPOS[j] - VARLIST[i] -> ATOMCNTNEG[j];
					if ( max < tmax ) {
						max = tmax;
						tvar = i;
						tval = j;
					}
				}
			}
		}
	}
	return ( tvar != -1 ? new Literal ( tvar, tval ) : NULL );
}

void Formula::reduceTheory ( int var, bool equals, int val ) {

	if ( equals ) {

		if ( LOG ) cout << "Reducing literal: " << var << "="<< val << " at level " << LEVEL << endl;

		// satisfy all clauses with theS literal, and remove the complement literal from clauses

		satisfyClauses ( var, equals, val );
		removeLiteral ( var, !equals, val );

		VARLIST[var] -> ATOMASSIGN[val] = 1;
		VARLIST[var] -> ATOMLEVEL[val] = LEVEL;

		VARLIST[var] -> VAL = val;
		VARLIST[var] -> SAT = true; // means variable is assigned
		VARLIST[var] -> LEVEL = LEVEL; // value assigned a positive value at this level

		VARLIST[var] -> CLAUSEID[val] = UNITCLAUSE;

		if ( LOG ) {
			cout << "The reason for the literal: " << endl;
			if ( UNITCLAUSE > -1 ) CLAUSELIST[UNITCLAUSE] -> Print(); else cout << UNITCLAUSE << endl;
		}

		//Add literal to the Decision Stack
		DECSTACK.push_back ( new Literal ( var, '=', val ) );
		VARLIST[var] -> ATOMINDEX[val] = DECSTACK.size() - 1; // to use in maxLit

		//foreach domain value x from dom(v) which is not assigned assign it
		for ( int i = 0; i < val && ! CONFLICT; i++ ) {

			if ( VARLIST[var] -> ATOMASSIGN[i] == 0 ) {

				satisfyClauses ( var, !equals, i );
				removeLiteral ( var, equals, i );

				VARLIST[var] -> ATOMASSIGN[i] = -1;
				VARLIST[var]->ATOMLEVEL[i] = LEVEL;
				VARLIST[var]->CLAUSEID[i] = UNITCLAUSE;
				VARLIST[var]->ATOMINDEX[i] = DECSTACK.size() - 1;

			}
		}

		for ( int i = val+1; i < VARLIST[var] -> DOMAINSIZE && ! CONFLICT; i++ ) {
			if ( VARLIST[var] -> ATOMASSIGN[i] == 0 ) {

				satisfyClauses ( var, !equals, i );
				removeLiteral ( var, equals, i );

				VARLIST[var] -> ATOMASSIGN[i] = -1;
				VARLIST[var] -> ATOMLEVEL[i] = LEVEL;
				VARLIST[var] -> CLAUSEID[i] = UNITCLAUSE;
				VARLIST[var] -> ATOMINDEX[i] = DECSTACK.size() - 1;
			}
		}
	}
	else {
		if ( LOG ) cout << "Reducing: " << var << "!" << val << " at level " << LEVEL << endl;

		satisfyClauses ( var, equals, val );
		removeLiteral ( var, !equals, val );

		VARLIST[var] -> ATOMASSIGN[val] = -1;
		VARLIST[var] -> ATOMLEVEL[val] = LEVEL;
		VARLIST[var] -> CLAUSEID[val] = UNITCLAUSE;
		VARLIST[var] -> ATOMINDEX[val] = DECSTACK.size() - 1;

		DECSTACK.push_back ( new Literal ( var, '!', val ) );

		// Check entailment on this variable

		if ( checkEntail ( var ) ) {

			ENTAILS++;
			if ( LOG ) cout << "Entailment... " << ENTAILLITERAL -> VAR << "=" << ENTAILLITERAL -> VAL << endl;
			UNITCLAUSE = -2;
			reduceTheory ( ENTAILLITERAL -> VAR, true, ENTAILLITERAL -> VAL );
		}
	}
}


inline void Formula::satisfyClauses ( int var, bool equals, int val ) {

	int lit_var = -1;
	bool lit_equal = false;
	int lit_val = -1;

	VARRECORD * current = NULL;

	// Get clauses where the literal appears:
	if ( equals ) current = VARLIST[var] -> ATOMRECPOS[val];
	else current = VARLIST[var] -> ATOMRECNEG[val];

	while ( current ) {

		// For every clause that contains this literal, satisfy it

		if ( ! CLAUSELIST[current->c_num] -> SAT ) {

			CLAUSELIST[current->c_num]->SAT = true;
			CLAUSELIST[current->c_num]->LEVEL = LEVEL;

			// and update the counts for other unassigned literals in the clause (corresponds to deleting the clause from the theory)

			for ( int i = 0; i < CLAUSELIST[current -> c_num] -> NumAtom; i++ ) {

				lit_var = CLAUSELIST[current -> c_num] -> ATOM_LIST[i] -> VAR;
				lit_equal = CLAUSELIST[current -> c_num] -> ATOM_LIST[i] -> EQUAL;
				lit_val = CLAUSELIST[current -> c_num] -> ATOM_LIST[i] -> VAL;

				if ( VARLIST[lit_var] -> ATOMASSIGN[lit_val] == 0 ) {

					CLAUSELIST[current->c_num]->NumUnAss--;

					if ( lit_equal ) VARLIST[lit_var] -> ATOMCNTPOS[lit_val]--;
					else VARLIST[lit_var] -> ATOMCNTNEG[lit_val]--;
				}
			}
		}
		current = current -> next;
	}
	current = NULL;
}



//removeLiteral
inline void Formula::removeLiteral ( int var, bool equals, int val ) {

	VARRECORD * current = NULL;

	// for every record of this literal reduce the number of unassigned literals in unsatisfied clauses
	// update counts

	if ( equals ) current = VARLIST[var] -> ATOMRECPOS[val];
	else current = VARLIST[var] -> ATOMRECNEG[val];

	while ( current ) {

		if( ! CLAUSELIST[current->c_num] -> SAT ) {

			CLAUSELIST[current->c_num]->NumUnAss--;

			if ( equals ) VARLIST[var] -> ATOMCNTPOS[val]--;
			else VARLIST[var] -> ATOMCNTNEG[val]--;

			//checking for units and conflicts right away:

			if ( CLAUSELIST[current->c_num] -> NumUnAss == 1 ) {
				UNITLIST.push_front(current -> c_num);
			}

			if ( CLAUSELIST[current->c_num] -> NumUnAss == 0 ) {

				CONFLICT = true;
				CONFLICTINGCLAUSE = current->c_num;
			}
		}
		current = current->next;
	}
	current = NULL;
}


//unsatisfyClauses, used in undoTheory(level)
inline void Formula::unsatisfyClauses ( int var, bool equals, int val, int level ) {

	VARRECORD * current = NULL;

	int lit_var = -1;
	bool lit_equal = false;
	int lit_val = -1;

	if ( equals ) current = VARLIST[var] -> ATOMRECPOS[val];
	else current = VARLIST[var] -> ATOMRECNEG[val];

	while ( current ) {

		if ( CLAUSELIST[current -> c_num] -> LEVEL > level ) {

			for ( int i = 0; i < CLAUSELIST[current -> c_num] -> NumAtom; i++ ) {

				lit_var = CLAUSELIST[current->c_num]->ATOM_LIST[i]->VAR;
				lit_equal = CLAUSELIST[current->c_num]->ATOM_LIST[i]->EQUAL;
				lit_val = CLAUSELIST[current->c_num]->ATOM_LIST[i]->VAL;

				if ( ( VARLIST[lit_var] -> ATOMLEVEL[lit_val] > level )
						|| ( VARLIST[lit_var] -> ATOMASSIGN[lit_val] == 0 ) ) {

					CLAUSELIST[current -> c_num] -> NumUnAss++;

					if ( lit_equal )
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
inline void Formula::addLiteral ( int var, bool equals, int val ) {

	VARRECORD * current = NULL;

	//for every record of this literal increase the number of
	//unassigned literals from unsatisfied clauses

	if ( equals ) current = VARLIST[var] -> ATOMRECPOS[val];
	else current = VARLIST[var] -> ATOMRECNEG[val];

	while ( current ) {
		if ( ! CLAUSELIST[current -> c_num] -> SAT ) {

			CLAUSELIST[current -> c_num] -> NumUnAss++;

			if ( equals ) VARLIST[var] -> ATOMCNTPOS[val]++;
			else VARLIST[var] -> ATOMCNTNEG[val]++;
		}
		current = current->next;
	}
	current = NULL;
}


bool Formula::hasAtom ( Clause* clause, Literal* atom ) {

	// true if clause contains the atom

	for ( int i = 0; i < clause -> NumAtom; i++ ) {

		Literal* c_atom = clause -> ATOM_LIST[i];

		if ( LitIsEqual (atom, c_atom) )
			return true;
	}
	return false;
}

Clause* Formula::resolve ( Clause* clause, Literal* literal, Clause* reason ) {

	//  Performs extended resolution on clause and reason wrt literal.
	// (See pp.47-50 of Jain: https://github.com/akinanop/mvl-solver/blob/master/literature/Jain_thesis.pdf)

	Clause * resolvent = new Clause();

	// Add literals L from the clause that are satisfied by at least one interpretation that does not satisfy L:

	for ( unsigned int i = 0; i < clause -> ATOM_LIST.size(); i++ ) {

		Literal* c_atom = clause -> ATOM_LIST[i];

		if ( c_atom -> VAR != literal -> VAR ) resolvent -> addAtom ( c_atom );
		else if ( c_atom -> VAL != literal -> VAL && c_atom -> EQUAL != literal -> EQUAL ) resolvent -> addAtom ( c_atom );
		else if ( falsifies ( c_atom, literal ) ) resolvent -> addAtom ( c_atom );

	}
	// Add literals L from the reason clause that are satisfied by at least one interpretation that also satisfies L, i.e., doesn't falsify.

	for ( unsigned int  i = 0; i < reason -> ATOM_LIST.size(); i++ ) {

		Literal* r_atom = reason -> ATOM_LIST[i];

		if ( ! falsifies ( r_atom, literal ) && ! hasAtom ( resolvent, r_atom ) ) resolvent -> addAtom ( r_atom );

	}

	return resolvent;
}



bool Formula::potent ( Clause* clause ) {

	// Check if there is exactly one atom falsified at the current level

	LASTFALSE = -1;
	int index = 0;
	int counter = 0;

	for ( int i = 0; i < clause -> NumAtom; i++ ) {

		Literal* atom = clause -> ATOM_LIST[i];

		if ( VARLIST[atom -> VAR] -> ATOMLEVEL[atom -> VAL] == LEVEL ) {
			counter++;
			index = i;
		}
	}

	if ( counter != 1 ) return false;

	else {
		LASTFALSE = index;
		return true;
	}
}

int Formula::backtrackLevel ( Clause * learnedClause ) {

	/* returns the second highest level associated
	  with a literal in the learnedClause */


	if ( LOG ) cout << "Finding backtrack level..." << endl;

	int max = -1;
	int csize = learnedClause -> NumAtom;
	WATCHED2 = 0; // the falsified literal just before lastFalse, assign as w2 in learnedClause
	/*
	 * Additional possibility: if learned clause has only one literal then backtrack to the level 0
    if ( csize == 1 )
    return 0;
	 */

	for ( int i = 0; i < csize; i++ ) {

		Literal* atom = learnedClause -> ATOM_LIST[i];
		int atom_level = VARLIST[atom -> VAR] -> ATOMLEVEL[atom -> VAL];

		if ( LEVEL > atom_level && max < atom_level ) {
			max = atom_level;
			WATCHED2 = i;
		}

	}

	// cout << "INDEX: " << INDEX << endl;

	if ( max != -1 ) return max;
	else return LEVEL-1;
}

void Formula::undoTheory ( int level ) {

	// Undo theory to the level
	// for each variable v, for each domain d from dom(v): if this value has been assigned at level this or greater undo

	for ( unsigned int  i = 1; i < VARLIST.size(); i++ ) {

		for ( int j = 0; j < VARLIST[i] -> DOMAINSIZE; j++ ) {

			if ( VARLIST[i] -> ATOMLEVEL[j] > level ) {

				if ( VARLIST[i] -> ATOMASSIGN[j] == -1 )
					addLiteral ( i, true, j );
				else
					addLiteral ( i, false, j );
			}
		}
	}

	for ( unsigned int  i = 1; i < VARLIST.size(); i++ ) {

		for ( int j = 0; j < VARLIST[i] -> DOMAINSIZE; j++ ) {

			if ( VARLIST[i] -> ATOMLEVEL[j] > level ) {

				if ( VARLIST[i] -> ATOMASSIGN[j] == -1)
					unsatisfyClauses (i, false, j, level );
				else
					unsatisfyClauses ( i, true, j, level );

				VARLIST[i]->ATOMLEVEL[j] = -10;
				VARLIST[i]->ATOMASSIGN[j] = 0;
				VARLIST[i]->CLAUSEID[j] = -10;
			}
		}

		if(VARLIST[i] -> LEVEL > level) {

			VARLIST[i] -> LEVEL = -1;
			VARLIST[i] -> SAT = false;
			VARLIST[i] -> VAL = -1;
		}
	}
	//undo the decision stack

	int decsize = DECSTACK.size();

	for ( int i = decsize-1; i > -1; i-- ) {
		if ( VARLIST[DECSTACK[i] -> VAR] -> ATOMASSIGN[DECSTACK[i] -> VAL] == 0 ) {
			DECSTACK.erase ( DECSTACK.begin() + i );
		}
	}
}


// Corresponding functions for watched literals version of the algorithm:

inline void Formula::watchedSatisfyLiteral ( int var, bool equals, int val ) {

	VARRECORD * current = NULL;

	// get clauses with the literal

	if ( equals ) current = VARLIST[var] -> ATOMRECPOS[val];
	else current = VARLIST[var] -> ATOMRECNEG[val];

	/*
	 * for every clause that contains this literal update watched literals, as described here:
	   https://github.com/akinanop/mvl-solver/blob/master/literature/Watched%20literals.pdf
	 */

	while ( current ) {

		// assign the satisfied literal from the clause to be watched1

		Clause* clause = CLAUSELIST[current->c_num];

		Literal* watched1 = CLAUSELIST[current->c_num]->WATCHED[0];
		Literal* watched2 = CLAUSELIST[current->c_num]->WATCHED[1];

		Literal* literal = NULL;

		for ( int j = 0; j < CLAUSELIST[current->c_num] -> NumAtom; j++ ) {

			Literal* temp = CLAUSELIST[current->c_num] -> ATOM_LIST[j];

			if ( temp -> VAR == var &&  temp -> VAL == val &&
					temp -> EQUAL == equals ) {

				literal = CLAUSELIST[current->c_num] -> ATOM_LIST[j];
				break;
			}
		}

		if ( sat ( CLAUSELIST[current->c_num]->WATCHED[0] )  != 1 ) {

			if ( CLAUSELIST[current->c_num]->WATCHED[1] != NULL && ! LitIsEqual ( CLAUSELIST[current->c_num]->WATCHED[1], literal ) ) {

				CLAUSELIST[current->c_num]->WATCHED[0] = literal;

			} else {

				CLAUSELIST[current->c_num]->WATCHED[1] = CLAUSELIST[current->c_num]->WATCHED[0];
				CLAUSELIST[current->c_num]->WATCHED[0] = literal;
			}
		}
		current = current -> next;
	}
}


inline void Formula::tempwatchedSatisfyLiteral ( int var, bool equals, int val ) {

	VARRECORD * current = NULL;

	// get clauses with the literal

	if ( equals ) current = VARLIST[var] -> ATOMRECPOS[val];
	else current = VARLIST[var] -> ATOMRECNEG[val];

	/*
	 * for every clause that contains this literal update watched literals, as described here:
	   https://github.com/akinanop/mvl-solver/blob/master/literature/Watched%20literals.pdf
	 */

	while ( current ) {

		// assign the satisfied literal from the clause to be watched1

		Clause* clause = CLAUSELIST[current->c_num];

		Literal* watched1 = clause -> ATOM_LIST[ clause -> W1];
		Literal* watched2 = NULL;

		if ( clause -> W2 > -1 ) watched2 = clause -> ATOM_LIST[ clause -> W2];

		Literal* literal = NULL;
		int index = 0;

		for ( int j = 0; j < clause -> NumAtom; j++ ) {

			Literal* temp = clause -> ATOM_LIST[j];

			if ( temp -> VAR == var &&  temp -> VAL == val &&
					temp -> EQUAL == equals ) {

				literal = CLAUSELIST[current->c_num] -> ATOM_LIST[j];
				index = j;
				break;
			}
		}

		if ( sat ( watched1 )  != 1 ) {

			if ( watched2 != NULL && ! LitIsEqual ( watched2, literal ) ) {

				clause -> W1 = index;

			} else {

				clause -> W2 = clause -> W1;
				clause -> W1 = index;
			}
		}
		current = current -> next;
	}
}

inline void Formula::watchedSatisfyLiteral( Literal * literal ) {

	// we don't need to  update counts any more

	watchedSatisfyLiteral ( literal -> VAR, literal -> EQUAL, literal -> VAL);
}

inline void Formula::tempwatchedSatisfyLiteral( Literal * literal ) {

	// we don't need to  update counts any more

	tempwatchedSatisfyLiteral ( literal -> VAR, literal -> EQUAL, literal -> VAL);
}


// falsify clauses with the literal
inline void Formula::watchedFalsifyLiteral ( Literal* literal ) {

	VARRECORD* current = NULL;

	int var = literal -> VAR;
	int val = literal -> VAL;
	bool equals = literal -> EQUAL;

	// Go through all occurrences of the literal in clauses and update watched literals

	if ( equals ) current = VARLIST[var] -> ATOMRECPOS[val];
	else current = VARLIST[var] -> ATOMRECNEG[val];

	while ( current ) {

		Literal* watched1 = CLAUSELIST[current -> c_num] -> WATCHED[0];
		Literal* watched2 = CLAUSELIST[current -> c_num] -> WATCHED[1];

		if ( sat ( watched1 ) != 1  &&
				( LitIsEqual ( watched1, literal ) ||
						( watched2 != NULL && LitIsEqual( watched2, literal ) ) ) ) SwapPointer ( CLAUSELIST[current -> c_num] );

		current = current -> next;
	}

	current = NULL;

}

inline void Formula::watchedFalsifyLiteral ( int var, bool equals, int val ) {


//	cout << "Falsifying " << var << ( equals ? "=" : "!" ) << val << endl;
	VARRECORD* current = NULL;

	// Go through all occurrences of the literal in clauses and update watched literals

	if ( equals ) current = VARLIST[var] -> ATOMRECPOS[val];
	else current = VARLIST[var] -> ATOMRECNEG[val];

	while ( current ) {

		// CLAUSELIST[current -> c_num] -> Print();

		Literal* watched1 = CLAUSELIST[current -> c_num] -> WATCHED[0];
		Literal* watched2 = CLAUSELIST[current -> c_num] -> WATCHED[1];

		if ( sat ( watched1 ) != 1 &&  watched2 != NULL  )  {

			/*
			 *if the clause is not sat, and literal = watched1 or literal = watched2,
			  it means that watched1 is falsified, need to replace it by a unassigned literal
			  if available
			 */

			 if ( watched1 -> VAR == var && watched1 -> VAL == val  && watched1 -> EQUAL == equals ) {
				SwapPointer ( current -> c_num );


			} else if ( watched2 -> VAR == var && watched2 -> VAL == val  && watched2 -> EQUAL == equals ) {

				SwapPointer ( current -> c_num);

			}
		}
		current = current -> next;
	}

	current = NULL;

}


inline void Formula::tempwatchedFalsifyLiteral ( int var, bool equals, int val ) {


//	cout << "Falsifying " << var << ( equals ? "=" : "!" ) << val << endl;
	VARRECORD* current = NULL;

	// Go through all occurrences of the literal in clauses and update watched literals

	if ( equals ) current = VARLIST[var] -> ATOMRECPOS[val];
	else current = VARLIST[var] -> ATOMRECNEG[val];

	while ( current ) {

		Clause* clause = CLAUSELIST[current -> c_num];

		Literal* watched1 = clause -> ATOM_LIST[clause -> W1];
		Literal* watched2 = NULL;

		if ( clause -> W2 != -1) {

			watched2 = clause -> ATOM_LIST[clause -> W2];

		}

		if ( sat ( watched1 ) != 1 &&  watched2 != NULL  )  {

			/*
			 *if the clause is not sat, and literal = watched1 or literal = watched2,
			  it means that watched1 is falsified, need to replace it by a unassigned literal
			  if available
			 */

			 if ( watched1 -> VAR == var && watched1 -> VAL == val  && watched1 -> EQUAL == equals ) {
				tempSwapPointer ( current -> c_num );


			} else if ( watched2 -> VAR == var && watched2 -> VAL == val  && watched2 -> EQUAL == equals ) {

				tempSwapPointer ( current -> c_num);

			}
		}
		current = current -> next;
	}

	current = NULL;

}

void Formula::watchedUndoTheory ( int level ) { // FIXME  fixed

	for ( unsigned int  i = 1; i < VARLIST.size(); i++ ) {

		for ( int j = 0; j < VARLIST[i] -> DOMAINSIZE; j++ ) {
			//if this domain has been assigned at level this or greater undo
			if ( VARLIST[i]->ATOMLEVEL[j] > level ) {

				// cout << "unassign " << i << " " << j <<endl;
				VARLIST[i] -> LEVEL = -1;
				VARLIST[i] -> SAT = false;
				VARLIST[i] -> VAL = -1;
				VARLIST[i]->ATOMASSIGN[j] = 0;
				VARLIST[i]->ATOMLEVEL[j] = -1;
				VARLIST[i]->CLAUSEID[j] = -10;

			}
		}
	}
	//undo the decision stack
	int decsize = DECSTACK.size();
	for ( int i = decsize - 1; i > -1; i-- ) {
		if ( VARLIST[DECSTACK[i] -> VAR] -> ATOMASSIGN[DECSTACK[i] -> VAL] == 0 ) {
			DECSTACK.erase ( DECSTACK.begin() + i );
		}
	}
}

void Formula::assignWatched (Clause* clause ) {

			clause -> WATCHED[0] = clause -> ATOM_LIST[LASTFALSE]; // latest falsified atom, to become unit upon backtrack

			if ( LOG ) {
				cout << "Latest falsified literal (now watched): " << endl;
				clause -> ATOM_LIST[LASTFALSE] -> Print ();
				cout << "Watched literal 1:"<<endl;
				clause -> WATCHED[0] -> Print();
			}

			Literal* watched1 = clause -> WATCHED[0];

			VARLIST[watched1 -> VAR] ->  ATOMWATCH[watched1 -> VAL] = 1;

			if ( clause -> NumAtom == 1 ) {
				clause -> WATCHED[1]  = NULL;
			}

			else if ( LASTFALSE < clause -> NumAtom - 1) {

				clause -> WATCHED[1] = clause -> ATOM_LIST[LASTFALSE + 1];
				Literal* watched2 = clause -> WATCHED[1];
				VARLIST[watched2 -> VAR] ->  ATOMWATCH[watched2 -> VAL] = 1;

			}
			else {
				clause -> WATCHED[1] = clause -> ATOM_LIST[LASTFALSE - 1];
				Literal* watched2 = clause -> WATCHED[1];
				VARLIST[watched2 -> VAR] ->  ATOMWATCH[watched2 -> VAL] = 1;

			}


		}



void Formula::tempassignWatched ( Clause* clause ) {


	// latest falsified atom, to become unit upon backtrack

	clause -> W1 = LASTFALSE;

	if ( LOG ) {
		cout << "Latest falsified literal (now watched): " << endl;
		clause -> ATOM_LIST[clause -> W1] -> Print ();

	}

	Literal* watched1 =  clause -> ATOM_LIST[clause -> W1];

	VARLIST[watched1 -> VAR] ->  ATOMWATCH[watched1 -> VAL] = 1;

	if ( clause -> NumAtom == 1 ) clause -> W2  = -1;

	else { // assign w2 next literal with a different variable

		bool flag = false;

		// start looking for a w2 from w1, then wrap around the tail:

		Literal* literal = NULL;

		for ( int i = clause -> W1 + 1; i < clause -> NumAtom; i++ ) {

			literal = clause -> ATOM_LIST[i];

			clause -> W2 = i;

			if ( literal -> VAR != watched1 -> VAR) {

				// clause -> watched2 = i;

				VARLIST[literal -> VAR] -> ATOMWATCH[literal -> VAL] = 2;
				flag = true;
				break;

			}
		}

		if ( ! flag ) {

			for ( int i =  0 ; i < clause -> W1 + 1; i++ ) {

				literal = clause -> ATOM_LIST[i];

				clause -> W2 = i;


				if ( literal -> VAR != watched1 -> VAR) {


					// clause -> watched2 = i;

					VARLIST[literal -> VAR] -> ATOMWATCH[literal -> VAL] = 2;
					break;

				}
			}
		}

		if ( LOG ) {
				cout << "watched2 in learned clause: " << endl;
				clause -> ATOM_LIST[clause -> W2] -> Print ();

			}

	}


}

//--------------- Resolution-based clause learning ---------------------//

Clause* Formula::analyzeConflict ( Clause * clause ) {

	/*
    First check if there is exactly one atom falsified at the current level.
    If yes, learn the clause: this way upon backtrack it becomes unit. Otherwise
    continue analyzing the conflict.
	 */

	if ( potent ( clause ) ) {

		// After backtracking the clause should be detected as unit
		clause -> NumUnAss = 0;

		if (LOG) {
			cout << "Learned a clause: " << endl;
			clause->Print();
		}
		// add the clause to the clause list
		CLAUSELIST.push_back( clause );
		// it's id
		int cid = CLAUSELIST.size()-1;
		// update global records for each atom in the clause
		for ( int i = 0; i < clause -> NumAtom; i++ ) {
			Literal* atom = clause->ATOM_LIST[i];
			VARLIST[atom->VAR] -> addRecord( cid, atom->VAL, atom->EQUAL);
		}

		if ( WATCH ) {
			clause -> WATCHED[0] = clause -> ATOM_LIST[LASTFALSE]; //  assignWatched ( clause );
			if ( clause -> NumAtom == 1 ) {
							clause -> WATCHED[1]  = NULL;
						}
		}

		if ( CMV ) {
			clause -> W1 = LASTFALSE; // tempassignWatched ( clause );
			if ( clause -> NumAtom == 1 ) {
									clause -> W2  = -1;
								}
		}

		return clause;

	}

	// Resolve the clause and its latest falsified literal's reason
	// Latest falsified literal in the clause:
	Literal* lastFalse = clause -> ATOM_LIST[maxLit ( clause )[0]];
	int var = lastFalse -> VAR;
	int val = lastFalse -> VAL;
	Clause * resolvent = new Clause();

	if ( LOG ) {
		cout << "Latest falsified literal: " << endl;
		lastFalse -> Print ();
		cout << "It's reason: " << endl;
		cout << VARLIST[var]-> CLAUSEID[val] << endl;
	}

	// Generate reasons for decision and entail reasons:
	Clause* reason = new Clause ();

	if ( VARLIST[var]-> CLAUSEID[val] == -1 ) {
		// Take literal in the decstack that falsified lastFalse
		Literal * falsifier = DECSTACK[maxLit ( clause )[1]];

		// Generate the reason clause:
		reason -> addAtom ( new Literal ( falsifier -> VAR, '=', falsifier -> VAL ) );
		reason -> addAtom ( new Literal ( falsifier -> VAR, '!', falsifier -> VAL ) );

	} else if ( VARLIST[var] -> CLAUSEID[val] == -2 ) {

		for ( int i = 0; i < VARLIST[var] -> DOMAINSIZE; i++ ) {
			reason -> addAtom ( new Literal ( var, '=', i ) );
		}
	} else {
		reason = CLAUSELIST[VARLIST[var] -> CLAUSEID[val]];
	}

	// resolve:
	resolvent = resolve ( clause, lastFalse, reason );

	if (LOG) {
		cout << "Resolvent:" << endl;
		resolvent -> Print();
	}
	return analyzeConflict ( resolvent );
}


//Finding the literal falsified the latest in the clause
vector<int> Formula::maxLit ( Clause* clause ) {

	vector<int> result;
	result.reserve(2);

	int atom_index = 0;
	int decision_index = 0;
	int max_decision_index = 0;

	for ( int i = 0; i < clause -> NumAtom; i++ ) {

		Literal * atom = clause -> ATOM_LIST[i];

		decision_index = VARLIST[atom -> VAR] -> ATOMINDEX[atom -> VAL];

		if ( max_decision_index < decision_index ) {

			max_decision_index = decision_index;
			atom_index = i;
		}

	}

	result[0] = atom_index; // atom in clause that is falsified last
	result[1] = max_decision_index; //  falsifier of the atom in the decstack

	return  result ;

}


bool Formula::falsifies ( Literal* literal, Literal* decision ) {

	// true if literal falsifies decision

	if ( literal -> VAR != decision -> VAR ) return false;
	else if ( literal -> VAL != decision -> VAL && literal -> EQUAL != decision -> EQUAL ) return false;
	else if ( literal -> VAL == decision -> VAL && literal -> EQUAL == decision -> EQUAL ) return false;

	else return true;
}

// Return unit literal from the unit clause
Literal* Formula::unitLiteral ( Clause* unit ) {

	for ( int i = 0; i < unit -> NumAtom; i++ ) {

		int lit_var = unit -> ATOM_LIST[i] -> VAR;
		int lit_val = unit -> ATOM_LIST[i] -> VAL;

		if ( VARLIST[lit_var] -> ATOMASSIGN[lit_val] == 0 )
			return unit -> ATOM_LIST[i];
	}

	// if no unit literal found, return the default literal -1 ! -1
	return new Literal ( -1, '!', -1 );

}


// WATCHED LITERALS ALGORITHM functions

int Formula::watchedCheckSat () {

	// returns 1 if all watched1 is sat (1), 0 if some watched1 and watched2 falsified (0), otherwise 2 -undefined (2)

	if (LOG) cout << "Checking satisfiability..." << endl;

     int counter = 0;

	for ( unsigned int  i = 0; i < CLAUSELIST.size(); i++ ) {

		Literal* watched1 = CLAUSELIST[i] -> WATCHED[0];
		Literal* watched2 = CLAUSELIST[i] -> WATCHED[1];


		if ( sat ( watched1 ) == 0
				&& (  watched2  == NULL || sat ( watched2 ) == 0 ) ) {

			if (LOG) cout << "Found conflict!" << endl;

			CONFLICTINGCLAUSE = i;
			CONFLICT = true;

			return 0;

		} else if ( sat ( watched1 ) == 2 &&  watched2 == NULL  ) counter++;
		  else if ( sat ( watched1 ) == 2   ) counter++ ;
	      else if ( watched2 != NULL && sat ( watched2 ) == 2   ) counter++ ;
	}

	if ( counter > 0 ) return 2;
	else return 1;
}

int Formula::tempwatchedCheckSat () {

	// returns 1 if all watched1 is sat (1), 0 if some watched1 and watched2 falsified (0), otherwise 2 -undefined (2)

	if (LOG) cout << "Checking satisfiability..." << endl;

     int counter = 0;

	for ( unsigned int  i = 0; i < CLAUSELIST.size(); i++ ) {

		Literal* watched1 = CLAUSELIST[i] -> ATOM_LIST[CLAUSELIST[i] -> W1];
		Literal* watched2 = NULL;

		if ( CLAUSELIST[i] -> W2 > -1 ) watched2 = CLAUSELIST[i] -> ATOM_LIST[CLAUSELIST[i] -> W2];

		if ( sat ( watched1 ) == 0
				&& (  watched2  == NULL || sat ( watched2 ) == 0 ) ) {

			if (LOG) cout << "Found conflict!" << endl;

			CONFLICTINGCLAUSE = i;
			CONFLICT = true;

			return 0;

		} else if ( sat ( watched1 ) == 2 &&  watched2 == NULL  ) counter++;
		  else if ( sat ( watched1 ) == 2   ) counter++ ;
	      else if ( watched2 != NULL && sat ( watched2 ) == 2   ) counter++ ;
	}

	if ( counter > 0 ) return 2;
	else return 1;
}



int Formula::sat ( Literal* literal ) {

	// computes literal -> SAT
	// literal -> SAT == 2 unassigned, == 1 satisfied, == 0 falsified

	int var = literal -> VAR;
	int val = literal -> VAL;
	int equals = 0;

	if ( literal -> EQUAL) equals = 1;
	else equals = -1;

	if ( VARLIST[var] -> ATOMASSIGN[val] == equals ) return 1; // literal is satisfied
	else if ( VARLIST[var] -> ATOMASSIGN[val] == 0 ) return 2; // unassigned
	else return 0; //  literal is falsified

}

Literal* Formula::tempwatchedCheckUnit () {

	//	if ( LOG ) cout << "Checking for units..." << endl;

	for ( unsigned int i = 0; i < CLAUSELIST.size(); i++ ) {

		Literal* watched1 = CLAUSELIST[i] -> ATOM_LIST[CLAUSELIST[i] -> W1];
		Literal* watched2 = NULL;

		if ( CLAUSELIST[i] -> W2 > -1 ) watched2 = CLAUSELIST[i] -> ATOM_LIST[CLAUSELIST[i] -> W2];

		if ( sat ( watched1 ) == 2 // if watched1 unassigned
				&& ( watched2 == NULL || sat ( watched2 ) == 0 ) ) {
			//	cout << "Found unit, watched1" << endl;
			UNITCLAUSE = i;
			return watched1;
		}

		else if ( sat ( watched1 ) == 0 && ( watched2 == NULL || sat ( watched2 ) == 0 ) ) {
					cout << "Found conflict" << endl;
					CONFLICTINGCLAUSE = i;
					CONFLICT = true;
					return NULL;
				}

		else if ( sat ( watched1 ) == 0 && sat ( watched2 ) == 2 ) {
			//	cout << "Found unit, watched2" << endl;
			UNITCLAUSE = i;
			return watched2;
		}
	}
	// if ( LOG ) cout << "No units..." << endl;
	return NULL;
}

Literal* Formula::watchedCheckUnit () {

//	if ( LOG ) cout << "Checking for units..." << endl;

	for ( unsigned int i = 0; i < CLAUSELIST.size(); i++ ) {

		Literal* watched1 = CLAUSELIST[i] -> WATCHED[0];
		Literal* watched2 = CLAUSELIST[i] -> WATCHED[1];

		if ( watched1 == NULL ) { ;
		}
		if ( sat ( watched1 ) == 2 // if watched1 unassigned
				&& ( watched2 == NULL || sat ( watched2 ) == 0 ) ) {
		//	cout << "Found unit, watched1" << endl;
			UNITCLAUSE = i;
			return watched1;
		}

	/*	else if ( sat ( watched1 ) == 0 && ( watched2 == NULL || sat ( watched2 ) == 0 ) ) {
					cout << "Found conflict" << endl;
					CONFLICTINGCLAUSE = i;
					CONFLICT = true;
					return NULL;
				} */

		else if ( sat ( watched1 ) == 0 && sat ( watched2 ) == 2 ) {
		//	cout << "Found unit, watched2" << endl;
			UNITCLAUSE = i;
			return watched2;
		}
	}
	// if ( LOG ) cout << "No units..." << endl;
	return NULL;
}


Literal* Formula::watchedChooseLiteral () {

	/*  Pick a literal which is not yet satisfied, and which satisfies more clauses and removes less literals,
	 *  use literal counts. Many other heuristics are possible.
	 */

	int max = INT_MIN;
	int tmax = -1;
	int tvar = -1;
	int tval = -1;
	UNITCLAUSE = -1;

	for ( unsigned int i = 0; i < VARLIST.size(); i++ ) {
		if ( ! VARLIST[i] -> SAT ) {
			for ( int j = 0; j < VARLIST[i] -> DOMAINSIZE; j++ ) {
				if ( VARLIST[i] -> ATOMASSIGN[j] == 0 && VARLIST[i] -> ATOMWATCH[j] == 1) {

					tmax = VARLIST[i] -> ATOMCNTPOS[j] - VARLIST[i] -> ATOMCNTNEG[j];

					if ( max < tmax ) {
						max = tmax;
						tvar = i;
						tval = j;
					}
				}
			}
		}
	}
	return ( tvar != -1 ? new Literal ( tvar, tval ) : lazyWatchedChooseLiteral () );
}


Literal * Formula::lazyWatchedChooseLiteral () {

	// Choose any unassigned first watched literal


	for ( unsigned int  i = 0; i < CLAUSELIST.size(); i++ ) {

		Literal* watched1 = CLAUSELIST[i] -> WATCHED[0];

		if ( sat ( watched1 ) == 2) return  new Literal(watched1 -> VAR, '=', watched1 ->VAL);
	}
	return NULL;
}

Literal * Formula::templazyWatchedChooseLiteral () {

	// Choose any unassigned first watched literal

	for ( unsigned int  i = 0; i < CLAUSELIST.size(); i++ ) {

		Literal* watched1 = CLAUSELIST[i] -> ATOM_LIST[CLAUSELIST[i] -> W1];

		if ( sat ( watched1 ) == 2) return  new Literal(watched1 -> VAR, '=', watched1 ->VAL);
	}
	return NULL;
}


void Formula::SwapPointer ( Clause* clause ) {

	Literal* watched1 = clause -> WATCHED[0];
	Literal* watched2 = clause -> WATCHED[1];


	if ( watched2 != NULL && sat ( watched2 )  == 2 ) {

		// if watched2 unassigned, watched1 falsified

		for ( int i = 0; i < clause -> NumAtom; i++ ) {

			Literal* literal = clause -> ATOM_LIST[i];

			if (  VARLIST[literal -> VAR] -> ATOMASSIGN[literal -> VAL] == 0 && ! LitIsEqual ( literal, watched2 ) ) {


				clause -> WATCHED[0] = literal;

				VARLIST[literal -> VAR] -> ATOMWATCH[literal -> VAL] = 1;
				break;
			}
		}
	} else if ( watched2 != NULL && sat ( watched1 )  == 2  ) {

	//	cout << "watched1 unassigned" << endl;

		for ( int i; i < clause -> NumAtom; i++ ) {

			Literal* literal = clause -> ATOM_LIST[i];

			if ( sat ( literal ) == 2 && ! LitIsEqual ( literal, watched1 ) ) {
				clause -> WATCHED[1] = literal;
				VARLIST[literal -> VAR] -> ATOMWATCH[literal -> VAL] = 2;
			}
		}
	} else {
		CONFLICT = true;
	}
}

void Formula::SwapPointer ( int clause_num ) {

    Clause* clause = CLAUSELIST[clause_num];

	Literal* watched1 = clause -> WATCHED[0];
	Literal* watched2 = clause -> WATCHED[1];


	if ( watched2 != NULL && sat ( watched2 )  == 2 ) {

		// if watched2 unassigned, watched1 falsified

		for ( int i = 0; i < clause -> NumAtom; i++ ) {

			Literal* literal = clause -> ATOM_LIST[i];

			if (  VARLIST[literal -> VAR] -> ATOMASSIGN[literal -> VAL] == 0 && ! LitIsEqual ( literal, watched2 ) ) {


				clause -> WATCHED[0] = literal;

				VARLIST[literal -> VAR] -> ATOMWATCH[literal -> VAL] = 1;
				break;
			}
		}
	} else if ( watched2 != NULL && sat ( watched1 )  == 2  ) {

		for ( int i = 0; i < clause -> NumAtom; i++ ) {

			Literal* literal = clause -> ATOM_LIST[i];

			if ( sat ( literal ) == 2 && ! LitIsEqual ( literal, watched1 ) ) {
				clause -> WATCHED[1] = literal;
				VARLIST[literal -> VAR] -> ATOMWATCH[literal -> VAL] = 2;
			}
		}
	} else if ( sat ( watched2 ) == 0 && sat ( watched1 )  == 0 ) {
		if ( LOG ) cout << "conflict" << endl;
	}


		/* if ( sat ( clause -> WATCHED[0] ) == 0 && ( clause -> WATCHED[1] == NULL || sat ( clause -> WATCHED[1] ) == 0 ) ) {
		CONFLICT = true;
		CONFLICTINGCLAUSE = clause_num; */
		// cout << "conflict"<< endl;
//	}

}


void Formula::tempSwapPointer ( int clause_num ) {

	Clause* clause = CLAUSELIST[clause_num];

	Literal* watched1 = clause -> ATOM_LIST[clause -> W1];
	Literal* watched2 = clause -> ATOM_LIST[clause -> W2];

	bool flag = false;

	if ( watched2 != NULL && sat ( watched2 )  == 2 ) {

		// if watched2 unassigned, watched1 falsified

		// start looking for a replacement w1 from w1, then wrap around the tail:

		for ( int i = clause -> W1 + 1; i < clause -> NumAtom; i++ ) {

			Literal* literal = clause -> ATOM_LIST[i];

			if (  VARLIST[literal -> VAR] -> ATOMASSIGN[literal -> VAL] == 0 && ! LitIsEqual ( literal, watched2 ) ) {

				if ( literal -> VAR != watched2 -> VAR ) {

					clause -> W1 = i;
				}

				clause -> W1 = i;

				VARLIST[literal -> VAR] -> ATOMWATCH[literal -> VAL] = 1;


				if ( literal -> VAR == watched1 -> VAR ) {

					flag = true;
					break;
				}
			}
		}

		if ( ! flag ) {

			for ( int i =  0 ; i < clause -> W1 + 1; i++ ) {

				Literal* literal = clause -> ATOM_LIST[i];

				if (  VARLIST[literal -> VAR] -> ATOMASSIGN[literal -> VAL] == 0 && ! LitIsEqual ( literal, watched2 ) ) {


					if ( literal -> VAR != watched2 -> VAR ) {

						clause -> W1 = i;
					}

					clause -> W1 = i;

					VARLIST[literal -> VAR] -> ATOMWATCH[literal -> VAL] = 1;

					if ( literal -> VAR == watched1 -> VAR ) {

						flag = true;
						break;
					}

				}
			}
		}

	} else if ( watched2 != NULL && sat ( watched1 )  == 2  ) {

		for ( int i = clause -> W2 + 1; i < clause -> NumAtom; i++ ) {

			Literal* literal = clause -> ATOM_LIST[i];

			if ( sat ( literal ) == 2 && ! LitIsEqual ( literal, watched1 ) ) {

				if ( literal -> VAR != watched1 -> VAR ) {

					clause -> W2 = i;
				}

				clause -> W2 = i;

				VARLIST[literal -> VAR] -> ATOMWATCH[literal -> VAL] = 2;

				if ( literal -> VAR == watched2 -> VAR ) {

					flag = true;
					break;
				}

			}
		}

		if ( ! flag ) {

			for ( int i = 0; i < clause -> W2 + 1; i++ ) {

				Literal* literal = clause -> ATOM_LIST[i];

				if ( sat ( literal ) == 2 && ! LitIsEqual ( literal, watched1 ) ) {

					if ( literal -> VAR != watched1 -> VAR ) {

						clause -> W2 = i;
					}


					clause -> W2 = i;
					VARLIST[literal -> VAR] -> ATOMWATCH[literal -> VAL] = 2;

					if ( literal -> VAR == watched2 -> VAR ) {

						flag = true;
						break;
					}

					if ( literal -> VAR != watched1 -> VAR ) {
						clause -> W2 = i;
					}
				}
			}
		}
	}
}

bool Formula::LitIsEqual ( Literal* literal1, Literal * literal2 ) {

	if ( literal1 -> VAR == literal2 -> VAR && literal1 -> VAL == literal2 -> VAL
			&& literal1 -> EQUAL == literal2 -> EQUAL ) return true;

	else return false;
}

void Formula::watchedReduceTheory ( Literal * literal, int var, bool equals, int val ) {

	if ( equals ) {

		if ( LOG ) cout << var << "=" << val << " at level " << LEVEL << endl;

		VARLIST[var] -> ATOMASSIGN[val] = 1;
		VARLIST[var] -> ATOMLEVEL[val] = LEVEL;
		VARLIST[var] -> VAL = val;
		VARLIST[var] -> SAT = true; // means variable is assigned. REDUNDANT!
		VARLIST[var] -> LEVEL = LEVEL; // value assigned a positive value at this level
		VARLIST[var] -> CLAUSEID[val] = UNITCLAUSE; 		// Set the reason for the literal
		DECSTACK.push_back(literal); 		// Add literal to the decision stack
		VARLIST[var] -> ATOMINDEX[val] = DECSTACK.size() - 1; // to use in maxLit

		// COUNTER = # of falsified values
		VARLIST[var] -> COUNTER = VARLIST[var] -> DOMAINSIZE -1; // all other values are falsified: no real need for counter here ! (?)

		if ( LOG ) {
			cout << "The reason for the literal: " << endl;
			if ( UNITCLAUSE > -1 ) CLAUSELIST[UNITCLAUSE] -> Print();
			else cout << UNITCLAUSE << endl;
		}

		// update watched literals:
		if ( CMV ) {
			tempwatchedSatisfyLiteral ( var, equals, val ); // set the literal to watched1 - then checkSat == 1
			tempwatchedFalsifyLiteral ( var, ! equals, val ); // if one of the watched literals falsified, swap watched literals
		} else {

			watchedSatisfyLiteral ( var, equals, val ); // set the literal to watched1 - then checkSat == 1
			watchedFalsifyLiteral ( var, ! equals, val ); // if one of the watched literals falsified, swap watched literals
		}

		//for each different domain value x from dom(var) which is not assigned (0) assign it

		for ( int i = 0; i < val && ! CONFLICT; i++ ) {

			if ( VARLIST[var] -> ATOMASSIGN[i] == 0 ) {

				VARLIST[var]->ATOMASSIGN[i] = -1;
				VARLIST[var]->ATOMLEVEL[i] = LEVEL;

				// Set the same reason:
				VARLIST[var]->CLAUSEID[i] = UNITCLAUSE;
				VARLIST[var] -> ATOMINDEX[i] = DECSTACK.size() - 1; // to use in maxLit


				if ( CMV ) {

					tempwatchedSatisfyLiteral ( var, ! equals, i ); // set the literal to watched1 - then checkSat == 1
					tempwatchedFalsifyLiteral ( var, equals, i ); // if one of the watched literals falsified, swap watched literals
				} else {

					watchedSatisfyLiteral ( var, !equals, i ); // set the literal to watched1 - then checkSat == 1
					watchedFalsifyLiteral ( var, equals, i ); // if one of the watched literals falsified, swap watched literals
				}


			}
		}

		for ( int i = val + 1; i < VARLIST[var] -> DOMAINSIZE && ! CONFLICT; i++ ) {

			if ( VARLIST[var] -> ATOMASSIGN[i] == 0 ) {

				VARLIST[var]->ATOMASSIGN[i] = -1;
				VARLIST[var]->ATOMLEVEL[i] = LEVEL;

				VARLIST[var] -> CLAUSEID[i] = UNITCLAUSE;
				VARLIST[var] -> ATOMINDEX[i] = DECSTACK.size() - 1; // to use in maxLit

				if ( CMV ) {

					tempwatchedSatisfyLiteral ( var, ! equals, i ); // set the literal to watched1 - then checkSat == 1
					tempwatchedFalsifyLiteral ( var, equals, i ); // if one of the watched literals falsified, swap watched literals
				} else {

					watchedSatisfyLiteral ( var, !equals, i ); // set the literal to watched1 - then checkSat == 1
					watchedFalsifyLiteral ( var, equals, i ); // if one of the watched literals falsified, swap watched literals
				}
			}
		}
	} else {

		if ( LOG ) cout << var << "!" << val << " at level " << LEVEL << endl;

		VARLIST[var] -> ATOMASSIGN[val] = -1;
		VARLIST[var] -> ATOMLEVEL[val] = LEVEL;
		// Set the reason:
		VARLIST[var] -> CLAUSEID[val] = UNITCLAUSE;
		//Add literal to decision stack:
		DECSTACK.push_back ( literal );
		VARLIST[var] -> ATOMINDEX[val] = DECSTACK.size() - 1; // to use in maxLit

		// one more value falsified
		VARLIST[var] -> COUNTER++;

		if ( CMV ) {
			tempwatchedSatisfyLiteral ( var, equals, val );
			tempwatchedFalsifyLiteral ( var, ! equals, val );
		} else {

			watchedSatisfyLiteral ( var, equals, val );
			watchedFalsifyLiteral ( var, ! equals, val );
		}


		//check Entailment on this variable
		if ( checkEntail ( var ) ) {
			if ( LOG ) cout << "Entailment... " << ENTAILLITERAL -> VAR << "=" << ENTAILLITERAL -> VAL << endl;
			ENTAILS++;
			// Set the reason:
			UNITCLAUSE = -2;
			watchedReduceTheory ( ENTAILLITERAL, ENTAILLITERAL -> VAR, true, ENTAILLITERAL -> VAL );
		}

	}

}
//=================== Watched literals NON-CHRONOLOGICAL BACKTRACK ============================//

int Formula::WatchedLiterals ( int restarts ) {

	int restartCount = restarts;

	// returns 0 if sat, 1 if timeout, 2 if unsat

	if ( LOG ) cout << "Solving with the watched literal algorithm..." << endl;

	while ( true ) {
		//Check if the theory satisfied
		if ( watchedCheckSat() == 1 )
			return 0; //

		//Check if time out
		TIME_E = GetTime();
	//	if ( ( TIME_E - TIME_S ) > TIMELIMIT )
		//	return 1;

		if ( CONFLICT ) {

			if ( LOG ) {
				cout << "There is a conflict at level: " << LEVEL << endl;
				cout<<"Conflicting clause: "<<  endl;
				CLAUSELIST[CONFLICTINGCLAUSE] -> Print ();
			}


			if ( LEVEL == 0 ) { cout << "UNSAT" << endl; return 2; }

			Clause* learned = analyzeConflict ( CLAUSELIST[CONFLICTINGCLAUSE] );
			LEVEL = backtrackLevel ( learned );

			if ( learned -> NumAtom > 1) {
			learned -> WATCHED[1] = learned -> ATOM_LIST[WATCHED2];
			}

			BACKTRACKS++;
			if ( LOG ) {
				cout << "We are backtracking to the level: " << LEVEL << endl;
				cout << "# of backtracks so far: " << BACKTRACKS << endl;
			}
			CONFLICT = false;

			if ( BACKTRACKS == restartCount ) {
				//	cout << "restarting" << endl;
				watchedUndoTheory ( 0 );
				restartCount = BACKTRACKS + restarts;
				RESTARTS++;
			}
			else watchedUndoTheory ( LEVEL );
					}
		// If there is a unit clause, propagate
		Literal* unit = watchedCheckUnit ();
		if ( unit ) {
		/*	if ( LOG ) {
				cout << "Found unit!" << endl;
				unit -> Print();
				cout << "Unit clause: "<<endl;
				CLAUSELIST[UNITCLAUSE] -> Print();
			} */
			watchedReduceTheory ( unit, unit -> VAR, unit -> EQUAL, unit -> VAL );

		}
		// otherwise choose a literal and propagate - no need for separate unit propagation
		else if ( ! CONFLICT ) {
			Literal * atom = chooseLiteral ();
			if ( atom ) {
				DECISIONS++;
				LEVEL++;
				// set REASON for subsequent falsified atoms
				UNITCLAUSE = -1;
				if ( LOG ) cout << "Decision: " << atom -> VAR << ( atom -> EQUAL ? '=' : '!' ) << atom -> VAL << endl;
				watchedReduceTheory ( atom, atom -> VAR, atom -> EQUAL, atom -> VAL );
			}

		}
	}

}


int Formula::tempWatchedLiterals ( int restarts ) {

	int restartCount = restarts;

	// returns 0 if sat, 1 if timeout, 2 if unsat

	if ( LOG ) cout << "Solving with the watched literal algorithm..." << endl;

	while ( true ) {
		//Check if the theory satisfied
		if ( tempwatchedCheckSat() == 1 )
			return 0; //

		//Check if time out
		TIME_E = GetTime();
	//	if ( ( TIME_E - TIME_S ) > TIMELIMIT )
		//	return 1;

		if ( CONFLICT ) {

			if ( LOG ) {
				cout << "There is a conflict at level: " << LEVEL << endl;
				cout<<"Conflicting clause: "<<  endl;
				CLAUSELIST[CONFLICTINGCLAUSE] -> Print ();
			}


			if ( LEVEL == 0 ) { cout << "UNSAT" << endl; return 2; }

			Clause* learned = analyzeConflict ( CLAUSELIST[CONFLICTINGCLAUSE] );
			LEVEL = backtrackLevel ( learned );

			if ( learned -> NumAtom > 1) {
				learned -> W2 = WATCHED2;
			}


			BACKTRACKS++;
			if ( LOG ) {
				cout << "We are backtracking to the level: " << LEVEL << endl;
				cout << "# of backtracks so far: " << BACKTRACKS << endl;
			}
			CONFLICT = false;

			if ( BACKTRACKS == restartCount ) {
				//	cout << "restarting" << endl;
				watchedUndoTheory ( 0 );
				restartCount = BACKTRACKS + restarts;
				RESTARTS++;
			}
			else watchedUndoTheory ( LEVEL );
					}
		// If there is a unit clause, propagate
		Literal* unit = tempwatchedCheckUnit ();
		if ( unit ) {
		/*	if ( LOG ) {
				cout << "Found unit!" << endl;
				unit -> Print();
				cout << "Unit clause: "<<endl;
				CLAUSELIST[UNITCLAUSE] -> Print();
			} */
			watchedReduceTheory ( unit, unit -> VAR, unit -> EQUAL, unit -> VAL );

		}
		// otherwise choose a literal and propagate - no need for separate unit propagation
		else if ( ! CONFLICT ) {
			Literal * atom = chooseLiteral ();
			if ( atom ) {
				DECISIONS++;
				LEVEL++;
				// set REASON for subsequent falsified atoms
				UNITCLAUSE = -1;
				if ( LOG ) cout << "Decision: " << atom -> VAR << ( atom -> EQUAL ? '=' : '!' ) << atom -> VAL << endl;
				watchedReduceTheory ( atom, atom -> VAR, atom -> EQUAL, atom -> VAL );
			}

		}
	}

}

// // Resolution-based non-chronological backtracking algorithm  with restarts
int Formula::NonChronoBacktrack ( int restarts ) {

	int restartCount = restarts;

	//start of finite domain extended dpll
	// return 0 : if theory satisfied
	// return 1 : if time out
	// return 2 : if CONFLICT and later used as unsatisfied

	while ( true ){

		if ( checkSat() )
			return 0;

		//Check if time out
		TIME_E = GetTime();
		if ( (TIME_E - TIME_S ) > TIMELIMIT )
			return 1;

		if ( CONFLICT ) {

			if ( LOG ) {
				cout << "There is a conflict at level: " << LEVEL << endl;
				cout << "Conflicting clause: " <<  endl;
				CLAUSELIST[CONFLICTINGCLAUSE] -> Print();
			}

			if ( LEVEL == 0 ) return 2;

			LEVEL = backtrackLevel ( analyzeConflict ( CLAUSELIST[CONFLICTINGCLAUSE]) );

			if ( LOG ) cout << "We are backtracking to the level: " << LEVEL << endl;

			BACKTRACKS++;
			CONFLICT = false;

			if ( LOG ) cout << "# of backtracks so far: " << BACKTRACKS << endl;


			if ( BACKTRACKS == restartCount ) {
				undoTheory ( 0 );
				restartCount = BACKTRACKS + restarts;
				RESTARTS++;
			}
			else undoTheory ( LEVEL );
		}

		// If there is a unit clause, propagate

		checkUnit();

		if ( !UNITLIST.empty() )
			unitPropagation();

		// otherwise choose a literal and propagate

			Literal* atom = chooseLiteral();

			if ( atom ) {

				if (LOG) cout << "Decision: " << atom -> VAR << ( atom -> EQUAL ? '=' : '!' ) << atom -> VAL << endl;

				DECISIONS++;
				LEVEL++;
				UNITCLAUSE = -1; // REASON for subsequent falsified atoms

				reduceTheory ( atom -> VAR, atom -> EQUAL, atom -> VAL);
			}

	}

}

// Resolution-based non-chronological backtracking algorithm

int Formula::NonChronoBacktrack() {

	/* start of finite domain extended dpll
	 return 0 : if theory satisfied
	 return 1 : if time out
	 return 2 : if CONFLICT and later used as unsatisfied
	 */

	while ( true ) {

		if ( checkSat() )
			return 0;

		//Check if time out
		TIME_E = GetTime();
		if ( ( TIME_E - TIME_S ) > TIMELIMIT )
			return 1;

		if ( CONFLICT ) {

			if ( LOG ) {
				cout << "There is a conflict at level: " << LEVEL << endl;
				cout<<"Conflicting clause: "<<  endl;
				CLAUSELIST[CONFLICTINGCLAUSE] -> Print();
			}

			if ( LEVEL == 0 ) return 2; // the formula is unsat

			LEVEL = backtrackLevel ( analyzeConflict ( CLAUSELIST[CONFLICTINGCLAUSE]) );

			BACKTRACKS++;

			if ( LOG ) {
				cout << "We are backtracking to the level: " << LEVEL << endl;
				cout << "# of backtracks so far: " << BACKTRACKS << endl;
			}

			CONFLICT = false;
			undoTheory(LEVEL);
		}

		// If there is a unit clause, propagate
		checkUnit();

		if ( !UNITLIST.empty() )
			unitPropagation();

		// otherwise choose a literal and propagate
		if ( !CONFLICT ) {

			Literal* atom = chooseLiteral();
			if ( atom ) {
				DECISIONS++;
				LEVEL++;

				if (LOG) cout << "Decision: " << atom -> VAR << ( atom -> EQUAL ? '=' : '!' ) << atom -> VAL << endl;

				UNITCLAUSE = -1; // REASON for the subsequent falsified atoms

				reduceTheory ( atom -> VAR, atom -> EQUAL, atom -> VAL );
			}

		}
	}

}

// Chronological backtrack algorithm

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
