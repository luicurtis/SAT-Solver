#include <vector>
#include <map>
#include <string>

#include "walkSAT.h"

WalkSAT::WalkSAT()
{
    numClauses = 0;
    numVariables = 0;
}

WalkSAT::~WalkSAT()
{

}

/* Checks satisfiability of all clauses by randomly flipping values of variables
 * The algorithm is based off of the AIMA-Python Library 
 * 
 * Input: filePathtoKB - relative path to the .txt KB file
 * 
 * Return: a map that is the model of each variable and it's value
 */
map<int, bool> solve(char* filePathtoKB)
{
    
}

/* Displays the variable and assignment */
void displayModel(map<int, bool>& model)
{

}

/* Returns true if the model is satisfied */
bool checkModel(map<int, bool>& model);