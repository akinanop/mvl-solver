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
  SAT = false;
  NumAtom = 0;
  NumUnAss = 0;
  LEVEL = -1;
  WATCHED.reserve(2);
  watched1 = 0;
  watched2 = -1;
}

//One Argument Constructor
Clause::Clause(int size)
{
  ATOM_LIST.reserve(size);
  SAT = false;
  NumAtom = 0;
  NumUnAss = 0;
  LEVEL = -1;
  WATCHED.reserve(2);
  watched1 = 0;
  watched2 = -1;

}

//Destructor
Clause::~Clause()
{
  ATOM_LIST.clear();
}

//AddAtom
void Clause::addAtom(Literal * atom)
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

bool Clause::LitisEqual(Literal * literal1, Literal * literal2){
  if(literal1->VAR == literal2->VAR && literal1->VAL == literal2->VAL && literal1->EQUAL == literal2->EQUAL) return true;
   else return false;
}

bool Clause::ClauseisEqual(Clause * clause1,Clause * clause2)
{
  if(clause1->NumAtom != clause2->NumAtom) return false;

  for(int i=0; i<clause1->NumAtom; i++)
  {
    if(!LitisEqual(clause1->ATOM_LIST[i], clause2->ATOM_LIST[i])) return false;
  }
  return true;
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
