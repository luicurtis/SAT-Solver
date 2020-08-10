#include <map>
#include <set>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "res_proving.h"

using namespace std;


ResolutionProving::ResolutionProving()
{
    numClauses = 0;
    numVariables = 0;
}

ResolutionProving::~ResolutionProving() {}

/* Returns true if the clauses are resolvable, else false */
bool ResolutionProving::solve()
{
    map<string, set<int>> newClauses;
    int n = 0;

    while(true) {
        n = clauses.size();
        
    }


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
void ResolutionProving::loadKB(char* filePath)
{
    string newLine;
    ifstream in_file (filePath);
    
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

    cout<<numVariables<<endl;
    cout<<numClauses<<endl;

    // Go through the clauses
    for (int i = 0; i < numClauses; i++) {
        getline(in_file, newLine);
        tokens = split(newLine, ' ');
    
        set<int> clause;
        // convert tokens into integers and put into clause map
        for (int j = 0; j < tokens.size(); j++) {
            if (tokens[j] != "0") {
                clause.insert(stoi(tokens[j]));
            }
            
        }
        
        string s_clause = clause_to_string(clause);
        
        pair<string, set<int>> newClause = make_pair(s_clause, clause);
        clauses.insert(newClause);
    }

    for (auto it = clauses.begin(); it != clauses.end(); it++) {
        cout << it->first << endl;
    }

    in_file.close();
}

/* Returns all clauss that are resolved from ci and cj */
map<string, set<int>> ResolutionProving::pl_resolve(const set<int> &ci, const set<int> &cj)
{

}

// Helper Functions
/* Returns a vector of tokens from s that is split by the specified delimiter */
vector<string> ResolutionProving::split(const string &s, char delimiter) const
{
    vector<string> tokens;
    int begIndex = 0;
    int subStrLen = 0;

    for (int i = 0; i < s.length(); i++) {
        // cout << i << endl;
        // cout << s[i] << endl;
        if (s[i] == delimiter || s[i] == '\n') {
            string subString = s.substr(begIndex, subStrLen);
            // cout << subString << endl;
            tokens.push_back(subString);
            begIndex = i + 1;
            subStrLen = 0;
        }
        else {
            subStrLen++;
        }
    }

    string subString = s.substr(begIndex, subStrLen);
    // cout << subString << endl;
    tokens.push_back(subString);

    return tokens;
}

/* Retuns a string. Converts given clause to a string */
string ResolutionProving::clause_to_string(const set<int> &clause) const
{
    string s_clause;
    for (auto it = clause.begin(); it != clause.end(); it++) {
        s_clause += to_string(*it) + " " ;
    }

    return s_clause;
}