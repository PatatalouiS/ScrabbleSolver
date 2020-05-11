#pragma once

///
/// \brief The Bonus struct represents a bonus on the Scrabble.
///     This struct was given at the beginning of the Project.
/// \author Mr Nivoliers Vincent
///

struct Bonus {

    ///
    /// \brief Bonus default constructor
    ///
    Bonus() :
    letter_factor(1), word_factor(1)
    {}

    ///
    /// \brief Bonus constructor wich takes the letter Factor and the Word Factor
    /// \param lf the letter_factor of the bonus to construt
    /// \param wf the word_factor of the bonus to construt
    ///
    Bonus(unsigned char lf, unsigned char wf) :
    letter_factor(lf), word_factor(wf)
    {}

    ///
    /// \brief apply the Bonus to a given letter score
    /// \param score the score wich receive the Bonus
    /// \return
    ///
    unsigned int apply_letter(unsigned int score) const ;

    ///
    /// \brief apply the Bonus to a given word score
    /// \param score the score wich receive the Bonus
    /// \return the computed score, with Bonus.
    ///
    unsigned int apply_word(unsigned int score) const ;

    ///
    /// \brief The letter factor of the Bonus
    ///
    unsigned char letter_factor ;

    ///
    /// \brief The word factor of the Bonus
    ///
    unsigned char word_factor ;
} ;
