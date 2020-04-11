#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <utility>
#include "solver.hpp"

namespace Utils {

    unsigned int charToIndex(const unsigned char letter);

    unsigned char indexTochar(const unsigned int index);

    std::string toRegularWord(const std::string& str);

    bool validIndex(const char index);

    bool validPos(const SpotPos& spot);

    SpotPos startPosStroke(const Stroke& stroke);
};

#endif // UTILS_HPP
