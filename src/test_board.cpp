#include <iostream>

#include "utils.hpp"
#include "globals.hpp"
#include "solver.hpp"

using namespace std;
using namespace Utils;

int main(int argc, char** argv) {
    Options options = parseArgs(argc, argv);
    printOptions(options);

    Gaddag gaddag(DICO_PATH);
    Solver solver(gaddag, options.suzette_check, options.jokers);

    if(options.loop) {
        while(true) {
            solver.solveFromScratch();
        }
    }
    else {
        ScrabbleConfig config = ScrabbleConfig::loadFromFile(CONFIG_BOARD);
        if(validConfig(config, options.jokers)) {
            solver.solveConfig(config);
        }
    }

    cout << endl << "Software is closing..." << endl;
    return 0 ;
}
