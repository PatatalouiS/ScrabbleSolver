#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

class Utils {

    public:

        static unsigned int charToIndex(const unsigned char letter);
        static unsigned char indexTochar(const unsigned int index);
        static std::string toRegularWord(const std::string& str);
};

#endif // UTILS_HPP
