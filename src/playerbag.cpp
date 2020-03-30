#include "playerbag.hpp"
#include <iostream>

using namespace std;

PlayerBag::PlayerBag(LetterBag& mainLetterBag)  /*letterBag(mainLetterBag)*/ {
    for(unsigned char letterOfPlayer : playerLetters) {
        letterOfPlayer = mainLetterBag.pickRandomLetter();
    }
}

PlayerBag::PlayerBag(const PlayerLetters& letters) : playerLetters(letters) {
}

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
