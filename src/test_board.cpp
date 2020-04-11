#include "board.hpp"

#include <iostream>
#include <sstream>
#include <random>
#include <fstream>
#include <cassert>
#include <utility>
#include <unordered_map>
#include "letterbag.hpp"
#include "gaddag.hpp"
#include "solver.hpp"
#include "playerbag.hpp"

using namespace std;

const string DICO_PATH = "./data/dico.txt";
const string DICO_PATH_TEST = "./data/dico_test.txt";
const string DICO_PATH_TEST2 = "./data/dico2.txt";
const string CONFIG_BOARD = "./data/config_board.txt";

void createDicoTest(const string& pathIN, const string& pathOUT, int modulo) {
    ifstream stream(pathIN);

    if(!stream.is_open()) {
        cout << "Error read" << endl;
        exit(EXIT_FAILURE);
    }

    ofstream newfic(pathOUT);

    if(!newfic.is_open()) {
        cout << "Error write" << endl;
        exit(EXIT_FAILURE);
    }

    int i = 0;
    string buffer;

    while(stream >> buffer) {

        if((i % modulo) == 0) {
            newfic << buffer << endl;
        }

        ++i;
    }
    stream.close();
    newfic.close();
}


void test_search(Gaddag& abr) {
    ifstream stream(DICO_PATH_TEST);

    if(!stream.is_open()) {
        cout << "Error read" << endl;
        exit(EXIT_FAILURE);
    }

    string buffer;

    while(stream >> buffer) {
        assert(abr.search(buffer));
    }

    stream.close();
}

pair<stringstream, vector<unsigned char>> load_from_file(const string& path) {
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

    return { move(ss), letters };
}


int main() {
    auto params = load_from_file(CONFIG_BOARD);

    cout << "Letters : " << string(params.second.begin(), params.second.end()) << endl;

    Game game(DICO_PATH, params.first, params.second);

    Solver solver(game);

    solver.solveNext();

    std::cout << game.board << std::endl ;

  return 0 ;
}
