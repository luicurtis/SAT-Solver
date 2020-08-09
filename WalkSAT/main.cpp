#include <iostream>
#include <chrono>
#include <map>

#include "walkSAT.h"

using namespace std;
using namespace std::chrono;

int main(int argc, char* argv[])
{
    if (argc != 2) {
        cout << "ERROR: Please use the executable like this: ./walksat <path to KB file> \n";
        exit(EXIT_FAILURE);
    }
    WalkSAT* walksat = new WalkSAT();
    walksat->loadKB(argv[1]);

    auto start = high_resolution_clock::now();
    map<int, bool> result = walksat->solve(0.5, 10000);
    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop - start);

    cout << "DURATION FOR WALKSAT: " << duration.count()/1000.0 << endl;

    delete walksat;

    return 0;
}