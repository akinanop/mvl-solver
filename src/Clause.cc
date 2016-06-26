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
  W1 = 0;
  W2 = -1;

  ATOM_LIST_MODIF.reserve(5); // ???

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
  W1 = 0;
  W2 = -1;


  ATOM_LIST_MODIF.reserve(size);

}

void addAtom ( int var, char ch, int val ) {
  for ( int i = 0; i < NumAtom; i++ ) {
    if ( ATOM_LIST[i] -> VAR == var ) {
      if ( ch == '=' )  MULTIVAL[val] = 1;
      else {
          for ( int i = 0; i < MULTIVAL.size() && i != vl; i++ ) {
            MULTIVAL[i] = 1;
          }
      }
    } else ATOM_LIST.push_back(new Literal(var,ch,val,VARLIST[var]->DOMAINSIZE));
  }
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


void Clause::addAtomModif(Literal * atom)
{
  for ( int i = 0; i < ATOM_LIST_MODIF.size(); i++ ) {

	  if ( atom -> VAR ==  ATOM_LIST_MODIF[i][0] -> VAR ) {
		  ATOM_LIST_MODIF[i][1]= atom;
	  }

  }
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
