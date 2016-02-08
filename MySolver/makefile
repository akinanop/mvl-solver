# Linking object files to create executable
Solver: Main.o Global.o GenBenFunc.o Boolean2Finite.o LinearEncode.o QuadraticEncode.o Clause.o Variable.o Formula.o SolveFinite.o
	g++ -o Solver Main.o Global.o GenBenFunc.o Boolean2Finite.o LinearEncode.o QuadraticEncode.o Clause.o Variable.o Formula.o SolveFinite.o

# Compile source code
Main.o: ~/netshare/mvl-solver/Main.cc ~/netshare/mvl-solver/Global.h ~/netshare/mvl-solver/Formula.h
	g++ -g -c ~/netshare/mvl-solver/Main.cc

Global.o: ~/netshare/mvl-solver/Global.cc ~/netshare/mvl-solver/Global.h
	g++ -g -c ~/netshare/mvl-solver/Global.cc

GenBenFunc.o: ~/netshare/mvl-solver/GenBenFunc.cc ~/netshare/mvl-solver/Global.h ~/netshare/mvl-solver/Literal.h
	g++ -g -c ~/netshare/mvl-solver/src/GenBenFunc.cc

Boolean2Finite.o: ~/netshare/mvl-solver/src/Boolean2Finite.cc ~/netshare/mvl-solver/Global.h
	g++ -g -c ~/netshare/mvl-solver/src/Boolean2Finite.cc

LinearEncode.o : ~/netshare/mvl-solver/src/LinearEncode.cc ~/netshare/mvl-solver/Global.h
	g++ -g -c ~/netshare/mvl-solver/src/LinearEncode.cc

QuadraticEncode.o : ~/netshare/mvl-solver/src/QuadraticEncode.cc ~/netshare/mvl-solver/Global.h
	g++ -g -c ~/netshare/mvl-solver/src/QuadraticEncode.cc

Clause.o: ~/netshare/mvl-solver/Clause.cc ~/netshare/mvl-solver/Clause.h
	g++ -g -c ~/netshare/mvl-solver/Clause.cc

Variable.o: ~/netshare/mvl-solver/Variable.cc ~/netshare/mvl-solver/src/Variable.h
	g++ -g -c ~/netshare/mvl-solver/src/Variable.cc

Formula.o:  ~/netshare/mvl-solver/Formula.cc ~/netshare/mvl-solver/src/Formula.h
	g++ -g -c ~/netshare/mvl-solver/src/Formula.cc

SolveFinite.o: ~/netshare/mvl-solver/src/SolveFinite.cc ~/netshare/mvl-solver/src/Formula.h
	g++ -g -c ~/netshare/mvl-solver/src/SolveFinite.cc

# Cleaning object files, exe, and temp files
clean:
	rm -f Solver
	rm -f *~* *.o *.a
	rm -f core

# End of Makefile
