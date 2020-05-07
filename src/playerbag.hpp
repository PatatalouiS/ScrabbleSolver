#ifndef PLAYERBAG_HPP
#define PLAYERBAG_HPP

#include <array>
#include "letterbag.hpp"

class PlayerBag {

    private :

        static const unsigned int nbLettersMAX = 7;

        using PlayerLetters = std::vector<unsigned char>;

        PlayerLetters playerLetters;

    public:

         PlayerBag();

         PlayerBag(LetterBag& mainLetterBag);

         PlayerBag(const PlayerBag& bag);

         PlayerBag(const PlayerLetters& letters);

         const PlayerLetters& data() const;

         bool isEmpty() const;

         PlayerLetters& data();

         PlayerBag& fillWith(LetterBag& mainLetterBag);

         PlayerBag pop(const unsigned char letter) const;

         void print() const;
};

std::ostream& operator << (std::ostream& stream, const PlayerBag& pb);

#endif // PLAYERBAG_HPP
