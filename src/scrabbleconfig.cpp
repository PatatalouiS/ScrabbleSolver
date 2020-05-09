#include <fstream>

#include "scrabbleconfig.hpp"

using namespace std;

ScrabbleConfig ScrabbleConfig::loadFromFile(const string& path) {
    ifstream txt(path);

    if(!txt.is_open()) {
        cout << "Error reading the ScrabbleConfig at path " << path << endl;
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
