
#include <iostream>

#include "globals.hpp"
#include "letterbag.hpp"
#include "utils.hpp"

using namespace std;

mt19937 LetterBag::generator(random_device{}());

LetterBag::LetterBag(const bool jokers) {
    static const array<unsigned int, LetterBag::NB_SYMBOLS> occurrences {
        9, 2, 2, 3, 15, 2, 2, 2, 8, 1, 1, 5, 3,
        6, 6, 2, 3, 6, 6, 6, 6, 2, 1, 1, 1, 1
    };

    for(unsigned char letter = 'A'; letter <= 'Z'; ++letter) {
        unsigned int index = Utils::charToIndex(letter);
        if(jokers && (letter == 'Q')) {
            bag.push_back(letter);
            nbLetters++;
        }
        else {
            for(unsigned int i = 0; i < occurrences[index]; ++i) {
                bag.push_back(letter);
                nbLetters++;
            }
        }
    }

    if(jokers) {
        for(unsigned int i = 0; i < NB_JOKERS; ++i) {
             bag.push_back(JOKER_SYMBOL);
             nbLetters++;
        }
    }
}

unsigned char LetterBag::pickRandomLetter() {
    try {
        if(isEmpty()) throw string("ERROR : No letters in the bag");
    }
    catch(string& error) {
        cerr << error << endl;
        exit(EXIT_FAILURE);
    }

    uniform_int_distribution<unsigned int> distrib(0, nbLetters-1);
    const unsigned int random = distrib(generator);
    const unsigned char selectedLetter = bag[random];
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
    for(const unsigned char letter : bag) {
        cout << letter << endl;
    }
}
