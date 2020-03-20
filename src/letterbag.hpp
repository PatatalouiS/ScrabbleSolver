#ifndef LETTERBAG_HPP
#define LETTERBAG_HPP

#include <array>
#include <vector>
#include <random>

class LetterBag {

    private : 

        struct LetterSpecs {
            unsigned int points;
            unsigned int occurrences;
        };

        unsigned int nbLetters = 0;

        std::vector<unsigned char> bag;

        static const unsigned int nbSymbols = 26;

        static const std::array<LetterSpecs, nbSymbols> letterSpecsArray;

        static std::mt19937 generator;

    public :

        LetterBag();

        unsigned char pickRandomLetter();

        unsigned int getNbLetters() const;

        bool isEmpty() const;

        void printBagContent() const;

        static unsigned int getLetterPoints(const unsigned char letter);
};

#endif // LETTERBAG_HPP
