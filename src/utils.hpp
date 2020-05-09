#ifndef UTILS_HPP
#define UTILS_HPP

#include "solver.hpp"

namespace Utils {

    unsigned int charToIndex(const unsigned char letter);

    unsigned char indexTochar(const unsigned int index);

    unsigned int getLetterPoints(const unsigned char letter);

    std::string toRegularWord(const std::string& str);

    bool validIndex(const char index);

    bool validPos(const SpotPos& spot);

    SpotPos startPosStroke(const Stroke& stroke);

    void clearScreen();

    void printHeader();

    void parseArgs(int argc, char ** argv);
};

#endif // UTILS_HPP
