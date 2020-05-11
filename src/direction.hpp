#ifndef DIRECTION_HPP
#define DIRECTION_HPP

#include <iostream>

///
/// \brief The Direction enum represents a Direction of a word in Scrabble.
/// \author CERBULEAN Ana-Maria, OLIVIE Maxime
///

enum class Direction {
    HORIZONTAL,
    VERTICAL
};

///
/// \brief operator << to print a Direction, for debug.
/// \param out
/// \param d
/// \return
///
std::ostream& operator<<(std::ostream& out, enum Direction d);

#endif // DIRECTION_HPP
