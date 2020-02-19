#ifndef LETTERBAG_HPP
#define LETTERBAG_HPP

#include <array>
#include <tuple>
#include <vector>
#include <memory>
#include "letterspecs.hpp"

class LetterBag
{
    private :

    unsigned int nbLetters = 0;
    static const unsigned int nbSymbols = 26;
    static const std::array<SpecsTuple, nbSymbols> initTab;

    std::array<std::unique_ptr<LetterSpecs>, nbSymbols> specs;
    std::vector<char> bag;

    public :

    LetterBag();
    char pickRandomLetter();
    unsigned int getNbLetters() const;
    bool isEmpty() const;
    void printBagContent() const;

    static unsigned int getScore(const char letter);

};

#endif // LETTERBAG_HPP
