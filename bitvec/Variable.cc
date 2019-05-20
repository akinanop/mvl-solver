//**************************************************************
//
// Finite Domain Solver
//
// File : Variable.cc
//**************************************************************

//Including Libraries and Header Files
#include "Variable.h"

using namespace std;
//**************************************************************
//Start of Code

//Zero argument constructor
Variable::Variable()
{
	VAR = -1;
	DOMAINSIZE = -1;
	VSIDS_SCORE = NULL;
	VARREC = NULL;
}

//Two argument constructor
Variable::Variable(int var, int domain)
{
	VAR = var;

	CURRENT_DOMAIN = bitset<MAX_DOMAIN>();
	CURRENT_DOMAIN.set();
	CURRENT_DOMAIN>>=MAX_DOMAIN-domain;

	DOMAINSIZE = domain;
	VSIDS_SCORE = new double[DOMAINSIZE];
	VARREC = NULL;
	for(int i=0; i<DOMAINSIZE; i++)
	{ // CHANGED DEFAULT ATOMLEVEL TO -10
		VSIDS_SCORE[i] = 0;
	}

	graphNodes.reserve(5);

}

//Destructor
Variable::~Variable()
{
	delete [] VSIDS_SCORE;

	VARRECORD * temp = VARREC;
	VARRECORD * temp2;
	while(temp)
	{
		temp2 = temp;
		temp = temp->next;
		delete temp2;
	}
}

//AddRecord
void Variable::addRecord ( int c_id )
{
	//local variable
	VARRECORD * temp = new VARRECORD;
	temp->c_num = c_id;

	//if memory allocated
	if(temp)
	{
		temp->next = VARREC;
		if (VARREC)
			temp->next->prev = temp;
		temp->prev = NULL;
		VARREC = temp;
	}
}

//RemoveRecord
void Variable::removeRecord ( VARRECORD* record )
{
	if (record->prev) {
		record->prev->next = record->next;
	} else {
		VARREC = record->next;
	}
	if (record->next) {
		record->next->prev = record->prev;
	}
	if (record == VARREC) {
		VARREC = record->next;
	}

	delete record;
}

//Print
void Variable::Print()
{
	//print variable name
	cout<<"Variable = "<<VAR<<endl;
	cout<<"Domain = "<<DOMAINSIZE<<endl;
}

IGNode Variable::getIGNode(bitset<MAX_DOMAIN> values) {
	for (int i = graphNodes.size() - 1; i >= 0; i--) {
		if ((graphNodes[i].values & values).any()) {
			return graphNodes[i];
		}
	}
	return IGNode(-1,-1,-1,values);
}


//End of Code
//**************************************************************
