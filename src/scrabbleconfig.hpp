#ifndef SCRABBLECONFIG_HPP
#define SCRABBLECONFIG_HPP

#include "board.hpp"

///
/// \brief The ScrabbleConfig struct represent a game configuration in
///     the Scrabble Game. ( A Board and a set of possible letters to play ).
/// \author CERBULEAN Ana-Maria, OLIVIE Maxime
///
struct ScrabbleConfig {
    ///
    /// \brief a PlayerBag
    ///
    PlayerBag playerBag;
    ///
    /// \brief a Board of Scrabble Game
    ///
    Board board;
    ///
    /// \brief loadFromFile this static method return a new ScrabbleConfig
    ///     constructed with a File path given.
    /// \param path the Path to the File wich contains the ScrabbleConfig
    /// \return an instance of ScrabbleConfig
    ///
    static ScrabbleConfig loadFromFile(const std::string& path);
};

#endif // SCRABBLECONFIG_HPP
