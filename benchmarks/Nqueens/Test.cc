#include <stdio.h>
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
// include "Nqueens.cc"

using namespace std;

void run_solver(int var, int clauses, string file)
{ char buffer [100];
  FILE * outp;
  outp = fopen("out4_60R_wl", "a");

  string total =  "~/Desktop/mvl-solver/./Solver -solvenc -wl -file "+file;
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

  for(int queens=4; queens < 61; queens++){

    string name = to_string(queens)+"queens";

    int clauses = 0;
    int var = queens;
    int domain = queens;
    ofstream file;
    file.open(name);

     for(int i=1; i < var + 1; i++){
      file<< "d "<<i<< " "<<domain<<endl;
    }

// alldifferent
    for(int i=0; i < domain; i++){

      for(int j=1; j < var +1; j++){
          for(int k=1; k < var +1 && k != j; k++){
        file<<j<<"!="<<i<<" "<<k<<"!="<<i<<" 0"<<endl;
        clauses++;
      }
    }

    }
file<<"c diagonals"<<endl;

for(int i=1; i < var+1; i++)
{
  for(int k=1; k < var+1 && k!=i; k++)
    { for(int d=0; d < domain ; d++){
        for(int l=0; l < domain ; l++)
      { if (i-d == k-l || i+d == k+l) {
        file<<i<<"!="<<d<<" "<<k<<"!="<<l<<" 0"<<endl;
        clauses++;}

    }
  }
}
}

file<<"p cnf "<<var<<" "<<clauses<<endl;
file.close();

run_solver(var,clauses,"~/Desktop/mvl-solver/benchmarks/Nqueens/"+name);

// cout<<clauses<<endl;
}

  printf("Done!\n");
  // cout<<'\a';
  return 0;
}
