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
        void displayModel(map<int, bool>& model);
        bool checkModel(map<int, bool>& model);
    
    private:
        int numClauses;
        int numVariables;
        map<int, vector<int>> allClauses;   // the int uniquely idenfities each clause
        vector<int> unsatClauses;           // the int uniquely idenfities each clause

        void loadKB(char* filePath);
        int satCount(map<int, bool>& model, int symbol);

        // Helper Functions
        vector<string> split(const string &s, char delimiter) const;
};

#endif