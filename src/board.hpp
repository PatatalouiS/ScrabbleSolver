#pragma once

#include "spot.hpp"
#include "gaddag.hpp"
#include "letterbag.hpp"
#include "playerbag.hpp"
#include "stroke.hpp"

#include <iostream>
#include <string>

/* Scrabble board */

struct Board {
  //default initialization of a scrabble board
  Board() ;

  static constexpr unsigned int SIZE = 15;

  //i/o to files
  void save(std::ostream& out) ;
  void load(std::istream& in) ;

  void putWord(const SpotPos& pos, const std::string& w, const Direction d);
  void putStroke(const Stroke& stroke);

  //access to the spots by coordinates
  Spot operator()(unsigned char ql, unsigned char c) const ;
  Spot& operator()(unsigned char l, unsigned char c) ;

  Spot operator() (const SpotPos& sp) const;
  Spot& operator() (const SpotPos& sp) ;

  //spots are public, and can therefore also be accessed b index
  Spot spots[225] ;
} ;

//board display on the console
std::ostream& operator<<(std::ostream& out, const Board& b) ;

struct Game {
    Board board;
    Gaddag dico;
    LetterBag letterBag;

    PlayerBag playerBag;

    Game(const std::string& path, std::istream& txt, PlayerBag p) :
        dico(path),
        playerBag(p) {
        board.load(txt);
    }

    ~Game() {}
};
