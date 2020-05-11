#pragma once

#include <iostream>

#include "bonus.hpp"

///
/// \brief The Spot struct represents a Spot on the Scrabble Board.
///     This struct was given at the beginning of the Project.
/// \author Mr Nivoliers Vincent
///
struct Spot {
    ///
    /// \brief Spot default constructor, with no bonus, no letter.
    ///
	Spot() :
		letter(0)
	{}

    ///
    /// \brief Spot constructor with take the letter factor and the word factor.
    /// \param lf the letter factor
    /// \param wf the word factor
    ///
	Spot(unsigned char lf, unsigned char wf) :
		letter(0), bonus(lf, wf)
	{}

    ///
    /// \brief The letter of the Spot
    ///
	unsigned char letter ;

    ///
    /// \brief The bonus of the Spot
    ///
	Bonus bonus ;

    ///
    /// \brief Check if the Spot is empty (contains no letter)
    /// \return true if empty, else false
    ///
	bool isEmpty() const;
} ;

///
/// \brief operator << print a Spot on console, for debug
/// \param out
/// \param s
/// \return
///
std::ostream& operator<<(std::ostream& out, Spot s);
