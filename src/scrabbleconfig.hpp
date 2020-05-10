#ifndef SCRABBLECONFIG_HPP
#define SCRABBLECONFIG_HPP

#include "board.hpp"

struct ScrabbleConfig {

    PlayerBag playerBag;

    Board board;

    static ScrabbleConfig loadFromFile(const std::string& path);
};


#endif // SCRABBLECONFIG_HPP