#include "spot.hpp"

//display of the spot contents
std::ostream& operator<<(std::ostream& out, Spot s) {

  //triple word color
  static const char* tw = "\033[1;91m" ;
  //double word color
  static const char* dw = "\033[1;95m" ;
  //triple letter color
  static const char* tl = "\033[1;34m" ;
  //double letter color
  static const char* dl = "\033[1;96m" ;
  //reset color
  static const char* reset = "\033[1;0m" ;

  if(s.letter != 0) {
    //a letter is present, show it
    out << s.letter ;
  } else if(s.bonus.word_factor == 3) {
    //triple word
    out << tw << "3" << reset ;
  } else if(s.bonus.word_factor == 2) {
    //double word
    out << dw << "2" << reset ;
  } else if(s.bonus.letter_factor == 3) {
    //triple letter
    out << tl << "3" << reset ;
  } else if(s.bonus.letter_factor == 2) {
    //double letter
    out << dl << "2" << reset ;
  } else {
    //nothing interesting
    out << " " ;
  }
  return out ;
}
