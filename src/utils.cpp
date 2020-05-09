#include <cassert>
#include <vector>
#include <array>
#include <getopt.h>

#include "utils.hpp"
#include "globals.hpp"

using namespace std;

unsigned int Utils::charToIndex(const unsigned char letter) {
    assert((letter <= 'Z' && letter >= 'A') || (letter == '+'));

    return letter == '+'
            ? 26
            : letter - 'A';
}

unsigned char Utils::indexTochar(const unsigned int index) {
    assert(index <= 0  && index >= 26);

    return static_cast<unsigned char>(index + 'A');
}

unsigned int Utils::getLetterPoints(const unsigned char letter) {
    static const array points {
        1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 10, 1, 2,
        1, 1, 3, 8, 1, 1, 1, 1, 4, 10, 10, 10, 10
    };

    return static_cast<unsigned int>(points[charToIndex(letter)]);
}

string Utils::toRegularWord(const string& str) {
    assert(str.find('+') != string::npos);

    std::vector<char> previous;
    string out;

    unsigned int i = 0;

    for(i = 0; str[i] != '+'; ++i) {
        previous.push_back(str[i]);
    }

    for(unsigned long j = previous.size() - 1; int(j) >= 0; j--) {
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

void Utils::clearScreen() {
    std::cout << "\x1B[2J\x1B[H";
}

void Utils::printHeader() {
    cout << endl << "---------- SCRABBLE SOLVER V1.0 ----------" << endl
         << " By CERBULEAN Ana-Maria and OLIVIE Maxime" << endl << endl;
}

void Utils::parseArgs(int argc, char ** argv) {
    int opt;

    while(true) {
        static struct option long_options[] = {
            { "suzette-check", no_argument, nullptr, 's' },
            { "mode", required_argument, nullptr, 'm' },
            { nullptr, 0, nullptr, 0 }
        };

        int option_index = 0;

        opt = getopt_long(argc, argv, "m", long_options, &option_index);

        if(opt == -1) {
            break;
        }

        switch(opt) {
            case 's':
              SUZETTE_CHECK = true;
              break;

            case 'm':
              if((string(optarg)) != "loop" && (string(optarg) != "singleshot")) {
                  cerr << "Usage : Parameters for option --mode must be 'loop' or 'singleshot'" << endl;
                  cerr << "Instead, singleshot is used" << endl;
                  MODE = "singleshot";
              }
              else {
                   MODE = string(optarg);
              }
              break;

            case '?':
              break;

            default:
              abort ();
        }
    }
}
