#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include <cmath>

using namespace std;

// n-queens using alldifferent

int main(){
    int var = 4;
    int domain = 4;
    ofstream file;
    file.open("alldiff");
    file<<"p cnf "<<var<<" "<<100<<endl;

     for(int i=1; i < var + 1; i++){
      file<< "d "<<i<< " "<<domain<<endl;
    }

// alldifferent
    for(int i=0; i < domain; i++){

      for(int j=1; j < var +1; j++){
          for(int k=1; k < var +1 && k != j; k++){
        file<<j<<"!="<<i<<" "<<k<<"!="<<i<<" 0"<<endl;
      }
    }

    }
file<<"c diagonals"<<endl;
// diagonals
  for(int i=1; i < var+1; i++)
  { for(int j=0; j < domain; j++)
    { for(int k=1; k < var+1 && k!=i; k++)
      { for(int l=0; l < domain && abs(i-(j+1)) == abs(k-(l+1)); l++)
        {
          file<<i<<"!="<<j<<" "<<k<<"!="<<l<<" 0"<<endl;

      }

    }
   }
  }


    file.close();

  return 0;
}
