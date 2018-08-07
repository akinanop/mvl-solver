# -O3 enables optimization
FLAGS = -O3

all: Solver

# For debugging optimization is turned off and debug information is enabled
debug: FLAGS = -g
debug: Solver

# Linking object files to create executable
Solver: Main.o Global.o GenBenFunc.o Boolean2Finite.o LinearEncode.o QuadraticEncode.o Clause.o Variable.o Formula.o SolveFinite.o
	g++ $(FLAGS) -o Solver Main.o Global.o GenBenFunc.o Boolean2Finite.o LinearEncode.o QuadraticEncode.o Clause.o Variable.o Formula.o SolveFinite.o

# Compile source code
Main.o: src/Main.cc src/Global.h src/Formula.h
	g++ $(FLAGS) -c src/Main.cc

Global.o: src/Global.cc src/Global.h
	g++ $(FLAGS) -c src/Global.cc

GenBenFunc.o: src/GenBenFunc.cc src/Global.h src/Literal.h
	g++ $(FLAGS) -c src/GenBenFunc.cc

Boolean2Finite.o: src/Boolean2Finite.cc src/Global.h
	g++ $(FLAGS) -c src/Boolean2Finite.cc

LinearEncode.o : src/LinearEncode.cc src/Global.h
	g++ $(FLAGS) -c src/LinearEncode.cc

QuadraticEncode.o : src/QuadraticEncode.cc src/Global.h
	g++ $(FLAGS) -c src/QuadraticEncode.cc

Clause.o: src/Clause.cc src/Clause.h
	g++ $(FLAGS) -c src/Clause.cc

Variable.o: src/Variable.cc src/Variable.h
	g++ $(FLAGS) -c src/Variable.cc

Formula.o:  src/Formula.cc src/Formula.h
	g++ $(FLAGS) -c src/Formula.cc

SolveFinite.o: src/SolveFinite.cc src/Formula.h
	g++ $(FLAGS) -c  src/SolveFinite.cc

# Cleaning object files, exe, and temp files
clean:
	rm -f *~* *.o *.a
	# rm -f core
	rm -f *.gcno
	rm -f *.gcda *.gcdo
# End of Makefile
