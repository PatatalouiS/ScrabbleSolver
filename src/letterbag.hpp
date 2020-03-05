#ifndef LETTERBAG_HPP
#define LETTERBAG_HPP

#include <array>
#include <vector>
#include <random>

class LetterBag {

    private :

        unsigned int nbLetters = 0;

        std::vector<char> bag;

        static const unsigned int nbSymbols = 26;

        static const std::array<unsigned int, nbSymbols> symbolPoints;

        static const std::array<unsigned int, nbSymbols> symbolOccurrences;

        static std::mt19937 generator;

    public :

        LetterBag();

        char pickRandomLetter();

        unsigned int getNbLetters() const;

        bool isEmpty() const;

        void printBagContent() const;

        static unsigned int getLetterPoints(const char letter);
};

#endif // LETTERBAG_HPP
