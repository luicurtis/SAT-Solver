''' Source code from aima-python repository 
    Edited to isolate the WalkSAT algorihtm for comparing 
    and to accept CNF formatted clauses '''

import time
import sys

def pl_resolution(clauses):
    """ Modeified: Aug 10, 2020, Curtis Lui - Edited to accept CNF clauses """
    
    new = set()
    while True:
        n = len(clauses)
        pairs = [(clauses[i], clauses[j])
                 for i in range(n) for j in range(i + 1, n)]
        for (ci, cj) in pairs:
            resolvents = pl_resolve(ci, cj)
            if False in resolvents:
                return True
            new = new.union(set(resolvents))
            
        if new.issubset(set(clauses)):
            return False
        for c in new:
            if c not in clauses:
                clauses.append(c)


def pl_resolve(ci, cj):
    """Return all clauses that can be obtained by resolving clauses ci and cj."""
    clauses = []
    for di in (ci):
        for dj in (cj):
            if int(di) == (int(dj) * -1 ):
                temp = ci
                newClause = cj
                temp.remove(di)
                newClause.remove(dj)
                
                if len(newClause) > 0 or len(temp) > 0:
                    
                    clauses.append(frozenset(newClause + temp))
                else:
                    clauses.append(False)
    return clauses

def run_pl_resolution():
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

    clauses = []

    # Create clause list
    for line in f:
        newLine = line.split()
        newLine.remove('0')
        clauses.append(newLine)
        # print(clauses)

    start_time = time.time()
    result = pl_resolution(clauses)
    elapsed_time = time.time() - start_time

    print("Resolution Running time:", elapsed_time)

    if result:
        print("Entailment proven")
    else:
        print("KB can not prove entailment")

    return

    

if __name__ == '__main__':
    run_pl_resolution()