#ifndef LETTERBAG_HPP
#define LETTERBAG_HPP

#include <vector>
#include <random>

const unsigned char JOKER_SYMBOL = '?';

class LetterBag {
    public :

    // Methods

    LetterBag(const bool useJokers = false);

    unsigned char pickRandomLetter();

    unsigned int getNbLetters() const;

    bool isEmpty() const;

    const std::vector<unsigned char>& data() const;

    private :

    // members

    static const unsigned int NB_SYMBOLS = 26;

    static const unsigned int NB_JOKERS = 2;

    unsigned int nbLetters = 0;

    std::vector<unsigned char> bag;

    static std::mt19937 generator;
};

std::ostream& operator<<(std::ostream& out, const LetterBag& lb);

#endif // LETTERBAG_HPP




