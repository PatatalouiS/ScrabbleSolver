#include "playerbag.hpp"
#include <iostream>
#include <algorithm>
#include <cassert>

using namespace std;

PlayerBag::PlayerBag() : playerLetters() {
}

PlayerBag::PlayerBag(LetterBag& mainLetterBag) {
    for(unsigned int i = 0; i < MAX_SIZE; ++i ) {
        playerLetters.push_back(mainLetterBag.pickRandomLetter());
    }
}

PlayerBag::PlayerBag(const PlayerLetters& letters) : playerLetters(letters) {
    assert(letters.size() <= MAX_SIZE);
}

PlayerBag::PlayerBag(const PlayerBag& bag) : playerLetters(bag.data()) {
    assert(bag.size() <= MAX_SIZE);
}

PlayerBag PlayerBag::pop(const unsigned char letter) const {
    assert((letter >= 'A' && letter <= 'Z') || (letter == JOKER_SYMBOL));
    PlayerLetters copy(playerLetters);

    auto iterator = find(copy.begin(), copy.end(), letter);
    assert(iterator != playerLetters.end());
    copy.erase(iterator);

    return PlayerBag(copy);
}

const PlayerBag::PlayerLetters& PlayerBag::data() const {
    return playerLetters;
}

PlayerBag::PlayerLetters& PlayerBag::data() {
    return playerLetters;
}

bool PlayerBag::isEmpty() const {
    return playerLetters.empty();
}

unsigned long PlayerBag::size() const {
    return playerLetters.size();
}

PlayerBag& PlayerBag::fillWith(LetterBag& mainLetterBag) {
    while(!mainLetterBag.isEmpty() && playerLetters.size() < 7) {
        playerLetters.push_back(mainLetterBag.pickRandomLetter());
    }
    return *this;
}

std::ostream& operator << (std::ostream& stream, const PlayerBag& pb) {
    return stream << string(pb.data().begin(), pb.data().end());
}
