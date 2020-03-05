
#include <iostream>

#include "letterbag.hpp"
#include "utils.hpp"

using namespace std;

// Static Init Bloc

const array<unsigned int, LetterBag::nbSymbols> LetterBag::symbolPoints {
    1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 10, 1, 2,
    1, 1, 3, 2, 1, 1, 1, 1, 4, 10, 10, 10, 10
};

const array<unsigned int, LetterBag::nbSymbols> LetterBag::symbolOccurrences {
    9, 2, 2, 3, 15, 2, 2, 2, 8, 1, 1, 5, 3,
    6, 6, 2, 3, 6, 6, 6, 6, 2, 1, 1, 1, 1
};

mt19937 LetterBag::generator(random_device{}());

// Static Init Bloc


LetterBag::LetterBag() {
    unsigned int index;
    unsigned int occurrences;

    for(char letter = 'A'; letter <= 'Z'; ++letter) {
        index = Utils::charToIndex(letter);
        occurrences = symbolOccurrences[index];

        for(unsigned int i = 0; i < occurrences; ++i) {
            bag.push_back(letter);
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

    uniform_int_distribution<unsigned int> distrib(0, nbLetters-1);
    const unsigned int random = distrib(generator);
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

unsigned int LetterBag::getLetterPoints(const char letter) {
    return symbolPoints[Utils::charToIndex(letter)];
}
