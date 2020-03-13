#include "utils.hpp"
#include <cassert>

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
