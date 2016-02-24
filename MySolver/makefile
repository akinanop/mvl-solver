# Linking object files to create executable
Solver: Main.o Global.o GenBenFunc.o Boolean2Finite.o LinearEncode.o QuadraticEncode.o Clause.o Variable.o Formula.o SolveFinite.o
	g++ -o Solver Main.o Global.o GenBenFunc.o Boolean2Finite.o LinearEncode.o QuadraticEncode.o Clause.o Variable.o Formula.o SolveFinite.o

# Compile source code
Main.o: ~/Desktop/mvl-solver/src/Main.cc ~/Desktop/mvl-solver/src/Global.h ~/Desktop/mvl-solver/src/Formula.h
	g++ -g -c ~/Desktop/mvl-solver/src/Main.cc

Global.o: ~/Desktop/mvl-solver/src/Global.cc ~/Desktop/mvl-solver/src/Global.h
	g++ -g -c ~/Desktop/mvl-solver/src/Global.cc

GenBenFunc.o: ~/Desktop/mvl-solver/src/GenBenFunc.cc ~/Desktop/mvl-solver/src/Global.h ~/Desktop/mvl-solver/src/Literal.h
	g++ -g -c ~/Desktop/mvl-solver/src/GenBenFunc.cc

Boolean2Finite.o: ~/Desktop/mvl-solver/src/Boolean2Finite.cc ~/Desktop/mvl-solver/src/Global.h
	g++ -g -c ~/Desktop/mvl-solver/src/Boolean2Finite.cc

LinearEncode.o : ~/Desktop/mvl-solver/src/LinearEncode.cc ~/Desktop/mvl-solver/src/Global.h
	g++ -g -c ~/Desktop/mvl-solver/src/LinearEncode.cc

QuadraticEncode.o : ~/Desktop/mvl-solver/src/QuadraticEncode.cc ~/Desktop/mvl-solver/src/Global.h
	g++ -g -c ~/Desktop/mvl-solver/src/QuadraticEncode.cc

Clause.o: ~/Desktop/mvl-solver/src/Clause.cc ~/Desktop/mvl-solver/src/Clause.h
	g++ -g -c ~/Desktop/mvl-solver/src/Clause.cc

Variable.o: ~/Desktop/mvl-solver/src/Variable.cc ~/Desktop/mvl-solver/src/Variable.h
	g++ -g -c ~/Desktop/mvl-solver/src/Variable.cc

Formula.o:  ~/Desktop/mvl-solver/src/Formula.cc ~/Desktop/mvl-solver/src/Formula.h
	g++ -g -c ~/Desktop/mvl-solver/src/Formula.cc

SolveFinite.o: ~/Desktop/mvl-solver/src/SolveFinite.cc ~/Desktop/mvl-solver/src/Formula.h
	g++ -g -c ~/Desktop/mvl-solver/src/SolveFinite.cc

# Cleaning object files, exe, and temp files
clean:
	rm -f Solver
	rm -f *~* *.o *.a
	rm -f core

# End of Makefile
