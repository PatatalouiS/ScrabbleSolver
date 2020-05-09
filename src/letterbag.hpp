#ifndef LETTERBAG_HPP
#define LETTERBAG_HPP

#include <vector>
#include <random>

class LetterBag {

    private : 

        static const unsigned int NB_SYMBOLS = 26;

        unsigned int nbLetters = 0;

        std::vector<unsigned char> bag;

        static std::mt19937 generator;

    public :

        LetterBag();

        unsigned char pickRandomLetter();

        unsigned int getNbLetters() const;

        bool isEmpty() const;

        void printBagContent() const;
};

#endif // LETTERBAG_HPP
