#include "utils.hpp"
#include <cassert>
#include <vector>
#include "stroke.hpp"

using namespace std;

unsigned int Utils::charToIndex(const unsigned char letter) {
   // assert((letter <= 'Z' && letter >= 'A') || (letter == '+'));

    return letter == '+'
            ? 26
            : letter - 'A';
}

unsigned char Utils::indexTochar(const unsigned int index) {
   // assert(index <= 0  && index >= 26);

    return static_cast<unsigned char>(index + 'A');
}

string Utils::toRegularWord(const string& str) {
    assert(str.find('+') != string::npos);

    std::vector<char> previous;
    string out;

    unsigned int i = 0;

    for(i = 0; str[i] != '+'; ++i) {
        previous.push_back(str[i]);
    }

    for(unsigned long j = previous.size()-1; int(j) >= 0; j--) {
        out.push_back(previous[j]);
    }

    for(unsigned long j = i+1; j < str.size(); j++) {
        out.push_back(str[j]);
    }

    return out;
}

bool Utils::validIndex(const char index) {
    return (index >= 0) && (index < static_cast<char>(Board::SIZE));
}

bool Utils::validPos(const SpotPos &spot) {
    return validIndex(spot.indexLine) && validIndex(spot.indexCol);
}

SpotPos Utils::startPosStroke(const Stroke& stroke) {
    SpotPos start(stroke.pos);

    char& movingIndex = stroke.direction == Direction::HORIZONTAL
            ?  start.indexCol
            :  start.indexLine;

    movingIndex -= (stroke.word.find(LINK_LETTER) - 1);
    return start;
}
