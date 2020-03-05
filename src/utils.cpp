#include "utils.hpp"
#include <cassert>

unsigned int Utils::charToIndex(const unsigned char letter) {
    assert(letter <= 'Z' && letter >= 'A');
    return letter - 'A';
}

unsigned char Utils::indexTochar(const unsigned int index) {
    assert(index <= 255 && index >= 0);
    return static_cast<unsigned char>(index + 'A');
}
