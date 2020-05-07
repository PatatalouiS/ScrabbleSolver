#include <iostream>
#include <sstream>
#include <fstream>

#include "solver.hpp"
#include "globals.hpp"

using namespace std;

extern const string DICO_PATH;
extern const string CONFIG_START;
extern const string CONFIG_BOARD;

Config load_from_file(const string& path) {
    ifstream txt(path);

    if(!txt.is_open()) {
        cout << "Error read" << endl;
        exit(EXIT_FAILURE);
    }

    std::stringstream ss;
    string bufferBoard;

    for(unsigned int i = 0; i < Board::SIZE; ++i) {
        txt >> bufferBoard;
        ss << bufferBoard;
    }

    unsigned char bufferPlayerBag;
    vector<unsigned char> letters;

    while(txt >> bufferPlayerBag) {
        letters.push_back(bufferPlayerBag);
    }

    txt.close();

    return { PlayerBag(letters), Board(ss) };
}


int main(int argc, char** argv) {
    Config config = load_from_file(CONFIG_BOARD);

    Solver solver;

    if(string(argv[1]) == "--one") {
        cout << solver.solveConfig(config) << endl;
    }
    else {
        solver.solveFromScratch();
    }

    cout << "Software is closing..." << endl;

    return 0 ;
}
