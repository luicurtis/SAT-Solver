#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <time.h>
#include <cstdlib>
#include <random>
#include <algorithm>

#include "walkSAT.h"

using namespace std;

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
map<int, bool> WalkSAT::solve(int p, int max_flips)
{
    srand(time(0)); // for rand set up the seed

    map<int, bool> model;   // note: int cannot be 0
    // give random asignments for all the symbols
    for (int i = 1; i < numClauses + 1; i++) {
        if (rand() % 1) {
            model[i] = true;
        }
        else {
            model[i] = false;
        }
    }

    // Determine Unsat Clauses
    for (int i = 0; i < numClauses; i++) {
        if (!checkClause(allClauses[i], model)) {
            unsatClauses.push_back(i); // note: i is the unique identifier for that clause
        }
    }
    int numFlips = 0;

    while (numFlips < max_flips) {
        if (unsatClauses.size() == 0) {
            return model;
        }

        // Choose a random clause from unsat list
        vector<int> chosenClause = allClauses[rand() % unsatClauses.size()];

        // https://stackoverflow.com/questions/5886987/true-or-false-output-based-on-a-probability
        default_random_engine rand_engine;
        uniform_real_distribution<> uniform_zero_to_one(0.0, 1.0);

        // Randomly pick flipping algorithm with probability p
        int chosenVariable;

        if (uniform_zero_to_one(rand_engine) > p) {
            // flip a random symbol in the clause
            chosenVariable = chosenClause[rand() % chosenClause.size()];
        }
        else {
            // flip the symbol that maximizes the number of sat clauses
            int satClauseCount = 0; // initialize as the least number of possible satisfied clauses

            // Loop through each variable in the clause and flip. Compare number of sat clauses
            for (int i = 0; i < chosenClause.size(); i++) {
                int tempVar = abs(chosenClause[i]);
                int tempCount = satCount(model, tempVar);

                if (tempCount > satClauseCount) {
                    satClauseCount = tempCount;
                    chosenVariable = tempVar;
                }
            }   
        }

        model[chosenVariable] = !model[chosenVariable];     // flip variable
        updateUnsatList(model, chosenVariable);
        numFlips++;
    }

    // no solution found
    model.clear();
    model[0] = false;
    return model;
}

/* Loads a KB from a given filePath
 *
 * Note: This function does not check for validity of the input file
 *       It assumes that the input file is formatted in CNF with no comments
 *       and the "p" statement is the first line of the file followed by
 *       the clauses line by line   
 * 
 * Example input:
 *      p cnf 30 100
 *      -123 456 890 0
 *      123 -456 890 0 
 *      ... etc
 * 
 * */
void WalkSAT::loadKB(char* filePath)
{
    
    string newLine;
    ifstream in_file (filePath);
    
    if (!(in_file.is_open())) {
        cout << "ERROR: Could not open: " << filePath << "\n";
        exit(EXIT_FAILURE);
    }

    cout << "File Path: " << filePath << endl;

    // get the "p" line
    getline(in_file, newLine);
    
    vector<string> tokens = split(newLine, ' ');
    // Example  p cnf 30 100
    if (tokens[0] != "p" || tokens[1].compare("cnf")) {
        cout << "ERROR: File: " << filePath << "is not in correct CNF form: " << "\n";
        exit(EXIT_FAILURE);
    }
    numVariables = stoi(tokens[2]);
    numClauses = stoi(tokens[3]);

    // cout << "numvariables: " << numVariables << endl;
    // cout << "numClauses: " << numClauses << endl;

    // Go through the clauses
    for (int i = 0; i < numClauses + 1; i++) {
        getline(in_file, newLine);
        tokens = split(newLine, ' ');

        vector<int> clause;
        // convert tokens into integers and put into clause map
        for (int j = 0; j < tokens.size(); j++) {
            clause.push_back(stoi(tokens[j]));
        }
        // Note: i in this for loop uniquely identifies each clause
        pair<int, vector<int>> newClause = make_pair(i, clause);
        allClauses.insert(newClause);
        
    }
    in_file.close();
}

/* Displays the variable and assignment */
void WalkSAT::displayModel(map<int, bool>& model)
{
    for (int i = 0; i < numVariables; i++) {
        // print the postitive version of the int unless it's negated
        cout << (i ? model[i] : i * -1) << " ";
    }
}

/* Returns true if the model satisfies all the clauses */
bool WalkSAT::checkModel(const map<int, bool>& model)
{
    for (auto it = allClauses.begin(); it != allClauses.end(); it++) {
        if (!checkClause(it->second, model)) {
            // if model does not satisfy the clause
            return false;
        }
    }
    return true;
}


/* Return the number of clauses satisfied by flipping the specified symbol*/
int WalkSAT::satCount(map<int, bool> &model, int symbol)
{
    int count = 0;

    model[symbol] = !model[symbol]; // flip the symbol
    
    // loop through all clauses and check if they are satisifed by the current model
    for (auto it = allClauses.begin(); it != allClauses.end(); it++) {
        if (checkClause(it->second, model)) {
            count++;
        }
    }

    model[symbol] = !model[symbol]; // flip the symbol back to original value
    return count;
}

/* Check if the given clause is satisfied by the model 
 * Returns True if clause is satisfied, else false */
bool WalkSAT::checkClause(const vector<int>& clause, const map<int, bool>& model)
{
    // Loop through all variables in the clause to check if any are true and match
    for (auto it = clause.begin(); it != clause.end(); it++) {
        /* clause is true (satisfied) if:
         * - *it is positive and the model for that variable is true OR
         * - *it is negative and the model for that varaible is false*/
        if ((*it > 0 && model.at(abs(*it))) || (*it > 0 && !(model.at(abs(*it))))) {
            // Note: only one of the variables in each clause need to be true
            return true;
        }
    }
    return false;
}

void WalkSAT::updateUnsatList(map<int, bool> &model, int flippedSymbol)
{
    // Find all clauses with the flippedSymbol and update the unsatList
    for (auto it = allClauses.begin(); it != allClauses.end(); it++) {
        if (symbolInClause(it->second, flippedSymbol)) {
            if (checkClause(it->second, model)) {
                for (auto unsat_it = unsatClauses.begin(); unsat_it != unsatClauses.end(); unsat_it++) {
                    // Check to see if the clause is in the unsat list
                    if (*unsat_it == it->first) {
                    unsatClauses.erase(unsat_it); // remove the clause from the unsat list because it is satisfied
                    }
                }
            }
            else {
                unsatClauses.push_back(it->first); // add clause to list if it isnt satisfied
            }
        }
    }
}

/* Returns a vector of tokens from s that is split by the specified delimiter */
vector<string> WalkSAT::split(const string &s, char delimiter) const
{
    vector<string> tokens;
    int begIndex = 0;
    int subStrLen = 0;
    cout << "s.length(): " << s.length() << endl;

    

    for (int i = 0; i < s.length(); i++) {
        // cout << i << endl;
        // cout << s[i] << endl;
        if (s[i] == delimiter || s[i] == '\n') {
            string subString = s.substr(begIndex, subStrLen);
            cout << subString << endl;
            tokens.push_back(subString);
            begIndex = i + 1;
            subStrLen = 0;
        }
        else {
            subStrLen++;
        }
    }

    string subString = s.substr(begIndex, subStrLen);
    cout << subString << endl;
    tokens.push_back(subString);

    return tokens;
}

/* Returns True if the symbol is in the clause, else false */
bool WalkSAT::symbolInClause(vector<int>& clause, int symbol) const
{
    for (int i = 0; i < clause.size(); i++) {
        if (abs(clause[i]) == symbol) {
            return true;
        }
    }
    return false;
}