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

        map<int, bool> solve(char* filePathtoKB);
        void displayModel(map<int, bool>& model);
        bool checkModel(map<int, bool>& model);
    
    private:
        int numClauses;
        int numVariables;
        map<string, vector<int>> clauses;

        void loadKB(char* filePath);
        int satCount(map<int, bool>& model, int symbol);
};

#endif