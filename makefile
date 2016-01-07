# Linking object files to create executable
Solver: Main.o Global.o GenBenFunc.o Boolean2Finite.o LinearEncode.o QuadraticEncode.o Clause.o Variable.o Formula.o SolveFinite.o
	g++ -o Solver Main.o Global.o GenBenFunc.o Boolean2Finite.o LinearEncode.o QuadraticEncode.o Clause.o Variable.o Formula.o SolveFinite.o

# Compile source code
Main.o: Main.cc Global.h Formula.h
	g++ -g -c Main.cc

Global.o: Global.cc Global.h
	g++ -g -c Global.cc

GenBenFunc.o: GenBenFunc.cc Global.h Literal.h
	g++ -g -c GenBenFunc.cc

Boolean2Finite.o: Boolean2Finite.cc Global.h
	g++ -g -c Boolean2Finite.cc

LinearEncode.o : LinearEncode.cc Global.h
	g++ -g -c LinearEncode.cc

QuadraticEncode.o : QuadraticEncode.cc Global.h
	g++ -g -c QuadraticEncode.cc

Clause.o: Clause.cc Clause.h
	g++ -g -c Clause.cc

Variable.o: Variable.cc Variable.h
	g++ -g -c Variable.cc

Formula.o:  Formula.cc Formula.h
	g++ -g -c Formula.cc

SolveFinite.o: SolveFinite.cc Formula.h
	g++ -g -c SolveFinite.cc

# Cleaning object files, exe, and temp files
clean:
	rm -f Solver
	rm -f *~* *.o *.a
	rm -f core

# End of Makefile
