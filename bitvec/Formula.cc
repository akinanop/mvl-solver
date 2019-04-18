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
	VSIDS=false;
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
	VSIDS = cline->VSIDS;
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
	Literal temp_atom;
	Clause * temp_clause = NULL;

	//to start variable from 1 and not from 0 we need this
	VARLIST.push_back( new Variable(0, 0));


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

			if (val>MAX_DOMAIN) {
				cout << "Error: domain exceeds MAX_DOMAIN"<<endl;
			}
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
						bitset<MAX_DOMAIN> vals;
						if (ch == '=') {
							vals.set(val);
						} else {
							vals = VARLIST[var]->CURRENT_DOMAIN;
							vals.reset(val);
						}

						//add it to clause_list's atom_list
						temp_atom = Literal(var, vals);
						temp_clause->addAtom(temp_atom);

						//increment the index representing value so that
						//we can keep track of number of occurrences of each
						//variable and its domain value, if ch is = then increment
						//the value count, else increment others count

						atom_num++;
					}
				}
			}while(*lp);
			CLAUSELIST.push_back(temp_clause);

			for (int i=0;i<temp_clause->ATOM_LIST.size();i++) {
				Literal lit = temp_clause->ATOM_LIST[i];
				for (int j=0;j<VARLIST[lit.VAR]->DOMAINSIZE;j++) {
					if (lit.VALS.test(j)) {
						VARLIST[lit.VAR]->ATOMCNTPOS[j]++;
					}
				}
			}

			if (temp_clause->NumAtom == 1) {
				UNITLIST.push_back(clause_num);
			}

			// with watched literals option, assign first two literals in the clause to watched1 and watched2



			Literal watched1 = temp_clause -> ATOM_LIST[0];
			temp_clause -> WATCHED[0] = watched1;
			VARLIST[watched1.VAR]->addRecord(clause_num);

			if ( temp_clause -> NumAtom > 1 ) {

				Literal watched2 = temp_clause -> ATOM_LIST[1];
				temp_clause -> WATCHED[1] = watched2;
				VARLIST[watched2.VAR]->addRecord(clause_num);

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
void Formula::PrintModel()
{
	for(unsigned int  i=0; i<VARLIST.size();i++)
	{
		cout <<i << "=" << VARLIST[i]->CURRENT_DOMAIN << endl;
	}
	if (LOG) cout<<endl;
}

//PrintInfo
void Formula::PrintInfo()
{
	printf("\n");
	printf("Decisions   : %d\n", DECISIONS);
	printf("Units       : %d\n", UNITS);
	printf("Backtracks  : %d\n", BACKTRACKS);
	printf("Entails     : %d\n", ENTAILS);
	//  printf("Number of Levels      : %d\n", LEVEL);
	printf( "Variables   : %zu\n", VARLIST.size()-1 );
	printf( "Clauses     : %zu\n", CLAUSELIST.size() );
	printf("Restarts   : %d\n", RESTARTS);

	printf("\n");
}


bool Formula::verifyModel() {
	//for each clause check whether at least one literal is satisfied
	int var = 0;
	int domainsize = 0;
	VARRECORD * curr = NULL;
	bool sat;
	Literal lit;
	for(unsigned int  i=0; i<CLAUSELIST.size(); i++) {
		sat=false;
		for (int j=0;j<CLAUSELIST[i]->ATOM_LIST.size();j++) {
			lit = CLAUSELIST[i]->ATOM_LIST[j];
			if (lit.superset(VARLIST[lit.VAR]->CURRENT_DOMAIN)) {
				sat=true;
				break;
			}
		}
		if (!sat)
			return false;
	}
	return true;
}

// Functions used in the extended DPLL algorithm:

Literal Formula::chooseLiteralVSIDS () {

	/*  Pick a literal which is not yet satisfied, and which has a maximal vsids counter
	 *
	 *  Negative literals are counted as disjunctions of positive literals
	 */


	int max = INT_MIN;
	int tmax = -1;
	int tvar = -1;
	int tval = -1;
	UNITCLAUSE = -1;

	for ( int i = 0; i < VARLIST.size(); i++ ) {
		if ( VARLIST[i] -> CURRENT_DOMAIN.count() > 1 ) {
			for ( int j = 0; j < VARLIST[i] -> DOMAINSIZE; j++ ) {
				if ( VARLIST[i] -> CURRENT_DOMAIN.test(j) ) {
					tmax = VARLIST[i]->ATOMCNTPOS[j];

					if (max < tmax) {
						max = tmax;
						tvar = i;
						tval = j;
					}
				}
			}
		}
	}
	return Literal ( tvar, tval );
}

Clause* Formula::resolve ( Clause* clause, Literal literal, Clause* reason ) {

	//  Performs extended resolution on clause and reason wrt literal.
	// (See pp.47-50 of Jain: https://github.com/akinanop/mvl-solver/blob/master/literature/Jain_thesis.pdf)

	Clause * resolvent = new Clause();

	// Add literals L from the clause that are satisfied by at least one interpretation that does not satisfy L:

	for ( unsigned int i = 0; i < clause -> ATOM_LIST.size(); i++ ) {

		Literal c_atom = clause -> ATOM_LIST[i];

		if ( c_atom . VAR != literal . VAR ) resolvent -> addAtom ( c_atom );

	}
	// Add literals L from the reason clause that are satisfied by at least one interpretation that also satisfies L, i.e., doesn't falsify.

	for ( unsigned int  i = 0; i < reason -> ATOM_LIST.size(); i++ ) {

		Literal r_atom = reason -> ATOM_LIST[i];

		if ( r_atom.VAR != literal.VAR ) resolvent -> addAtom ( r_atom );
		else {
			bitset<MAX_DOMAIN> res = literal.VALS & r_atom.VALS;
			if (res.any()) {
				resolvent->addAtom(Literal(literal.VAR, res));
			}
		}
	}

	return resolvent;
}



bool Formula::potent ( Clause* clause ) {

	// Check if there is exactly one atom falsified at the current level

	if (clause->NumAtom == 1) {
		return true;
	}

	Literal w2 = clause->WATCHED[1];
	if (VARLIST[w2.VAR]->getIGNode(w2.VALS).level < LEVEL) {
		return true;
	}
	return false;

}

// Corresponding functions for watched literals version of the algorithm:

inline void Formula::watchedUpdateVar ( int var ) {

	// get clauses with the literal
	VARRECORD * current = VARLIST[var] -> VARREC;
	VARRECORD * next;

	/*
	 * for every clause that contains this literal update watched literals, as described here:
	   https://github.com/akinanop/mvl-solver/blob/master/literature/Watched%20literals.pdf
	 */

	while ( current && !CONFLICT ) {
		next = current->next;

		if (sat(CLAUSELIST[current->c_num]->WATCHED[0])!=1) {
			if (sat(CLAUSELIST[current->c_num]->WATCHED[1])==1) {
				Literal lit = CLAUSELIST[current->c_num]->WATCHED[1];
				CLAUSELIST[current->c_num]->WATCHED[1] = CLAUSELIST[current->c_num]->WATCHED[0];
				CLAUSELIST[current->c_num]->WATCHED[0] = lit;
			} else {
				if (CLAUSELIST[current->c_num]->WATCHED[1].VAR == var) {
					Literal lit = CLAUSELIST[current->c_num]->WATCHED[1];
					CLAUSELIST[current->c_num]->WATCHED[1] = CLAUSELIST[current->c_num]->WATCHED[0];
					CLAUSELIST[current->c_num]->WATCHED[0] = lit;
				}
				if (sat(CLAUSELIST[current->c_num]->WATCHED[0]) == 0) {
					SwapPointer(current);
				}
			}
		}
		current = next;
	}
}

void Formula::watchedUndoTheory ( int level ) { // FIXME  fixed ?

	for ( unsigned int  i = 1; i < VARLIST.size(); i++ ) {

		while (!VARLIST[i]->graphNodes.empty() && VARLIST[i]->graphNodes.back().level>LEVEL) {

			VARLIST[i]->CURRENT_DOMAIN = VARLIST[i]->CURRENT_DOMAIN | VARLIST[i]->graphNodes.back().values;

			VARLIST[i]->graphNodes.pop_back();

		}

	}
	//undo the decision stack
	int decsize = DECSTACK.size();
	for ( int i = decsize - 1; i > -1; i-- ) {
		if ( sat(DECSTACK[i]) == 2 ) {
			DECSTACK.erase ( DECSTACK.begin() + i );
		}
	}
}

//--------------- Resolution-based clause learning ---------------------//

Clause* Formula::analyzeConflict ( Clause * clause, bool freeClauseAfterUse ) {

	/*
    First check if there is exactly one atom falsified at the current level.
    If yes, learn the clause: this way upon backtrack it becomes unit. Otherwise
    continue analyzing the conflict.
	 */

	if ( potent ( clause ) ) {

		// After backtracking the clause should be detected as unit

		if (LOG) {
			cout << "Learned a clause: " << endl;
			clause->Print();
		}
		// add the clause to the clause list
		CLAUSELIST.push_back( clause );
		// it's id
		int cid = CLAUSELIST.size()-1;

		VARLIST[clause->WATCHED[0].VAR] -> addRecord( cid );
		if (clause->NumAtom>1)
			VARLIST[clause->WATCHED[1].VAR] -> addRecord( cid );

		// update global records for each atom in the clause
		for ( int i = 0; i < clause -> NumAtom; i++ ) {
			Literal atom = clause->ATOM_LIST[i];
		//	VARLIST[atom.VAR] -> addRecord( cid );

			for (int j = 0; j < VARLIST[atom.VAR]->DOMAINSIZE; j++) {
				if (atom.VALS.test(j))
					VARLIST[atom.VAR]->ATOMCNTPOS[j]++;
			}
		}

		for (int i = 0; i < VARLIST.size(); i++) {
			for (int j = 0; j < VARLIST[i]->DOMAINSIZE; j++) {
				VARLIST[i]->ATOMCNTPOS[j] /= 2;
			}
		}

		return clause;

	}

	// Resolve the clause and its latest falsified literal's reason
	// Latest falsified literal in the clause:
	Literal lastFalse;
	lastFalse = clause->WATCHED[0];
	int var = lastFalse . VAR;


	// Generate reasons for decision and entail reasons:
	Clause* reason = (CLAUSELIST[VARLIST[var] -> getIGNode(lastFalse.VALS).reason]);

	if ( LOG ) {
		cout << "Latest falsified literal: " << endl;
		lastFalse . Print ();
		cout << "It's reason: " << endl;
		reason->Print();
	}

	// resolve:
	Clause * resolvent = resolve ( clause, lastFalse, reason );

	assignWatched(resolvent);

	if (freeClauseAfterUse) {
		delete clause;
	}

	//cout << "Clause size: " << resolvent -> NumAtom << endl;

	if (LOG) {
		cout << "Resolvent:" << endl;
		resolvent -> Print();
	}
	return analyzeConflict ( resolvent, true );
}

void Formula::assignWatched( Clause* clause) {

	Literal w1;
	Literal w2;
	int decision_index = -1;
	int max_decision_index = -1;
	int second_max_decision_index = -1;

	for ( int i = 0; i < clause -> NumAtom; i++ ) {

		Literal atom = clause -> ATOM_LIST[i];

		decision_index = VARLIST[atom . VAR] -> getIGNode(atom.VALS).index;

		if ( max_decision_index < decision_index ) {
			second_max_decision_index = max_decision_index;
			max_decision_index = decision_index;
			w2 = w1;
			w1 = atom;
		} else if (second_max_decision_index < decision_index) {
			second_max_decision_index = decision_index;
			w2 = atom;
		}
	}
	clause->WATCHED[0] = w1;
	clause->WATCHED[1] = w2;
}

// WATCHED LITERALS ALGORITHM functions

int Formula::sat ( Literal literal ) {

	// computes literal -> SAT
	// literal -> SAT == 2 unassigned, == 1 satisfied, == 0 falsified

	if ( literal.superset(VARLIST[literal . VAR] -> CURRENT_DOMAIN ) ) return 1; // literal is satisfied
	else if ( literal.falsifies(VARLIST[literal . VAR] -> CURRENT_DOMAIN ) ) return 0; // falsified
	else return 2; //  literal is undefined

}

void Formula::WatchedUnitPropagation()
{
	int unit_clause = -1;

	//while there is no conflict and there exists a unit clause
	//find the unit literal and satisfy it
	while( ! CONFLICT && ! UNITLIST.empty() ) {

		unit_clause = UNITLIST.front();
		UNITCLAUSE = unit_clause;

		UNITLIST.pop_front();

		Clause* clause = CLAUSELIST[unit_clause];
		if (LOG) {
			cout<<"unit c : "<<unit_clause<<endl;
			clause->Print();
		}
		if ( sat ( clause -> WATCHED[0] ) != 1 ) {
			if ( sat ( clause -> WATCHED[0] ) == 2 ) {
				watchedReduceTheory(clause -> WATCHED[0]);
				UNITS++;
			}
			else if ( sat ( clause -> WATCHED[1] ) == 2 ) {
				watchedReduceTheory(clause -> WATCHED[1]);
				UNITS++;
			}
		}
	}

	if ( CONFLICT ) UNITLIST.clear();

}

Literal Formula::lazyWatchedChooseLiteral () {

	// Choose any unassigned first watched literal


	for ( unsigned int  i = 0; i < CLAUSELIST.size(); i++ ) {

		Literal watched1 = CLAUSELIST[i] -> WATCHED[0];

		if ( sat ( watched1 ) == 2) return  watched1;
	}
	return Literal();
}

void Formula::SwapPointer ( VARRECORD* current ) {

	int c_num = current->c_num;
    Clause* clause = CLAUSELIST[c_num];

	Literal watched1 = clause -> WATCHED[0];
	Literal watched2 = clause -> WATCHED[1];

	bool unitClause = true;

	if ( sat ( watched2 )  == 2 ) {

		// if watched2 unassigned, watched1 falsified

		for ( int i = 0; i < clause -> NumAtom; i++ ) {

			Literal literal = clause -> ATOM_LIST[i];

			if (sat(literal)==1) {

				VARLIST[watched1.VAR]->removeRecord(current);
				VARLIST[literal.VAR]->addRecord(c_num);

				clause->WATCHED[0] = literal;
				unitClause=false;
				break;
			}

			if (  sat(literal) == 2 &&   (literal.VAR != watched2.VAR)  ) {


				VARLIST[watched1.VAR]->removeRecord(current);
				VARLIST[literal.VAR]->addRecord(c_num);

				clause -> WATCHED[0] = literal;

				unitClause = false;
				break;
			}
		}
	} else {
		if ( LOG ) cout << "Conflict" << endl;
		CONFLICT = true;
		CONFLICTINGCLAUSE = c_num;
		unitClause = false;
	}
	if (unitClause) {
		UNITLIST.push_front(c_num);
	}
}

void Formula::watchedReduceTheory ( Literal lit ) {

	bitset<MAX_DOMAIN> change = VARLIST[lit.VAR]->CURRENT_DOMAIN & ~lit.VALS;

	if (LOG) {
		cout << lit.VAR << "=" << lit.VALS << " at level " << LEVEL << endl;
		cout <<"Effect: " << change <<endl;
		cout <<"New domain: " << (VARLIST[lit.VAR]->CURRENT_DOMAIN & lit.VALS) <<endl;
	}


	if (change.any()) {
		DECSTACK.push_back(lit);

		VARLIST[lit.VAR]->graphNodes.push_back(IGNode(DECSTACK.size() - 1, LEVEL, UNITCLAUSE, change));

		VARLIST[lit.VAR]->CURRENT_DOMAIN = VARLIST[lit.VAR]->CURRENT_DOMAIN & lit.VALS;

		watchedUpdateVar(lit.VAR);
	}
}
//=================== Watched literals NON-CHRONOLOGICAL BACKTRACK ============================//

int Formula::WatchedLiterals ( int restarts ) {


	int restartCount = restarts;

	if ( LOG ) cout << "Solving with the watched literal algorithm..." << endl;

	while ( true ) {
		//Check if time out
		TIME_E = GetTime();
		if ( ( TIME_E - TIME_S ) > TIMELIMIT )
			return 1;

		if ( CONFLICT ) {
			if ( LOG ) {
				cout << "There is a conflict at level: " << LEVEL << endl;
				cout<<"Conflicting clause: "<<  endl;
				CLAUSELIST[CONFLICTINGCLAUSE] -> Print ();
				CLAUSELIST[CONFLICTINGCLAUSE]->WATCHED[0] . Print ();
				CLAUSELIST[CONFLICTINGCLAUSE]->WATCHED[1] . Print ();
			}


			if ( LEVEL == 0 ) { cout << "UNSAT" << endl; return 2; }

			Clause* learned = analyzeConflict ( CLAUSELIST[CONFLICTINGCLAUSE], false );
			if (learned->NumAtom > 1)
				LEVEL = VARLIST[learned->WATCHED[1].VAR]->getIGNode(learned->WATCHED[1].VALS).level;
			else
				LEVEL=0;

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

			//After backtracking the learned clause is the only unit clause
			UNITLIST.clear();
			UNITLIST.push_back(CLAUSELIST.size()-1);
		}

		// If there is a unit clause, propagate
		if ( ! UNITLIST.empty() )
			WatchedUnitPropagation();

	 if ( ! CONFLICT ) {
			Literal atom;
			if ( VSIDS ) atom = chooseLiteralVSIDS();
			else atom = lazyWatchedChooseLiteral ();
			if ( atom.VAR != -1 ) {
				DECISIONS++;
				LEVEL++;
				// set REASON for subsequent falsified atoms
				UNITCLAUSE = -1;
				if ( LOG ) cout << "Decision: " << atom . VAR << "=" << atom . VALS << endl;
				watchedReduceTheory ( atom );
			}  // the theory is satisfied if all variables are assigned and there is no conflict
			else return 0;

		}
	}
}


//End Formula
