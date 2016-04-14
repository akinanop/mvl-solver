#include <stdio.h>


void run_solver(int var, int clauses, string file)
{   FILE* test = popen("./Solver -solvenc -var "+to_string(var)+" -clause "+to_string(clauses)+" -file "+file, "r");
pclose(test);

}

int main()
{
  run_solver(4,18,"UNSAT/counterexample.txt");
  printf("Subprocess finished.\n");

  return 0;
}
