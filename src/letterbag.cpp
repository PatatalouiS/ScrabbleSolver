
#include <iostream>

#include "letterbag.hpp"
#include "utils.hpp"

using namespace std;

mt19937 LetterBag::generator(random_device{}());

LetterBag::LetterBag(const bool jokers) {
    // At Scrabble Letter 'Q' is present only 1 times if we use Jokers !
    array<unsigned int, LetterBag::NB_SYMBOLS> occurrences {
        9, 2, 2, 3, 15, 2, 2, 2, 8, 1, 1, 5, 3,
        6, 6, 2, jokers ? 1u : 3u, 6, 6, 6, 6, 2, 1, 1, 1, 1
    };

    // for each letter of the Alphabet..
    for(unsigned char letter = 'A'; letter <= 'Z'; ++letter) {
        unsigned int index = Utils::charToIndex(letter);
        // put ocurrences[index] times the letter in the bag
        for(unsigned int i = 0; i < occurrences[index]; ++i) {
            bag.push_back(letter);
            nbLetters++;
        }
    }

    // if jokers are enabled
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
    // Ensure good random letter and O(1) complexity
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

const std::vector<unsigned char>& LetterBag::data() const {
    return bag;
}

std::ostream& operator<<(std::ostream& out, const LetterBag& lb) {
    for(const unsigned char letter : lb.data()) {
        out << letter << endl;
    }
    return out;
}
