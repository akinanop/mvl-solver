// This is a script tp generate an n-queens problem for any n
#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include <cmath>

using namespace std;

int main( ) {
  for( int queens = 60 ; queens < 61 ; queens++ ) {
    // we want a seperate file for each n = 4...100 with title "nqueens"
    string name = to_string(queens) + "queens" ;

    int clauses = 0 ; // we want to count clauses to add this info later - could have just found a fomula as in the pigeonhole case
    int var = queens ; // each variable corresponds to a queen on a column
    int domain = queens ; // each domain value corresponds to a quuen on a row

    //open a file
    ofstream file ;
    file.open(name) ;

    // write domain declaration for each variable

     for(int i = 1; i < var + 1; i++ ) {
      file << "d " << i << " " << domain << endl ;
    }

// no two queens should be on the same row
    for ( int i = 0; i < domain ; i ++ ) {
      for ( int j = 1; j < var + 1 ; j++ ) {
        for ( int k = 1; k < var + 1 && k != j; k++ ) {
          file << j << "!=" << i << " " << k << "!=" << i <<" 0" << endl ;
          clauses++;
        }
      }
    }
    // commment
  file << "c diagonals" << endl ;

// no two queens should be on the same diagonal:
  for ( int i = 1 ; i < var + 1 ; i++ ) {
    for ( int k = 1 ; k < var + 1 && k != i ; k++ ) {
      for ( int d = 0 ; d < domain ; d++ ) {
          for( int l = 0 ; l < domain ; l++ ) {
            if ( i - d == k - l || i + d == k + l ) {
              file << i<< "!=" << d << " " << k << "!=" << l << " 0" << endl ;
              clauses++ ;
            }
          }
        }
      }
    }

// adding the problem info - better to do at the beginning
  file << "p cnf " << var << " " << clauses << endl ;

// closing (saving) the file
    file.close( ) ;
  }
    return 0 ;

}
