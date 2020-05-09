#include <iostream>

#include "utils.hpp"
#include "globals.hpp"
#include "solver.hpp"

using namespace std;
using namespace Utils;

std::string MODE = "singleshot";
bool SUZETTE_CHECK = false;

int main(int argc, char** argv) {
    clearScreen();
    parseArgs(argc, argv);

    printHeader();
    cout << "Used MODE : " << MODE << endl;
    cout << "Suzette Check : "
         << (SUZETTE_CHECK ? "enabled" : "disabled") << endl;

    Gaddag gaddag(DICO_PATH);

    Solver solver(gaddag);

    if(MODE == "singleshot") {
        ScrabbleConfig config = ScrabbleConfig::loadFromFile(CONFIG_BOARD);
        solver.solveConfig(config);
    }
    else {
        while(true) {
            solver.solveFromScratch();
        }
    }

    cout << endl << "Software is closing..." << endl;
    return 0 ;
}
