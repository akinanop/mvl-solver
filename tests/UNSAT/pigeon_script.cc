#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>

using namespace std;

int main(){

  for(int pgn=11; pgn < 21; pgn++){

    int hole = pgn-1;
    ofstream file;
    string name = static_cast<ostringstream*>( &(ostringstream() << pgn) )->str();
    file.open(name);
    int clauses = ( pow(pgn-1,2) * pgn)/2;
    file<<"c This is a pigeonhole problem with "<<pgn<<" pigeons and "<<hole<<" holes"<< endl;
    file<<"cnf "<<pgn<<" "<<clauses<<endl;

     for(int i=1; i < pgn+1; i++){
      file<< "d "<<i<< " "<<hole<<endl;
    }

    for(int i=0; i < hole; i++){

      for(int j=1; j < pgn +1; j++){
          for(int k=1; k < pgn +1 && k != j; k++){
        file<<j<<"!="<<i<<" "<<k<<"!="<<i<<" 0"<<endl;
      }
      }

    }
    file.close();

  }
  return 0;
}
