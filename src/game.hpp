#ifndef GAME_HPP
#define GAME_HPP

#include "board.hpp"
#include "gaddag.hpp"
#include "letterbag.hpp"

struct Game {
    Board board;
    Gaddag dico;
    LetterBag letterBag;
    PlayerBag playerBag;

    Game(const std::string& dicoPath, std::istream& txt, PlayerBag p);
};

#endif // GAME_HPP
