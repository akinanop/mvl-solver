//**************************************************************
//
// Finite Domain Solver - Hemal Lal
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
  SAT = false;
  LEVEL = -1;
  ATOMLEVEL = NULL;
  ATOMASSIGN = NULL;
  ATOMCNTPOS = NULL;
  ATOMCNTNEG = NULL;
  ATOMRECPOS = NULL;
  ATOMRECNEG = NULL;
  FLAG = NULL;
  CLAUSEID = NULL;
}

//Two argument constructor
Variable::Variable(int var, int domain)
{
  VAR = var;
  VAL = -1;
  DOMAINSIZE = domain;
  SAT = false;
  LEVEL = -1;
  ATOMLEVEL = new int[DOMAINSIZE];
  ATOMASSIGN = new int[DOMAINSIZE];
  ATOMCNTPOS = new int[DOMAINSIZE];
  ATOMCNTNEG = new int[DOMAINSIZE];
  FLAG = new bool[DOMAINSIZE];
  CLAUSEID = new int[DOMAINSIZE];
  ATOMRECPOS = new VARRECORD *[DOMAINSIZE];
  ATOMRECNEG = new VARRECORD *[DOMAINSIZE];
  for(int i=0; i<DOMAINSIZE; i++)
    {
      ATOMLEVEL[i] = -1;
      ATOMASSIGN[i] = 0;
      ATOMCNTPOS[i] = 0;
      ATOMCNTNEG[i] = 0;
      FLAG[i] = false;
      CLAUSEID[i] = -1;
      ATOMRECPOS[i] = NULL;
      ATOMRECNEG[i] = NULL;
    }
}

//Destructor
Variable::~Variable()
{
  delete [] ATOMLEVEL;
  delete [] ATOMASSIGN;
  delete [] ATOMCNTPOS;
  delete [] ATOMCNTNEG;
  delete [] FLAG;
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
void Variable::AddRecord(int c_id, int d_id, bool flag)
{
  //local variable
  VARRECORD * temp = new VARRECORD;
  temp->c_num = c_id;

  //if memory allocated
  if(temp)
    {
      //if flag = true then its "="; else "!"
      if(flag)
	{
	  temp->next = ATOMRECPOS[d_id];
	  ATOMRECPOS[d_id] = temp;
	}
      else
	{
	  temp->next = ATOMRECNEG[d_id];
	  ATOMRECNEG[d_id] = temp;
	}
    }
}

//Print
void Variable::Print()
{
 //print variable name
  cout<<"Variable = "<<VAR<<endl;
  for(int i=0; i<DOMAINSIZE; i++)
    {
      cout<<"Domain = "<<i<<endl;
      //print POS
      cout<<"(POS Records = ";
      VARRECORD * curr = ATOMRECPOS[i];
      while(curr)
	{
	  cout<<curr->c_num<<" ";
	  curr = curr->next;
	}
      cout<<")"<<endl;
      if(curr) delete curr;

      //print NEG
      cout<<"(NEG Records = ";
      VARRECORD * curr1 = ATOMRECPOS[i];
      curr1 = ATOMRECNEG[i];
      while(curr1)
	{
	  cout<<curr1->c_num<<" ";
	  curr1 = curr1->next;
	}
      cout<<")"<<endl<<endl;
      if (curr1) delete curr1;
    }
}
//End of Code
//**************************************************************
