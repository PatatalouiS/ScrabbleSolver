#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include "solver.hpp"

    class Utils {

        public:

            static unsigned int charToIndex(const unsigned char letter);
            static unsigned char indexTochar(const unsigned int index);
            static std::string toRegularWord(const std::string& str);
            static bool validIndex(const char index);
            static bool validSpot(const SpotPos& spot);
    };

#endif // UTILS_HPP
