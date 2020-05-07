#include "game.hpp"

Game::Game(const std::string& dicoPath, std::istream& txt, PlayerBag p) :
    dico(dicoPath),
    playerBag(p) {
    board.load(txt);
}
