#include <iostream>
#include <chrono>
#include <map>

#include "res_proving.h"

using namespace std;
using namespace std::chrono;

int main(int argc, char* argv[])
{
    if (argc != 2) {
        cout << "ERROR: Please use the executable like this: ./resolutionProver <path to CNF file> \n";
        exit(EXIT_FAILURE);
    }
    ResolutionProving* resProving = new ResolutionProving();
    resProving->loadKB(argv[1]);
    
    auto start = high_resolution_clock::now();
    bool result = resProving->solve();
    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop - start);

    cout << "DURATION FOR WALKSAT: " << duration.count()/1000000.0 << endl;
    
    if (!result) {
        cout << "Could not be resolved. Unsatisfiable.";
    }

    delete resProving;

    return 0;
}