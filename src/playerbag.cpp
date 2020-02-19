#include "playerbag.hpp"

PlayerBag::PlayerBag(LetterBag& mainLetterBag) {
    for(char& letterOfPlayer : playerLetters) {
        letterOfPlayer = mainLetterBag.pickRandomLetter();
    }
}
