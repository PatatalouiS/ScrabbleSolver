#ifndef LETTERBAG_HPP
#define LETTERBAG_HPP

#include <vector>
#include <random>

const unsigned char JOKER_SYMBOL = '?';

class LetterBag {

    private : 

        static const unsigned int NB_SYMBOLS = 26;
        static const unsigned int NB_JOKERS = 2;

        unsigned int nbLetters = 0;

        std::vector<unsigned char> bag;

        static std::mt19937 generator;

    public :

        LetterBag(const bool jokers = false);

        unsigned char pickRandomLetter();

        unsigned int getNbLetters() const;

        bool isEmpty() const;

        void printBagContent() const;
};

#endif // LETTERBAG_HPP
