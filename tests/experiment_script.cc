#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include "Global.h"
#include "Formula.h"
#include <time.h>

using namespace std;

int main(){

  for(int pgn=88; pgn < 89; pgn++){

    int vars = pgn;
    int clauses = (pow(pgn-1,2) * pgn)/2;
    string name = "test/pgn"+to_string(pgn);

    CommandLine * cline = new CommandLine;

    cline->NUM_VAR = vars;
    cline->NUM_CLAUSE = clauses;
     cline->CLAUSE_SIZE = 2;
    cline->SAT_UNSAT = true;
    cline->DOMAIN_SIZE = pgn-1;
    cline->DRAND = false;
    cline->BOOL_FINITE = false;

   cline->MODEL_FILE = "die";
    cline->TIME = 3600;
    cline->FILE = name;

    SolveFinite(cline, "nc");
    pgn = pgn + 10;

//  free(cline);
  }

  return 0;

}
