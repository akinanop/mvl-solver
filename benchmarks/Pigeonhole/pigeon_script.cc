// This is a script to generate a pigeonhole problems for different number of pigeons
// Pigeonhole problem: it is impossible to fit n+1 pigeons in n holes, such each hole contains exactly one pigeon
// We have n+1 variables, each has domain n, then the clauses descibe the conditions that all variables should get different assignments.
#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>

using namespace std;

int main () {

  for ( int pgn = 3; pgn < 11; pgn++ ) { // generating problems for n = 2...100

    int hole = pgn - 1 ;
    int clauses = ( pow( pgn - 1 , 2 ) * pgn ) / 2 ; // # of clauses in the problem

    // create a separate file for each pigeonhole problem:

    ofstream file ;
    string name = "pgn" + to_string( pgn ) ;
    file.open( name );

    // write to this file:

    // comment describing the problem:
    file << "c This is a pigeonhole problem with " << pgn << " pigeons and " << hole << " holes" << endl ;

    // line with # of variables and clauses:
    file << "p cnf " << pgn << " " << clauses << endl ;

    // domain for each variable:
     for ( int i = 1 ; i < pgn + 1 ; i++ ) {
      file << "d " << i << " " << hole << endl ;
    }

    // for each pair of variables we state that they should get different values
    // output line example: 1!=0 2!=0  - if pigeon 1 is the hole 0, then 2 is not in 0
    for ( int i = 0 ; i < hole; i++ ) {
      for ( int j = 1 ; j < pgn + 1; j++ ) {
          for ( int k = 1 ; k < pgn + 1 && k != j; k++ ) {
        file << j << "!=" << i << " " << k << "!=" << i << " 0" << endl ;
        }
      }
    }

    // closing the file
    file.close( ) ;
  }
  return 0 ;
}
