# Linking object files to create executable
Solver: Main.o Global.o GenBenFunc.o Boolean2Finite.o LinearEncode.o QuadraticEncode.o Clause.o Variable.o Formula_old.o SolveFinite.o
	g++ -o Solver Main.o Global.o GenBenFunc.o Boolean2Finite.o LinearEncode.o QuadraticEncode.o Clause.o Variable.o Formula_old.o SolveFinite.o

# Compile source code
Main.o: ~/netshare/mvl-solver/src/Main.cc ~/netshare/mvl-solver/src/Global.h ~/netshare/mvl-solver/src/Formula_old.h
	g++ -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused -g -c ~/netshare/mvl-solver/src/Main.cc

Global.o: ~/netshare/mvl-solver/src/Global.cc ~/netshare/mvl-solver/src/Global.h
	g++ -g -c ~/netshare/mvl-solver/src/Global.cc

GenBenFunc.o: ~/netshare/mvl-solver/src/GenBenFunc.cc ~/netshare/mvl-solver/src/Global.h ~/netshare/mvl-solver/src/Literal.h
	g++ -g -c ~/netshare/mvl-solver/src/GenBenFunc.cc

Boolean2Finite.o: ~/netshare/mvl-solver/src/Boolean2Finite.cc ~/netshare/mvl-solver/src/Global.h
	g++ -g -c ~/netshare/mvl-solver/src/Boolean2Finite.cc

LinearEncode.o : ~/netshare/mvl-solver/src/LinearEncode.cc ~/netshare/mvl-solver/src/Global.h
	g++ -g -c ~/netshare/mvl-solver/src/LinearEncode.cc

QuadraticEncode.o : ~/netshare/mvl-solver/src/QuadraticEncode.cc ~/netshare/mvl-solver/src/Global.h
	g++ -g -c ~/netshare/mvl-solver/src/QuadraticEncode.cc

Clause.o: ~/netshare/mvl-solver/src/Clause.cc ~/netshare/mvl-solver/src/Clause.h
	g++ -g -c ~/netshare/mvl-solver/src/Clause.cc

Variable.o: ~/netshare/mvl-solver/src/Variable.cc ~/netshare/mvl-solver/src/Variable.h
	g++ -g -c ~/netshare/mvl-solver/src/Variable.cc

Formula_old.o:  ~/netshare/mvl-solver/src/Formula_old.cc ~/netshare/mvl-solver/src/Formula_old.h
	g++ -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Wno-unused -g -c ~/netshare/mvl-solver/src/Formula_old.cc

SolveFinite.o: ~/netshare/mvl-solver/src/SolveFinite.cc ~/netshare/mvl-solver/src/Formula_old.h
	g++ -g -c ~/netshare/mvl-solver/src/SolveFinite.cc

# Cleaning object files, exe, and temp files
clean:
	rm -f Solver
	rm -f *~* *.o *.a
	rm -f core

# End of Makefile
