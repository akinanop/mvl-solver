#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>

using namespace std;

int main(){

  for(int pgn=11; pgn < 1001; pgn++){
    int vars = pgn;
    int clauses = ( pow(pgn-1,2) * pgn)/2;
    ofstream file;
    string name = "pgn"+to_string(pgn);
    file.open(name);
    

    pgn = pgn + 10;
  }

  return 0;

}
