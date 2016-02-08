import random
import sys

def sat_formula(number_variables, number_clauses, clause_length):
    formula = []
    for j in range(number_clauses):
        clause = [random.randint(1, number_variables) for _ in range(clause_length)]	
        for i in range(clause_length):
            flip = random.randint(1,2)
            if flip == 1:
                clause[i] = -clause[i]
        formula.append(clause)
    return formula

def dimacs(number_variables, number_clauses, clause_length = 3):
    s = ""
    formula = sat_formula(number_variables, number_clauses, clause_length)
    s += "p cnf %d %d\n" %(number_variables, number_clauses) 
    for k in formula:
        s += " ".join(str(i) for i in k) + ' 0\n'
    return s

if __name__ == "__main__":
    name, number_variables1, number_clauses1 = sys.argv
    number_variables = int(number_variables1)
    number_clauses = int(number_clauses1)

    print dimacs(number_variables, number_clauses)