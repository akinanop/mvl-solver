# Linking object files to create executable
Solver: Main.o Global.o GenBenFunc.o Boolean2Finite.o LinearEncode.o QuadraticEncode.o Clause.o Variable.o Formula.o SolveFinite.o
	g++ -o Solver Main.o Global.o GenBenFunc.o Boolean2Finite.o LinearEncode.o QuadraticEncode.o Clause.o Variable.o Formula.o SolveFinite.o

# Compile source code
Main.o: ~/netshare/mvl-solver/Main.cc ~/netshare/mvl-solver/Global.h ~/netshare/mvl-solver/Formula.h
	g++ -g -c ~/netshare/mvl-solver/Main.cc

Global.o: ~/netshare/mvl-solver/Global.cc ~/netshare/mvl-solver/Global.h
	g++ -g -c ~/netshare/mvl-solver/Global.cc

GenBenFunc.o: ~/netshare/mvl-solver/GenBenFunc.cc ~/netshare/mvl-solver/Global.h ~/netshare/mvl-solver/Literal.h
	g++ -g -c ~/netshare/mvl-solver/GenBenFunc.cc

Boolean2Finite.o: ~/netshare/mvl-solver/Boolean2Finite.cc ~/netshare/mvl-solver/Global.h
	g++ -g -c ~/netshare/mvl-solver/Boolean2Finite.cc

LinearEncode.o : ~/netshare/mvl-solver/LinearEncode.cc ~/netshare/mvl-solver/Global.h
	g++ -g -c ~/netshare/mvl-solver/LinearEncode.cc

QuadraticEncode.o : ~/netshare/mvl-solver/QuadraticEncode.cc ~/netshare/mvl-solver/Global.h
	g++ -g -c ~/netshare/mvl-solver/QuadraticEncode.cc

Clause.o: ~/netshare/mvl-solver/Clause.cc ~/netshare/mvl-solver/Clause.h
	g++ -g -c ~/netshare/mvl-solver/Clause.cc

Variable.o: ~/netshare/mvl-solver/Variable.cc ~/netshare/mvl-solver/Variable.h
	g++ -g -c ~/netshare/mvl-solver/Variable.cc

Formula.o:  ~/netshare/mvl-solver/Formula.cc ~/netshare/mvl-solver/Formula.h
	g++ -g -c ~/netshare/mvl-solver/Formula.cc

SolveFinite.o: ~/netshare/mvl-solver/SolveFinite.cc ~/netshare/mvl-solver/Formula.h
	g++ -g -c ~/netshare/mvl-solver/SolveFinite.cc

# Cleaning object files, exe, and temp files
clean:
	rm -f Solver
	rm -f *~* *.o *.a
	rm -f core

# End of Makefile
