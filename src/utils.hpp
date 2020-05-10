#ifndef UTILS_HPP
#define UTILS_HPP

#include "solver.hpp"

namespace Utils {

    struct Options {
        bool loop = false;
        bool jokers = false;
        bool suzette_check = false;
    };

    Options parseArgs(int argc, char ** argv);

    unsigned int charToIndex(const unsigned char letter);

    unsigned char indexTochar(const unsigned int index);

    unsigned int getLetterPoints(const unsigned char letter);

    std::string toRegularWord(const std::string& str);

    bool validIndex(const char index);

    bool validPos(const SpotPos& spot);

    bool validConfig(const ScrabbleConfig& config, const bool jokers);

    SpotPos startPosStroke(const Stroke& stroke);

    void clearScreen();

    void printHeader();

    void printOptions(const Options& opt);
};

#endif // UTILS_HPP
