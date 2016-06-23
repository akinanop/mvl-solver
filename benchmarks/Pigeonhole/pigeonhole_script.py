import sys

for pgn in range(2,101): # generating problems for n = 2...100
    hole = pgn - 1
    clauses = (pow(pgn - 1, 2) * pgn)/2 ## of clauses in the problem
    print('Creating new text file')
    name = "pgn" + str(pgn) + '.txt'  # Name of text file coerced with +.txt
    try:
        file = open(name, 'w')  # Trying to create a new file
        file.write("c This is a pigeonhole problem with " + str(pgn) + " pigeons and " + str(hole) + " holes\n" + "p cnf " + str(pgn) + " " + str(int(clauses)))
    except:
        print("Something went wrong! Can't tell what?")
        sys.exit(0)  # quit Python

    """
    for each pair of variables we state that they should get different values
    output line example: 1 != 0 2 != 0 - if pigeon 1 is the hole 0, then 2 is not in 0
    """
    for i in range(0, hole):
         for j in range(1, pgn + 1):
            for d in range(j, (pgn + 1)):
                if(d != j):
                    file.write("\n")
                    file.write(str(j) + "!=" + str(i)+ " " + str(d) + "!=" + str(i)+" 0")

    file.close() # closing the file
