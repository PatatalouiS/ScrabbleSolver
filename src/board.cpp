#include "board.hpp"

#include <fstream>

//default initialization
Board::Board() {
  //Triple word spots
  {
    Spot s(1, 3) ;
    static const unsigned char sz = 9 ;
    static unsigned char triple_words[sz] = {
      0, 7, 14,
      105, 112, 119,
      210, 217, 224
    } ;
    for(unsigned char i = 0; i < sz; ++i) {
      spots[triple_words[i]] = s ;
    }
  }
  //Double word spots
  {
    Spot s(1, 2) ;
    static const unsigned char sz = 17 ;
    static unsigned char double_words[sz] = {
      16, 28,
      32, 42,
      48, 56,
      64, 70,
      112,
      154, 160,
      168, 176,
      182, 192,
      196, 208
    } ;
    for(unsigned char i = 0; i < sz; ++i) {
      spots[double_words[i]] = s ;
    }
  }
  //Tripple letter spots
  {
    Spot s(3, 1) ;
    static const unsigned char sz = 12 ;
    static unsigned char triple_letters[sz] = {
      20, 24,
      76, 80, 84, 88,
      136, 140, 144, 148,
      200, 204
    } ;
    for(unsigned char i = 0; i < sz; ++i) {
      spots[triple_letters[i]] = s ;
    }
  }
  //Double letter spots
  {
    Spot s(2, 1) ;
    static const unsigned char sz = 24 ;
    static unsigned char double_letters[sz] = {
      3, 11,
      36, 38,
      45, 52, 59,
      92, 96, 98, 102,
      108, 116,
      122, 126, 128, 132,
      165, 172, 179,
      186, 188,
      213, 221
    } ;
    for(unsigned char i = 0; i < sz; ++i) {
      spots[double_letters[i]] = s ;
    }
  }
}

//i/o to files
void Board::save(std::ostream& out) {
  //write the grid of chars
  for(unsigned char i = 0; i < 225; ++i) {
    if( i%15 == 0) {
      out << std::endl ;
    }
    if(spots[i].letter) {
      //a letter is present, write it
      out << spots[i].letter ;
    } else {
      //no letter, put a dot
      out << "." ;
    }
  }
}

void Board::load(std::istream& in) {
  char c ;
  //read a char for each spot
  for(unsigned char i = 0; i < 225; ++i) {
    in >> c ;
    if(c != '.') {
      //a letter is provided, save it
      spots[i].letter = c ;
    } else {
      //no letter, set to 0
      spots[i].letter = 0 ;
    }
  }
}

//access to the spots by coordinates
Spot Board::operator()(unsigned char l, unsigned char c) const {
  return spots[l*15 + c] ;
}

Spot& Board::operator()(unsigned char l, unsigned char c) {
  return spots[l*15 + c] ;
}

//display on the console
std::ostream& operator<<(std::ostream& out, const Board& b) {

  static const char* grey = "\033[1;90m" ;
  static const char* reset = "\033[1;0m" ;

  //top frame line
  for(int i = 0; i < 67; ++i) {
    out << "#" ;
  }
  out << std::endl ;

  //rows top symbols
  for(unsigned char i = 0; i < 15; ++i) {
    //left frame
    out << "#  " ;
    //columns first line
    for(unsigned char j = 0; j < 15; ++j) {
      //type of the top left corner
      bool above = (i > 0 && (b(i-1,j).letter != 0)) ;
      bool weak_above = (above || (i > 0 && j > 0 && b(i-1,j-1).letter != 0)) ;
      bool below = (b(i,j).letter != 0) ;
      bool weak_below = (below || (j > 0 && b(i, j-1).letter != 0)) ;
      if (weak_above && weak_below) {
        //a letter is placed on both sides
        out << ":" ;
      } else if (weak_above) {
        //letters only above
        out << "'" ;
      } else if (weak_below) {
        //letters only below
        out << "." ;
      } else {
        //no letter around
        out << grey << "+" << reset ;
      }
      //type of the top edge
      if(above && below) {
        //both sides contain letters
        out << "---" ;
      } else if(above || below) {
        //a letter only on one side
        out << "---" ;
      } else {
        //no letter around
        out << "   " ;
      }
      //final top right corner
      if(j == 14) {
        if (above && below) {
          //letters on both sides
          out << ":" ;
        } else if (above) {
          //letter above
          out << "'" ;
        } else if (below) {
          //letter below
          out << "." ;
        } else {
          //no letter around
          out << grey << "+" << reset ;
        }
      }
    }
    //right frame and left frame of next line
    out << "  #" << std::endl << "#  " ;
    //columns second line
    for(unsigned char j = 0; j < 15; ++j) {
      //type of the left vertical edge
      bool left = (j > 0 && b(i,j-1).letter != 0) ;
      bool right = b(i,j).letter != 0 ;
      if(left || right) {
        //a letter is present at least on one side
        out << "| " ;
      } else {
        //no letter around
        out << "  " ;
      }
      //contents of the spot
      out << b(i,j) << " " ;
      //right vertical edge of the last spot on the line
      if(j == 14) {
        if(right) {
          //a letter is present
          out << "|" ;
        } else {
          //no letter around
          out << " " ;
        }
      }
    }
    //final frame
    out << "  #" << std::endl ;
  }
  //lower part of last line
  out << "#  " ;
  for(unsigned char j = 0; j < 15; ++j) {
    //corner type
    bool above = (b(14,j).letter != 0) ;
    bool weak_above = (above || (j > 0 && b(14,j-1).letter != 0)) ;

    if (weak_above) {
      //a letter is above
      out << "'" ;
    } else {
      //no letter around
      out << grey << "+"<< reset ;
    }
    //edge type
    if(above) {
      //a letter is above
      out << "---" ;
    } else {
      //no letter above
      out << "   " ;
    }
    //last corner type
    if(j == 14) {
      if(above) {
        //a letter is present
        out << "." ;
      } else {
        //no letter around
        out << grey << "+" << reset ;
      }
    }
  }
  //finalize frame
  out << "  #" << std::endl ;
  for(int i = 0; i < 67; ++i) {
    out << "#" ;
  }
  out << std::endl ;

  return out ;
}

