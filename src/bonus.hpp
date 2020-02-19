#pragma once

/* Bonus */

struct Bonus {
  //no bonus by default, factors are 1
  Bonus() : 
    letter_factor(1), word_factor(1) 
  {}

  //initialization with provided factors
  // * lf is the letter factor
  // * wf is the word factor
  Bonus(unsigned char lf, unsigned char wf) :
    letter_factor(lf), word_factor(wf) 
  {}

  //apply the bonus to scores
  unsigned int apply_letter(unsigned int score) const ;
  unsigned int apply_word(unsigned int score) const ;

  //stored factors
  unsigned char letter_factor ;
  unsigned char word_factor ;
} ;
