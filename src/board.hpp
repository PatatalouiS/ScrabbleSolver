#pragma once

#include "spot.hpp"
#include "move.hpp"

#include <sstream>

///
/// \brief The Board struct represents the Scrabble Board Game and some methods
///     for I/O to files the Board. This struct was given at the beginning of
///     the Project.
/// \author Mr Nivoliers Vincent
///

struct Board {
    ///
    /// \brief Board Default constructor. Initialize an empty Board, with no letters.
    ///
    Board() ;

    ///
    /// \brief Board This Constructor take an std::istream at input for initialise
    ///     the Board with a given configuration. (From File, stringstream ..)
    /// \param ss The input stream wich contains the configuration to construct
    ///
    Board(std::istream& ss);

    ///
    /// \brief Board copy Constructor
    /// \param b The Board to copy
    ///
    Board(const Board& b);

    ///
    /// \brief SIZE the Max SIZE of the Board
    ///
    static constexpr unsigned int SIZE = 15;

    ///
    /// \brief Save the Board in a std::ostream given. (to File, stringstream..)
    /// \param out the stream used to save the Board configuration.
    ///
    void save(std::ostream& out) const;

    ///
    /// \brief Load into the Board a configuration given. Similar to istream
    ///     constructor
    /// \param in a stream wich contains the configuration to load in Board.
    ///
    void load(std::istream& in) ;

    ///
    /// \brief place a word on the Scrabble Board
    /// \param pos A SpotPos wich contains the position of the first letter
    /// \param w the word to place on the Board
    /// \param d the Direction of the word to place
    /// \return a reference to the current Board, for chainning
    ///
    Board& putWord(const SpotPos& pos, const std::string& w, const Direction d);

    ///
    /// \brief place a computed Move on the Board. Similar to putWord.
    /// \param Move the Move to place
    /// \return a reference to the current Board, for chaining.
    ///
    Board& putMove(const Move& Move);

    ///
    /// \brief operator () allows to access a Spot on the Board by Coordinates
    /// \param l index of the line
    /// \param c index of the column
    /// \return a Copy of the SpotPos at the given coordinates
    ///
    Spot operator()(unsigned char l, unsigned char c) const ;

    ///
    /// \brief operator () allows to access a Spot on the Board by Coordinates
    /// \param l index of the line
    /// \param c index of the column
    /// \return a Copy of the SpotPos at the given coordinates
    ///
    Spot& operator()(unsigned char l, unsigned char c) ;

    ///
    /// \brief operator () allows to access a Spot on the Board by a SpotPos
    /// \param sp a SpotPos wich contains the coordinates
    /// \return
    ///
    Spot operator() (const SpotPos& sp) const;

    ///
    /// \brief operator () allows to access a Spot on the Board by a SpotPos
    /// \param sp a SpotPos wich contains the coordinates
    /// \return
    ///
    Spot& operator() (const SpotPos& sp) ;

    ///
    /// \brief check if the Board contains no letters
    /// \return true if empty, else false
    ///
    bool isEmpty() const;

    ///
    /// \brief The array of Board Spots.
    ///
    Spot spots[225] ;
} ;

///
/// \brief operator << allows to print a Board, for debug
/// \param out
/// \param b
/// \return
///
std::ostream& operator<<(std::ostream& out, const Board& b) ;
