''' Generates a k-CNF sentence with m clauses and n symbols, where the clauses 
    are chosen uniformly, independently, and without replacement from 
    among all clauses with k different literals, which are positive or 
    negative at random'''
import sys
import os
import random

def generate_unique_random_clause(clauses, variables, numLiterals):
    while True:
        # randomly choose k of the variables for each clause
        literals = random.sample(variables, k = numLiterals)

        # randomly negate variables
        numVarToFlip = random.randint(0, numLiterals-1)
        if numVarToFlip != 0:
            # Randomly choose which variables to flip
            varToFlip = random.sample(literals, k = numVarToFlip)
            for var in varToFlip:
                varIndex = literals.index(var)
                literals[varIndex] = var * -1   # flip variable

        # check if a clause with the literals already exists
        if all(sorted(literals) != sorted(i) for i in clauses):
            return literals
        
def k_cnf():
    try:
        argumentList = [int(i) for i in sys.argv[1:]] 
    except:
        print("ERROR: inputs [k], [m], and [n] must be positive integers")
        return
    
    if len(argumentList) != 3:
        print("ERROR: k-cnf requires 3 inputs in the format: python3 k-cnf.py [k] [m] [n]")
        return

    if not all(x > 0 for x in argumentList):
        print("ERROR: inputs [k], [m], and [n] must be positive integers")
        return

    numLiterals = argumentList[0]   # k
    numClauses = argumentList[1]    # m
    numVariables = argumentList[2]  # n

    if numLiterals > numVariables:
        print("ERROR: k cannot be greater than n")
        return

    variables = list(range(1, numVariables+1)) # list of variables numbered 1 to n
    
    # write first line of CNF file
    # https://stackoverflow.com/questions/17984809/how-do-i-create-a-incrementing-filename-in-python
    i = 0
    while os.path.exists("input%s.txt" % i):
        i += 1

    fileName = "input%s.txt" % i 

    try:
        f = open(fileName, "a")
    except:
        print("ERROR: Unable to open/create a file")
        exit
    
    f.write("p cnf %s %s\n" % (numVariables, numClauses))

    clauses = []    # list of list of literals

    for i in range(0, numClauses):
        newClause = generate_unique_random_clause(clauses, variables, numLiterals)
        clauses.append(newClause)
        # write new clause into the file?
        print("Clauses:", clauses)

        for j in range(numLiterals):
            f.write("%s " % newClause[j])
        f.write("0\n")


if __name__ == '__main__':    
    k_cnf()