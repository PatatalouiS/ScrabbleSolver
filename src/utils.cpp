#include <cassert>
#include <getopt.h>

#include "utils.hpp"

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
    if(letter == JOKER_SYMBOL) {
        return 0;
    }

    assert(letter <= 'Z' && letter >= 'A');

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

bool Utils::validConfig(const ScrabbleConfig& config, const bool jokers) {
    const auto& [ playerBag, board ] = config;

    if(playerBag.size() > PlayerBag::MAX_SIZE) {
        return false;
    }

    unsigned int nbJokers = 0;

    for(unsigned char letter : playerBag.data()) {
        if(letter == JOKER_SYMBOL) {
            nbJokers++;
        }
    }

    if(!jokers && (nbJokers != 0)) {
        cerr << "/!\\ Jokers detected but Joker Mode is disabled" << endl
             << "     Please run with --jokers flag if you want to use jokers ! "
             << "Abort... /!\\" << endl;
        return false;
    }
    else if(!jokers && (nbJokers > 2)) {
        cerr << "/!\\ Maximum number of Jokers is 2 ! Abort... /!\\" << endl;
        return false;
    }
    return true;
}

SpotPos Utils::startPosMove(const Move& Move) {
    SpotPos start(Move.pos);

    char& movingIndex = Move.direction == Direction::HORIZONTAL
            ?  start.indexCol
            :  start.indexLine;

    movingIndex -= (Move.word.find(Gaddag::LINK_LETTER) - 1);
    return start;
}

void Utils::clearScreen() {
    std::cout << "\x1B[2J\x1B[H";
}

void Utils::printHeader() {
    cout << "---------- SCRABBLE SOLVER V1.0 ----------" << endl
         << " By CERBULEAN Ana-Maria and OLIVIE Maxime" << endl << endl;
}

void Utils::validOptions(const Utils::Options& opt) {
    const auto [ loop, jokers, suzette_check ] = opt;
    clearScreen();
    printHeader();

    cout << "Loop Mode : "
         << (loop ? "enabled" : "disabled") << endl
         << "Use Jokers : "
         << (jokers ? "enabled" : "disabled") << endl
         << "Suzette Check : "
         << (suzette_check ? "enabled" : "disabled") << endl << endl;

    if(jokers && suzette_check) {
        cout << "/!\\ Warning : Suzette isn't able to play with jokers" << endl
             << "     but --jokers option is enabled. The score by Suzette" << endl
             << "     will be wrong. /!\\" << endl << endl;
    }

    cout << "--> Press ENTER Key to construct Gaddag and run Solver..." << endl;

    string temp;
    getline(cin, temp);
    clearScreen();
    printHeader();
}

void Utils::waitForEnter() {
    cout << "    ----> Press ENTER Key to see Next Move..." << endl;
    string wait;
    getline(cin, wait);
}

Utils::Options Utils::parseArgs(int argc, char ** argv) {
    int opt;

    Utils::Options options;

    while(true) {
        static struct option long_options[] = {
            { "suzette-check", no_argument, nullptr, 's' },
            { "loop", no_argument, nullptr, 'l' },
            { "jokers", no_argument, nullptr, 'j' },
            { nullptr, 0, nullptr, 0 }
        };

        int option_index = 0;

        opt = getopt_long(argc, argv, "m", long_options, &option_index);

        if(opt == -1) {
            break;
        }

        switch(opt) {
            case 's' :
                options.suzette_check = true;
                break;

            case 'l' :
                options.loop = true;
                break;

            case 'j' :
                options.jokers = true;
                break;

            case '?' : break;

            default : abort ();
        }
    }

    return options;
}

string Utils::urlencode(const std::string& str) {
    static const char lookup[]= "0123456789abcdef";
    std::stringstream e;
    for(unsigned long i = 0, ix = str.length(); i < ix; i++) {
        const char& c = str[i];
        if ( (48 <= c && c <= 57) ||
             (65 <= c && c <= 90) ||
             (97 <= c && c <= 122) ||
             (c=='-' || c=='_' || c=='.' || c=='~')
        ){
            e << c;
        }
        else{
            e << '%';
            e << lookup[ (c&0xF0)>>4 ];
            e << lookup[ (c&0x0F) ];
        }
    }
    return e.str();
}
