#pragma once

#include "bonus.hpp"

#include <iostream>


struct SpotPos {
    unsigned char indexLine;
    unsigned char indexCol;

    SpotPos(unsigned char l = 0, unsigned char c = 0) :
        indexLine(l),
        indexCol(c) {
    }

    friend std::ostream& operator<<(std::ostream& out, const SpotPos& sp){
        out << "Line : " << int(sp.indexLine) << "  Col : " << int(sp.indexCol);
        return out;
    }
};



/* A spot on the board to place a letter */

struct Spot {

    static constexpr unsigned char EMPTY_SPOT = 0;

  //default initialization, no letter, no bonus
  Spot() :
    letter(0)
  {}

  //bonus spot initialization
  // * lf is the bonus letter factor
  // * wf is the bonus word factor
  Spot(unsigned char lf, unsigned char wf) :
    letter(0), bonus(lf, wf)
  {}

  //the letter placed on the spot, 0 if none
  unsigned char letter ;

  //the bonus present on the spot
  Bonus bonus ;


} ;

//display of the spot contents
std::ostream& operator<<(std::ostream& out, Spot s) ;
