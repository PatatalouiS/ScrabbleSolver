
#include <iostream>

#include "letterbag.hpp"
#include "utils.hpp"

using namespace std;

// Static Init Bloc

const array<LetterBag::LetterSpecs, LetterBag::nbSymbols>
    LetterBag::letterSpecsArray {{
        { 1, 9 }, { 3, 2 }, { 3, 2 }, { 2, 3 }, { 1, 15 }, { 4, 2 },
        { 2, 2 }, { 4, 2 }, { 1, 8 }, { 8, 1 }, { 10, 1 }, { 1, 5 },
        { 2, 3 }, { 1, 6 }, { 1, 6 }, { 3, 2 }, { 2, 3 }, { 1, 6 },
        { 1, 6 }, { 1, 6 }, { 1, 6 }, { 4, 2 }, { 10, 1 }, { 10, 1 },
        { 10, 1 }, { 10, 1 }
}};

mt19937 LetterBag::generator(random_device{}());

// Static Init Bloc

LetterBag::LetterBag() {
    unsigned int index;
    unsigned int occurrences;

    for(unsigned char letter = 'A'; letter <= 'Z'; ++letter) {
        index = Utils::charToIndex(letter);
        occurrences = letterSpecsArray[index].occurrences;

        for(unsigned int i = 0; i < occurrences; ++i) {
            bag.push_back(letter);
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

unsigned int LetterBag::getLetterPoints(const unsigned char letter) {
    return letterSpecsArray[Utils::charToIndex(letter)].points;
}
