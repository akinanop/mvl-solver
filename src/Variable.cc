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
	VAL = -1;
	DOMAINSIZE = -1;
	ATOMLEVEL = NULL;
	ATOMASSIGN = NULL;
	ATOMCNTPOS = NULL;
	ATOMCNTNEG = NULL;
	VSIDS_SCORE = NULL;
	VSIDS_SCORE_NEG = NULL;
	ATOMWATCHPOS = NULL;
	ATOMWATCHNEG = NULL;
	ATOMRECPOS = NULL;
	ATOMRECNEG = NULL;
	ATOMINDEX = NULL;
	CLAUSEID = NULL;
}

//Two argument constructor
Variable::Variable(int var, int domain)
{
	VAR = var;
	VAL = -1;
	DOMAINSIZE = domain;
	ATOMINDEX = new int[DOMAINSIZE];
	ATOMLEVEL = new int[DOMAINSIZE];
	ATOMASSIGN = new int[DOMAINSIZE];
	ATOMCNTPOS = new int[DOMAINSIZE];
	ATOMCNTNEG = new int[DOMAINSIZE];
	VSIDS_SCORE = new double[DOMAINSIZE];
	VSIDS_SCORE_NEG = new double[DOMAINSIZE];
	CLAUSEID = new int[DOMAINSIZE];
	ATOMWATCHPOS = new VARRECORD *[DOMAINSIZE];
	ATOMWATCHNEG = new VARRECORD *[DOMAINSIZE];
	ATOMRECPOS = new vector<int> [DOMAINSIZE];
	ATOMRECNEG = new vector<int> [DOMAINSIZE];
	for(int i=0; i<DOMAINSIZE; i++)
	{ // CHANGED DEFAULT ATOMLEVEL TO -10
		ATOMLEVEL[i] = -10;
		ATOMASSIGN[i] = 0;
		ATOMCNTPOS[i] = 0;
		ATOMCNTNEG[i] = 0;
		VSIDS_SCORE[i] = 0;
		VSIDS_SCORE_NEG[i] = 0;
		CLAUSEID[i] = -10;
		ATOMWATCHPOS[i] = NULL;
		ATOMWATCHNEG[i] = NULL;
		ATOMINDEX[i] = -1;
	}
}

//Destructor
Variable::~Variable()
{
	delete [] ATOMLEVEL;
	delete [] ATOMASSIGN;
	delete [] ATOMCNTPOS;
	delete [] ATOMCNTNEG;
	delete [] VSIDS_SCORE;
	delete [] VSIDS_SCORE_NEG;
	delete [] CLAUSEID;

	for(int i=0; i<DOMAINSIZE; i++)
	{
		VARRECORD * temp = ATOMWATCHPOS[i];
		VARRECORD * temp2;
		while(temp)
		{
			temp2 = temp;
			temp = temp->next;
			delete temp2;
		}
		temp = ATOMWATCHNEG[i];
		while(temp)
		{
			temp2 = temp;
			temp = temp->next;
			delete temp2;
		}
		delete ATOMWATCHPOS[i];
		delete ATOMWATCHNEG[i];
	}
}

//AddRecord
void Variable::addRecord ( int c_id, int d_id, bool flag )
{
	//local variable
	VARRECORD * temp = new VARRECORD;
	temp->c_num = c_id;

	//if memory allocated
	if(temp)
	{
		temp->prev = NULL;
		if (flag) {
			temp->next = ATOMWATCHPOS[d_id];
			if (ATOMWATCHPOS[d_id])
				temp->next->prev = temp;
			ATOMWATCHPOS[d_id] = temp;
		} else {
			temp->next = ATOMWATCHNEG[d_id];
			if (ATOMWATCHNEG[d_id])
				temp->next->prev = temp;
			ATOMWATCHNEG[d_id] = temp;
		}
	}
}

//RemoveRecord
void Variable::removeRecord ( VARRECORD* record, int d_id, bool flag )
{
	if (record->prev) {
		record->prev->next = record->next;
	} else {
		if (flag)
			ATOMWATCHPOS[d_id] = record->next;
		else
			ATOMWATCHNEG[d_id] = record->next;
	}
	if (record->next) {
		record->next->prev = record->prev;
	}

	delete record;
}

//Print
void Variable::Print()
{
	//print variable name
	cout<<"Variable = "<<VAR<<endl;
}
//End of Code
//**************************************************************
