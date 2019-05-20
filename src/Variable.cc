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
	ATOMRECPOS = new VARRECORD *[DOMAINSIZE];
	ATOMRECNEG = new VARRECORD *[DOMAINSIZE];
	for(int i=0; i<DOMAINSIZE; i++)
	{ // CHANGED DEFAULT ATOMLEVEL TO -10
		ATOMLEVEL[i] = -10;
		ATOMASSIGN[i] = 0;
		ATOMCNTPOS[i] = 0;
		ATOMCNTNEG[i] = 0;
		VSIDS_SCORE[i] = 0;
		VSIDS_SCORE_NEG[i] = 0;
		CLAUSEID[i] = -10;
		ATOMRECPOS[i] = NULL;
		ATOMRECNEG[i] = NULL;
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
		VARRECORD * temp = ATOMRECPOS[i];
		VARRECORD * temp2;
		while(temp)
		{
			temp2 = temp;
			temp = temp->next;
			delete temp2;
		}
		temp = ATOMRECNEG[i];
		while(temp)
		{
			temp2 = temp;
			temp = temp->next;
			delete temp2;
		}
		delete ATOMRECPOS[i];
		delete ATOMRECNEG[i];
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
			temp->next = ATOMRECPOS[d_id];
			if (ATOMRECPOS[d_id])
				temp->next->prev = temp;
			ATOMRECPOS[d_id] = temp;
		} else {
			temp->next = ATOMRECNEG[d_id];
			if (ATOMRECNEG[d_id])
				temp->next->prev = temp;
			ATOMRECNEG[d_id] = temp;
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
			ATOMRECPOS[d_id] = record->next;
		else
			ATOMRECNEG[d_id] = record->next;
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
