#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>

using namespace std;

int main(){

  for(int pgn=5; pgn < 10; pgn++){

    int hole = pgn-1;
    ofstream file;
    string name = "pgn"+to_string(pgn);
    // string name = static_cast<ostringstream*>( &(ostringstream() << pgn) )->str();
    file.open(name);
    int clauses = ( pow(pgn-1,2) * pgn)/2;
    file<<"c This is a GT problem with n = "<<n<<endl;
    file<<"p cnf "<<pgn<<" "<<clauses<<endl;

     for(int i=1; i < pgn+1; i++){
      file<< "d "<<i<< " "<<2<<endl;
    }
   // anti-symmetry:
//  int var = 0;
   for(int i=1; i<n+1; i++)
   {
     for(int j=1; j<n+1 && i<j; j++)
     {
       file<<i<<j<<"=0"<<" "<<j<<i<<"=0"<<" 0"
     }

   }

    }
    file.close();
  //  pgn = pgn + 10;
  }
  return 0;
}
