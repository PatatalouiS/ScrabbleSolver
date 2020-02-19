#include "bonus.hpp"

//apply the letter factor on a letter score
unsigned int Bonus::apply_letter(unsigned int score) const {
  return letter_factor * score ;
}

//apply the word factor on a word score
unsigned int Bonus::apply_word(unsigned int score) const {
  return word_factor * score ;
}
