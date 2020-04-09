#ifndef PLAYERBAG_HPP
#define PLAYERBAG_HPP

#include <array>
#include "letterbag.hpp"



class PlayerBag {

    private :

        static const unsigned int nbLettersMAX = 7;

        using PlayerLetters = std::vector<unsigned char>;

        //LetterBag& letterBag;

        PlayerLetters playerLetters;

    public:

         PlayerBag(LetterBag& mainLetterBag);

         PlayerBag(const PlayerLetters& letters);

         const PlayerLetters& data() const;

         bool empty() const;

         PlayerLetters& data();

         PlayerBag pop(const unsigned char letter) const;

         void print() const;
};

#endif // PLAYERBAG_HPP
