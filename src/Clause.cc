//**************************************************************
//
// Finite Domain Solver - Hemal Lal
//
// File : Clause.cc
//**************************************************************

//Including Libraries and Header Files
#include "Clause.h"

using namespace std;
//**************************************************************
//Start of Code

//Zero Argument Constructor
Clause::Clause()
{
  ATOM_LIST.reserve(5);
  SAT = false;
  NumAtom = 0;
  NumUnAss = 0;
  LEVEL = -1;
}

//One Argument Constructor
Clause::Clause(int size)
{
  ATOM_LIST.reserve(size);
  SAT = false;
  NumAtom = 0;
  NumUnAss = 0;
  LEVEL = -1;
}

//Destructor
Clause::~Clause()
{
  ATOM_LIST.clear();
}

//AddAtom
void Clause::AddAtom(Literal * atom)
{
  ATOM_LIST.push_back(atom);
  NumAtom++;
  NumUnAss++;
}


//Print
void Clause::Print()
{
  for(int i=0; i<NumAtom; i++)
    {
      ATOM_LIST[i]->Print();
      // cout<<" ";
    }
  cout<<endl;
}

//Accessor functions
//getAtom
inline Literal * Clause::getAtom(int index)
{
  return ATOM_LIST[index];
}

//getSAT
inline bool Clause::getSAT()
{
  return SAT;
}

//setSAT
inline void Clause::setSAT(bool flag)
{
  SAT = flag;
}

//getNumAtom
inline int Clause::getNumAtom()
{
  return NumAtom;
}

//getNumUnAss
inline int Clause::getNumUnAss()
{
  return NumUnAss;
}

//incNumUnAss
inline void Clause::incNumUnAss()
{
  NumUnAss++;
}

//decNumUnAss
inline void Clause::decNumUnAss()
{
  NumUnAss--;
}

//getLevel
inline int Clause::getLevel()
{
  return LEVEL;
}

//setLevel
inline void Clause::setLevel(int level)
{
  LEVEL = level;
}

//isUnit
inline bool Clause::isUnit()
{
  return (NumUnAss == 1 ? true : false);
}

//isEmpty
inline bool Clause::isEmpty()
{
  return (NumUnAss == 0 ? true : false);
}
//End of Code
//**************************************************************
