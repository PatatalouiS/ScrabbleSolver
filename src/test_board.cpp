#include <iostream>

#include "utils.hpp"
#include "solver.hpp"

// Constants, PATHS
const std::string DICO_PATH = "./data/dico.txt";
const std::string CONFIG_BOARD = "./data/config_board.txt";

using namespace std;
using namespace Utils;

int main(int argc, char** argv) {
    // parse command line args
    Options options = parseArgs(argc, argv);
    validOptions(options);
    // Create stuff for solve
    Gaddag gaddag(DICO_PATH);
    Solver solver(gaddag, options.suzette_check, options.jokers);

    if(options.loop) {
        while(true) {
            solver.solveFromScratch();
        }
    }
    else {
        // Load the Scrabble config in the File
        ScrabbleConfig config = ScrabbleConfig::loadFromFile(CONFIG_BOARD);
        // if config is valid, run the solve loop
        if(validConfig(config, options.jokers)) {
            solver.solveConfig(config);
        }
    }

    cout << endl << "Software is closing..." << endl;
    return 0 ;
}
