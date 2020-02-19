#ifndef LETTERSPECS_HPP
#define LETTERSPECS_HPP

#include <tuple>

using SpecsTuple = std::tuple<char, unsigned int, unsigned int>;

const unsigned int SYMB = 0;
const unsigned int NB   = 1;
const unsigned int PTS  = 2;

class LetterSpecs
{
    private :

    char symbol;
    unsigned int points;
    unsigned int number;

    public :

    LetterSpecs(const SpecsTuple specs);

    unsigned int getAvailableNumber() const;

    void pickOne();
    void putOne();
};

#endif // LETTERSPECS_HPP
