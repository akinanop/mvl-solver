# Linking object files to create executable
Solver: Main.o Global.o Clause.o Variable.o Formula.o SolveFinite.o
	g++  -O -o Solver Main.o Global.o Clause.o Variable.o Formula.o SolveFinite.o

# Compile source code
Main.o: Main.cc Global.h Formula.h
	g++ -O  -g -c Main.cc

Global.o: Global.cc Global.h
	g++  -O -g -c Global.cc

Clause.o: Clause.cc Clause.h Literal.h
	g++  -O -g -c Clause.cc

Variable.o: Variable.cc Variable.h Literal.h
	g++  -O -g -c Variable.cc

Formula.o:  Formula.cc Formula.h Literal.h
	g++ -O  -g -c Formula.cc

SolveFinite.o: SolveFinite.cc Formula.h
	g++  -O -g -c  SolveFinite.cc

# Cleaning object files, exe, and temp files
clean:
	rm -f *~* *.o *.a
	rm -f *.gcno
	rm -f *.gcda *.gcdo
# End of Makefile
