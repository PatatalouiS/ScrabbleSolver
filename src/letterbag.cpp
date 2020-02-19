#include "letterbag.hpp"
#include <iostream>
#include <random>

using namespace std;

// Static Init Bloc

const array<SpecsTuple, LetterBag::nbSymbols> LetterBag::initTab {
    std::make_tuple('A', 9, 1),
    std::make_tuple('B', 2, 3),
    std::make_tuple('C', 2, 3),
    std::make_tuple('D', 3, 2),
    std::make_tuple('E', 15, 1),
    std::make_tuple('F', 2, 4),
    std::make_tuple('G', 2, 2),
    std::make_tuple('H', 2, 4),
    std::make_tuple('I', 8, 1),
    std::make_tuple('J', 1, 8),
    std::make_tuple('K', 1, 10),
    std::make_tuple('L', 5, 1),
    std::make_tuple('M', 3, 2),
    std::make_tuple('N', 6, 1),
    std::make_tuple('O', 6, 1),
    std::make_tuple('P', 2, 3),
    std::make_tuple('Q', 3, 2),
    std::make_tuple('R', 6, 1),
    std::make_tuple('S', 6, 1),
    std::make_tuple('T', 6, 1),
    std::make_tuple('U', 6, 1),
    std::make_tuple('V', 2, 4),
    std::make_tuple('W', 1, 10),
    std::make_tuple('X', 1, 10),
    std::make_tuple('Y', 1, 10),
    std::make_tuple('Z', 1, 10)
};

// Static Init Bloc


LetterBag::LetterBag() {
    for(unsigned int i = 0; i < nbSymbols; ++i) {
        specs[i] = std::make_unique<LetterSpecs>(initTab[i]);
    }

    for(auto& specs : initTab) {
        const char currentSymbol = get<SYMB>(specs);
        const unsigned int number = get<NB>(specs);

        for(unsigned int i = 0; i < number; ++i) {
            bag.push_back(currentSymbol);
            nbLetters++;
        }
    }
}

char LetterBag::pickRandomLetter() {
    try {
        if(isEmpty()) throw string("ERROR : No letters in the bag");
    }
    catch(string& error) {
        cerr << error << endl;
        exit(EXIT_FAILURE);
    }

    random_device rd;
    uniform_int_distribution<unsigned int> distrib(0, nbLetters-1);
    mt19937 gen(rd());

    const unsigned int random = distrib(gen);
    const char selectedLetter = bag[random];
    bag[random] = bag[nbLetters-1];
    nbLetters--;

    return selectedLetter;
}

unsigned int LetterBag::getNbLetters() const {
    return nbLetters;
}

bool LetterBag::isEmpty() const {
    return nbLetters == 0;
}

void LetterBag::printBagContent() const {
    for(const char letter : bag) {
        cout << letter << endl;
    }
}

unsigned int LetterBag::getScore(const char letter) {
    return get<PTS>(initTab[static_cast<unsigned int>(letter - 'A')]);
}



