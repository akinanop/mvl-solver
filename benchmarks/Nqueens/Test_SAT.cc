#include <stdio.h>
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
// include "Nqueens.cc"

using namespace std;

void run_solver(int file)
{ char buffer [100];
  FILE * outp;
  outp = fopen("out4_74_minisat", "a");

  string total =  "minisat "+to_string(file);
  FILE* test = popen(total.c_str(), "r");
  while ( ! feof (test) )
       {
         if ( fgets (buffer , 100 , test) == NULL ) break;
         fputs (buffer , outp);

       }
  fclose(outp);
  pclose(test);

}

int main()
{
  // Nqueens

  for ( int i = 4; i < 75 ; i++ ) {
    char buffer [100];
      FILE * outp;
      outp = fopen("ttt", "a");
  string total =  "~/netshare/mvl-solver/benchmarks/Nqueens/./sat "+ to_string(i) + ' ' + to_string(i);
  FILE* test = popen(total.c_str(), "r");

  while ( ! feof (test) )
       {
         if ( fgets (buffer , 100 , test) == NULL ) break;
         fputs (buffer , outp);

       }
  fclose(outp);
  pclose(test);

  run_solver(i);
// cout<<clauses<<endl;
}

  printf("Done!\n");
  // cout<<'\a';
  return 0;
}
