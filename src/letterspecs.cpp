#include "letterspecs.hpp"
#include <iostream>

using namespace std;

LetterSpecs::LetterSpecs(const SpecsTuple specs) {
    this->symbol = get<SYMB>(specs);
    this->points = get<PTS>(specs);
    this->number = get<NB>(specs);
}

unsigned int LetterSpecs::getAvailableNumber() const {
    return number;
}

void LetterSpecs::pickOne() {
    if(number <= 0) {
        cout << "No letter " << symbol << "available" << endl;
        return;
    }
    number--;
}

void LetterSpecs::putOne() {
    number++;
}
