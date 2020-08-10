#ifndef _RES_PROVING_H_
#define _RES_PROVING_H_

#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

class ResolutionProving
{
    public:
        ResolutionProving();
        ~ResolutionProving();

        bool solve();
        void loadKB(char* filePath);
    
    private:
        int numClauses;     // number of clauses from the input file
        int numVariables;   // number of variables from the input file
        map<string, set<int>> clauses;

        map<string, set<int>> pl_resolve(const set<int> &ci, const set<int> &cj);

        // Helper Functions
        vector<string> split(const string &s, char delimiter) const;
        string clause_to_string(const set<int> &clause) const;
};

#endif 