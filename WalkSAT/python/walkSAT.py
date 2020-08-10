''' Taken from aima-python repository 
    Edited to isolate the WalkSAT algorihtm for comparing '''

import random
import sys
import time

def probability(p):
    """Return true with probability p."""
    return p > random.uniform(0.0, 1.0)

def pl_true(exp, model={}):
    """Return True if the propositional logic expression is true in the model,
    and False if it is false. If the model does not specify the value for
    every proposition, this may return None to indicate 'not obvious';
    this may happen even when the expression is tautological.
    >>> pl_true(P, {}) is None
    True
    """
    for sym in exp:
        # clause is true (satisfied) if:
        #   sym is positive and the model for syn is true OR
        #   sym is negative and the model for sym is false
        if (int(sym) > 0 and model[abs(int(sym))]) or (int(sym) < 0 and not (model[abs(int(sym))])):
            return True

    return False

def prop_symbols(x):
    """Return the set of all propositional symbols in x."""
    if not isinstance(x, Expr):
        return set()
    elif is_prop_symbol(x.op):
        return {x}
    else:
        return {symbol for arg in x.args for symbol in prop_symbols(arg)}


def is_prop_symbol(s):
    """A proposition logic symbol is an initial-uppercase string.
    >>> is_prop_symbol('exe')
    False
    """
    return is_symbol(s) and s[0].isupper()

def is_symbol(s):
    """A string s is a symbol if it starts with an alphabetic char.
    >>> is_symbol('R2D2')
    True
    """
    return isinstance(s, str) and s[:1].isalpha()


def WalkSAT(clauses, numClauses, numVariables, p=0.5, max_flips=10000):
    """Checks for satisfiability of all clauses by randomly flipping values of variables

        Modified: Aug 9 2020, Curtis Lui - accepts clauses in form of complete CNF

    """
    # Set of all symbols in all clauses
    # print("Clauses:")
    # print(clauses)
    # print()
    symbols = {i for i in range(1,int(numVariables)+1)}

    # print("Symbols:")
    # print(symbols)
    # print()

    # model is a random assignment of true/false to the symbols in clauses

    model = {s: random.choice([True, False]) for s in symbols}
    
    # print("model:")
    # print(model)
    # print()
    for i in range(max_flips):
        satisfied, unsatisfied = [], []
        for clause in clauses:
            (satisfied if pl_true(clause, model) else unsatisfied).append(clause)
        
        # print("model:")
        # print(model)
        # print()

        # print("unsatisfied:")
        # print(unsatisfied)
        # print()

        if not unsatisfied:  # if model satisfies all the clauses
            return model
        
        clause = random.choice(unsatisfied)

        if probability(p):
            sym = random.choice(clause)
        else:
            # Flip the symbol in clause that maximizes number of sat. clauses
            def sat_count(sym):
                # Return the the number of clauses satisfied after flipping the symbol.
                model[abs(int(sym))] = not model[abs(int(sym))]
                count = len([clause for clause in clauses if pl_true(clause, model)])
                model[abs(int(sym))] = not model[abs(int(sym))]
                return count

            sym = max(clause, key=sat_count)
        model[abs(int(sym))] = not model[abs(int(sym))]
    # If no solution is found within the flip limit, we return failure
    return None

def runWalkSat():
    if len(sys.argv) != 2:
        print("ERROR: Please use the executable like this: python3 walksat.py <path to CNF file>\n")
        exit()
    
    try:
        f = open(str(sys.argv[1]))
    except:
        print(f"ERROR: '{sys.argv[1]}' could not be opened\n")
        exit()

    # parse first line info
    line = f.readline()
    newLine = line.split()
    
    numVariables = newLine[2]
    numClauses = newLine[3]

    # print(numVariables, numClauses)

    clauses = []

    # Create clause list
    for line in f:
        newLine = line.split()
        newLine.remove('0')
        clauses.append(newLine)
        # print(clauses)

    start_time = time.time()
    model = WalkSAT(clauses, numVariables, numClauses)
    elapsed_time = time.time() - start_time

    print("WalkSAT Running time:", elapsed_time)

    if not model:
        print("File is unsat")
    return

if __name__ == '__main__':
    runWalkSat()