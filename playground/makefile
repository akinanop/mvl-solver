# Linking object files to create executable
Solver: Main.o Global.o GenBenFunc.o Boolean2Finite.o LinearEncode.o QuadraticEncode.o Clause.o Variable.o Formula_verbose.o SolveFinite.o
	g++ -fprofile-arcs -o Solver Main.o Global.o GenBenFunc.o Boolean2Finite.o LinearEncode.o QuadraticEncode.o Clause.o Variable.o Formula_verbose.o SolveFinite.o

# Compile source code
Main.o: ~/netshare/mvl-solver/src/Main.cc ~/netshare/mvl-solver/src/Global.h ~/netshare/mvl-solver/src/Formula.h
	g++ -fprofile-arcs -ftest-coverage -g -c ~/netshare/mvl-solver/src/Main.cc

Global.o: ~/netshare/mvl-solver/src/Global.cc ~/netshare/mvl-solver/src/Global.h
	g++ -fprofile-arcs -ftest-coverage -g -c ~/netshare/mvl-solver/src/Global.cc

GenBenFunc.o: ~/netshare/mvl-solver/src/GenBenFunc.cc ~/netshare/mvl-solver/src/Global.h ~/netshare/mvl-solver/src/Literal.h
	g++ -fprofile-arcs -ftest-coverage -g -c ~/netshare/mvl-solver/src/GenBenFunc.cc

Boolean2Finite.o: ~/netshare/mvl-solver/src/Boolean2Finite.cc ~/netshare/mvl-solver/src/Global.h
	g++ -fprofile-arcs -ftest-coverage -g -c ~/netshare/mvl-solver/src/Boolean2Finite.cc

LinearEncode.o : ~/netshare/mvl-solver/src/LinearEncode.cc ~/netshare/mvl-solver/src/Global.h
	g++ -fprofile-arcs -ftest-coverage -g -c ~/netshare/mvl-solver/src/LinearEncode.cc

QuadraticEncode.o : ~/netshare/mvl-solver/src/QuadraticEncode.cc ~/netshare/mvl-solver/src/Global.h
	g++ -fprofile-arcs -ftest-coverage -g -c ~/netshare/mvl-solver/src/QuadraticEncode.cc

Clause.o: ~/netshare/mvl-solver/src/Clause.cc ~/netshare/mvl-solver/src/Clause.h
	g++ -fprofile-arcs -ftest-coverage -g -c ~/netshare/mvl-solver/src/Clause.cc

Variable.o: ~/netshare/mvl-solver/src/Variable.cc ~/netshare/mvl-solver/src/Variable.h
	g++ -fprofile-arcs -ftest-coverage -g -c ~/netshare/mvl-solver/src/Variable.cc

Formula_verbose.o:  ~/netshare/mvl-solver/src/Formula_verbose.cc ~/netshare/mvl-solver/src/Formula.h
	g++ -fprofile-arcs -ftest-coverage -g -c ~/netshare/mvl-solver/src/Formula_verbose.cc

SolveFinite.o: ~/netshare/mvl-solver/src/SolveFinite.cc ~/netshare/mvl-solver/src/Formula.h
	g++ -fprofile-arcs -ftest-coverage -g -c  ~/netshare/mvl-solver/src/SolveFinite.cc

# Cleaning object files, exe, and temp files
clean:
	rm -f Solver
	rm -f *~* *.o *.a
	rm -f core

# End of Makefile
