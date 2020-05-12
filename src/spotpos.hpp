#ifndef SPOTPOS_HPP
#define SPOTPOS_HPP

#include <iostream>

#include "direction.hpp"

///
/// \brief The SpotPos struct is a little struct to represent the coordinates
///     of a Spot in the Board. It's not an essential Class because we can
///     work with the Scrabble Board with only one index because the Spots array is linear,
///     But sometimes it's more comfortable to think with two dimensions.
///     To use with bracket initialisation.
/// \author CERBULEAN Ana-Maria, OLIVIE Maxime
///
struct SpotPos {
    ///
    /// \brief line index
    ///
    char indexLine = 0;

    ///
    /// \brief col index
    ///
    char indexCol = 0;

    ///
    /// \brief operator == Return if two Spot instances are equals.
    ///     It's used especially for template Specialization of the
    ///     std::unordered_set
    /// \param other the other Spot to campare
    /// \return true if equals, else false
    ///
    bool operator==(const SpotPos& other) const;
};

///
/// \brief operator << print a SpotPos on console. for debug.
/// \param out
/// \param sp
/// \return
///
std::ostream& operator<<(std::ostream& out, const SpotPos& sp);

namespace std {
    template<>
    struct hash<SpotPos> {
        ///
        /// \brief operator () Hash Function for a SpotPos.
        ///     template Specialization for use unordered_set
        /// \param pos the SpotPos to Hash
        /// \return the hash value
        ///
        size_t operator()(const SpotPos& pos) const;
    };

    template<>
    struct hash<std::pair<SpotPos, Direction>> {
        ///
        /// \brief operator () Hash Function for a std::pair<SpotPos, Direction>.
        ///     template Specialization for use unordered_set
        /// \param p the Move to Hash
        /// \return the hash value
        ///
        size_t operator()(const pair<SpotPos, Direction>& p) const;
    };
}

#endif // SPOTPOS_HPP
