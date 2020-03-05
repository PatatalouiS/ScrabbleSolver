#ifndef PLAYERBAG_HPP
#define PLAYERBAG_HPP

#include <array>
#include "letterbag.hpp"


class PlayerBag {

    public:

     PlayerBag(LetterBag& mainLetterBag);

    private :

        static const unsigned int nbLetters = 7;

        std::array<char, nbLetters> playerLetters;
};

#endif // PLAYERBAG_HPP
