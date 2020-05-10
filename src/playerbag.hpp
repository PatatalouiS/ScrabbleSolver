#ifndef PLAYERBAG_HPP
#define PLAYERBAG_HPP

#include <array>
#include "letterbag.hpp"

class PlayerBag {

    using PlayerLetters = std::vector<unsigned char>;

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

         static const unsigned int MAX_SIZE = 7;

    private :

        PlayerLetters playerLetters;
};

std::ostream& operator << (std::ostream& stream, const PlayerBag& pb);

#endif // PLAYERBAG_HPP
