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
  NumUnAss = 0;
  LEVEL = -1;

}

//One Argument Constructor
Clause::Clause(int size)
{
  ATOM_LIST.reserve(size);
  NumUnAss = 0;
  LEVEL = -1;

}

//Destructor
Clause::~Clause()
{
  ATOM_LIST.clear();
}

//AddAtom
void Clause::addAtom(Literal atom)
{
  ATOM_LIST.push_back(atom);
  NumUnAss++;
}

//Print
void Clause::Print()
{
  for(int i=0; i<ATOM_LIST.size(); i++)
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
