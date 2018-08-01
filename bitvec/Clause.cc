//**************************************************************
//
// Finite Domain Solver
//
// File : Clause.cc
//**************************************************************

//Including Libraries and Header Files
#include "Clause.h"
#include "Literal.h"
using namespace std;
//**************************************************************
//Start of Code

//Zero Argument Constructor
Clause::Clause()
{
  ATOM_LIST.reserve(5);
  NumAtom = 0;

}

//One Argument Constructor
Clause::Clause(int size)
{
  ATOM_LIST.reserve(size);
  NumAtom = 0;

}

//Destructor
Clause::~Clause()
{
  ATOM_LIST.clear();
}

//AddAtom
void Clause::addAtom(Literal atom)
{
	for (int i=0;i<ATOM_LIST.size();i++) {
		if (ATOM_LIST[i].VAR == atom.VAR) {
			ATOM_LIST[i].VALS |= atom.VALS;
			return;
		}
	}

  ATOM_LIST.push_back(atom);
  NumAtom++;
}

//Print
void Clause::Print()
{
  for(int i=0; i<NumAtom; i++)
    {
      ATOM_LIST[i].Print();
      // cout<<" ";
    }
  cout<<endl;
}

//Accessor functions
//getAtom
inline Literal Clause::getAtom(int index)
{
  return ATOM_LIST[index];
}

//getNumAtom
inline int Clause::getNumAtom()
{
  return NumAtom;
}
//End of Code
//**************************************************************
