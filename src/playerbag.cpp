#include "playerbag.hpp"
#include <iostream>

using namespace std;

PlayerBag::PlayerBag() : playerLetters() {
}

PlayerBag::PlayerBag(LetterBag& mainLetterBag) {
    for(unsigned int i = 0; i < MAX_SIZE; ++i ) {
        playerLetters.push_back(mainLetterBag.pickRandomLetter());
    }
}

PlayerBag::PlayerBag(const PlayerLetters& letters) : playerLetters(letters) {
}

PlayerBag::PlayerBag(const PlayerBag& bag) : playerLetters(bag.data()) {}

PlayerBag PlayerBag::pop(const unsigned char letter) const {
    PlayerLetters copy(playerLetters);

    auto iterator = find(copy.begin(), copy.end(), letter);

    if(iterator != playerLetters.end()) {
        copy.erase(iterator);
    }

    return PlayerBag(copy);
}

const PlayerBag::PlayerLetters& PlayerBag::data() const {
    return playerLetters;
}

void PlayerBag::print() const {
    for(unsigned char c : playerLetters) {
        cout << c;
    }
    cout << endl;
}

PlayerBag::PlayerLetters& PlayerBag::data() {
    return playerLetters;
}

bool PlayerBag::isEmpty() const {
    return playerLetters.empty();
}

PlayerBag& PlayerBag::fillWith(LetterBag& mainLetterBag) {
    while(!mainLetterBag.isEmpty() && playerLetters.size() < 7) {
        playerLetters.push_back(mainLetterBag.pickRandomLetter());
    }
    return *this;
}

std::ostream& operator << (std::ostream& stream, const PlayerBag& pb) {
    string letters(pb.data().begin(), pb.data().end());
    return stream << letters;
}
