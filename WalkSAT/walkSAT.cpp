#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <fstream>

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
    
}

/* Displays the variable and assignment */
void WalkSAT::displayModel(map<int, bool>& model)
{
    for (int i = 0; i < numVariables; i++) {
        // print the postitive version of the int unless it's negated
        cout << (i ?  model[i] : i * -1) << " ";
    }
}

/* Returns true if the model is satisfied */
bool WalkSAT::checkModel(map<int, bool>& model)
{

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
    ifstream in_file (filePath, ifstream::in);

    if (!(in_file.is_open())) {
        cout << "ERROR: Could not open: " << filePath << "\n";
        exit(EXIT_FAILURE);
    }

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

    // Go through the clauses
    for (int i = 0; i < numClauses; i++) {
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


/* Returns a vector of tokens from s that is split by the specified delimiter */
vector<string> WalkSAT::split(const string &s, char delimiter) const
{
    vector<string> tokens;
    int begIndex = 0;
    int subStrLen = 0;

    for (int i = 0; i < s.length(); i++) {
        if (s[i] == delimiter) {
            string subString = s.substr(begIndex, subStrLen);
            tokens.push_back(subString);
            begIndex = i + 1;
            subStrLen = 0;
        }
        else {
            subStrLen++;
        }
    }
    return tokens;
}

