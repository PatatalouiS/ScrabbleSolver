#ifndef MOVE_HPP
#define MOVE_HPP

#include <string>

#include "spotpos.hpp"
#include "direction.hpp"
#include "playerbag.hpp"

///
/// \brief The Move struct store a Move in the Scrabble game
/// \author CERBULEAN Ana-Maria, OLIVIE Maxime
struct Move {

    ///
    /// \brief word the word of the Move, In his Gaddag representation.
    ///     set To empty string "" by default
    ///
    std::string word = "";

    ///
    /// \brief Start Position of the Move
    ///     Set to (0, 0) by default
    ///
    SpotPos pos = SpotPos{0, 0};

    ///
    /// \brief Direction of the Move
    /// Set to Direction::Horizontal by default
    ///
    Direction direction = Direction::HORIZONTAL;

    ///
    /// \brief The PlayersBag after the Move (remaining letters after the Move)
    ///
    PlayerBag playerBag = PlayerBag();

    ///
    /// \brief score the Total Score of the Move
    ///
    unsigned int score = 0;

    ///
    /// \brief operator == Return if two Move instances are equals.
    ///     It's used especially for template Specialization of the
    ///     std::unordered_set
    /// \param other
    /// \return true if equals, else false
    ///
    bool operator==(const Move& other) const;
};

///
/// \brief operator << Print a Move on console
/// \param out
/// \param move
/// \return
///
std::ostream& operator <<(std::ostream& out, const Move& move);

namespace std {
    template<>
    struct hash<Move> {
        ///
        /// \brief operator () Hash Function for a Move. template Specialization.
        /// \param s the Move to Hash
        /// \return the hash value
        ///
        size_t operator()(const Move& s) const;
    };
}

#endif // MOVE_HPP
