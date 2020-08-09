#ifndef _WALKSAT_H_
#define _WALKSAT_H_

#include <vector>
#include <map>
#include <string>

using namespace std;

class WalkSAT {
    public:
        WalkSAT();
        ~WalkSAT();

        map<int, bool> solve(int p, int max_flips);
        void loadKB(char* filePath);
        void displayModel(map<int, bool> &model);
        bool checkModel(const map<int, bool> &model);
    
    private:
        int numClauses;
        int numVariables;
        map<int, vector<int>> allClauses;   // the int uniquely idenfities each clause
        vector<int> unsatClauses;           // the int uniquely idenfities each clause

        
        int satCount(map<int, bool> &model, int symbol);
        bool checkClause(const vector<int> &clause, const map<int, bool> &model);
        void updateUnsatList(map<int, bool> &model, int flippedSymbol);

        // Helper Functions
        vector<string> split(const string &s, char delimiter) const;
        bool symbolInClause(vector<int>& clause, int symbol) const;
};

#endif