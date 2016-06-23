import random
import sys

# generate formulas using the constant probability model

def random_clause(number_variables):
    clause = []
    for i in range(1,number_variables+1): 
        flip_coin = random.randint(1,2)
        if flip_coin == 1: 
            flip_more = random.randint(1,2)
            if flip_more == 1:
                clause.append(-i)
            else:
                clause.append(i)
    return clause    

def random_formula(number_variables, number_clauses):
    formula = [ random_clause(number_variables) for _ in range(1, number_clauses + 1) ]

    variables_in_the_formula = set()
    for cl in formula:
        variables_in_the_formula.update(abs(i) for i in cl)
    nvars = len(variables_in_the_formula)

    average = sum(len(clause) for clause in formula) * 1.0 / number_clauses
    return formula, nvars, average

def dimacs_formula(number_variables, number_clauses):
    s = ""
    formula, nvars, average = random_formula(number_variables, number_clauses)
    s += "p cnf %d %d\n" %(number_variables, number_clauses) 
    for k in formula:
        s += " ".join(str(i) for i in k) + ' 0\n'
    return s, nvars, average

#if __name__ == "__main__":
#    name, number_variables1, number_clauses1 = sys.argv
#    number_variables = int(number_variables1)
#    number_clauses = int(number_clauses1)
#
#    print dimacs_formula(number_variables, number_clauses)
