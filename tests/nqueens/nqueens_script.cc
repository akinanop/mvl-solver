#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include <cmath>

using namespace std;

// n-queens as alldifferent

int main(){
  for(int queens=27; queens < 49; queens++){

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
cout<<clauses<<endl;
    file.close();
}
  return 0;
}
