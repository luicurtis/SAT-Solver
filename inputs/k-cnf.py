''' Generates a k-CNF sentence with m clauses and n symbols, where the clauses 
    are chosen uniformly, independently, and without replacement from 
    among all clauses with k different literals, which are positive or 
    negative at random'''
import sys

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

    k = argumentList[0]
    m = argumentList[1]
    n = argumentList[2]

    
    return

if __name__ == '__main__':    
    k_cnf()