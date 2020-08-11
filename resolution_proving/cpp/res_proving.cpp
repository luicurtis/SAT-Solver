#include <map>
#include <set>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "res_proving.h"

using namespace std;

#define EMPTY_CLAUSE_INDICATOR "0"

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
    map<string, set<int>> resolvedClauses;
    bool noNewClausesFlag = false;
    int past_size = 0;

    // Loop until no clauses can be resolved, or an empty clause is created
    while(true) {
        past_size = newClauses.size();  // keep track of number of new clauses

        // Iterate all possible pairs of clauses
        for (auto ci = clauses.begin(); ci != clauses.end(); ci++) {
            for (auto cj = ci; cj != clauses.end(); cj++) {
                // resolve pair of clauses
                resolvedClauses = pl_resolve(ci->second, cj->second);

                // Check if there is an empty clause created from resolving
                if (resolvedClauses.find(EMPTY_CLAUSE_INDICATOR) != resolvedClauses.end()) {
                    return true;
                }

                // add all resolved clauses to new
                newClauses.insert(resolvedClauses.begin(), resolvedClauses.end());
            }
        }

        if (newClauses.size() == past_size) {
            // no new clauses were added since last loop
            // Therefore, no more new clauses will be found
            return false;
        }

        // add newClauses into master list of clauses
        // note: map.insert() will not have duplicates
        clauses.insert(newClauses.begin(), newClauses.end());
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

    // cout<<numVariables<<endl;
    // cout<<numClauses<<endl;

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

    // for (auto it = clauses.begin(); it != clauses.end(); it++) {
    //     cout << it->first << endl;
    // }

    in_file.close();
}

/* Returns all clauss that are resolved from ci and cj */
map<string, set<int>> ResolutionProving::pl_resolve(const set<int> &ci, const set<int> &cj)
{
    map<string, set<int>> resolvedClauses;
    set<int> temp_i;
    set<int> newClause;
    for (auto lit_i = ci.begin(); lit_i != ci.end(); lit_i++) {
        for (auto lit_j = cj.begin(); lit_j != cj.end(); lit_j++) {
            if (*lit_i == ((*lit_j  * - 1))) {
                // remove resolving literals from clause and combine
                temp_i = ci;
                newClause = cj;

                temp_i.erase(*lit_i);
                newClause.erase(*lit_j);

                newClause.insert(temp_i.begin(), temp_i.end());
                string s_clause = clause_to_string(newClause);

                if (newClause.size() == 0) {
                    s_clause = "0";
                }
                pair<string, set<int>> newPair = make_pair(s_clause, newClause);
                resolvedClauses.insert(newPair);
            }
        }
    }

    return resolvedClauses;

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