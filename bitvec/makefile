# -O3 enables optimization
FLAGS = -O3

all: Solver

# For debugging optimization is turned off and debug information is enabled
debug: FLAGS = -g
debug: Solver

# Linking object files to create executable
Solver: Main.o Global.o Clause.o Variable.o Formula.o SolveFinite.o
	g++ $(FLAGS) -o Solver Main.o Global.o Clause.o Variable.o Formula.o SolveFinite.o

# Compile source code
Main.o: Main.cc Global.h Formula.h
	g++ $(FLAGS) -c Main.cc

Global.o: Global.cc Global.h
	g++ $(FLAGS) -c Global.cc

Clause.o: Clause.cc Clause.h Literal.h
	g++ $(FLAGS) -c Clause.cc

Variable.o: Variable.cc Variable.h Literal.h
	g++ $(FLAGS) -c Variable.cc

Formula.o:  Formula.cc Formula.h Literal.h
	g++ $(FLAGS) -c Formula.cc

SolveFinite.o: SolveFinite.cc Formula.h
	g++ $(FLAGS) -c  SolveFinite.cc

# Cleaning object files, exe, and temp files
clean:
	rm -f *~* *.o *.a
	rm -f *.gcno
	rm -f *.gcda *.gcdo
# End of Makefile
